//*********************************************************
//
// Copyright 2020 Intel Corporation 
//
// Permission is hereby granted, free of charge, to any 
// person obtaining a copy of this software and associated 
// documentation files(the "Software"), to deal in the Software 
// without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to 
// whom the Software is furnished to do so, subject to the 
// following conditions :
// The above copyright notice and this permission notice shall 
// be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.
//
//*********************************************************

#include "pch.h"

#include "TileUpdateManager.h"
#include "Interfaces.h"
#include "XeTexture.h"

#define USE_DEDICATED_UPDATERESIDENCY_THREAD 1

#define COPY_RESIDENCY_MAPS 0

//=============================================================================
// data structure to manage reserved resource
//=============================================================================
TileUpdateManager::TileUpdateManager(
    // query resource for tiling properties. use its device to create internal resources
    ID3D12Device8* in_pDevice,

    // the Direct command queue the application is using to render, which TUM monitors to know when new feedback is ready
    ID3D12CommandQueue* in_pDirectCommandQueue,

    const TileUpdateManagerDesc& in_desc) :
m_numSwapBuffers(in_desc.m_swapChainBufferCount)
, m_gpuTimerResolve(in_pDevice, in_desc.m_swapChainBufferCount, D3D12GpuTimer::TimerType::Direct)
, m_sharedFrameIndex(0)
, m_directCommandQueue(in_pDirectCommandQueue)
, m_withinFrame(false)
, m_device(in_pDevice)
, m_commandLists((UINT)CommandListName::Num)
, m_maxTileMappingUpdatesPerApiCall(in_desc.m_maxTileMappingUpdatesPerApiCall)
, m_maxTileCopiesPerBatch(in_desc.m_maxTileCopiesPerBatch)
{
    ASSERT(D3D12_COMMAND_LIST_TYPE_DIRECT == in_pDirectCommandQueue->GetDesc().Type);

    ThrowIfFailed(in_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_frameFence)));
    m_frameFence->SetName(L"TileUpdateManager::m_frameFence");

    // NOTE: DataUploader creates an atlas, which determines the heap size.
    m_pDataUploader = std::make_unique<Streaming::DataUploader>(
        in_pDevice,
        in_desc.m_maxNumCopyBatches,
        in_desc.m_maxTileCopiesPerBatch,
        in_desc.m_maxTileCopiesInFlight,
        in_desc.m_maxTileMappingUpdatesPerApiCall,
        in_desc.m_timingNumBatchesToCapture);

    const UINT numAllocators = m_numSwapBuffers;
    for (UINT c = 0; c < (UINT)CommandListName::Num; c++)
    {
        auto& cl = m_commandLists[c];
        cl.m_allocators.resize(numAllocators);
        for (UINT i = 0; i < numAllocators; i++)
        {
            ThrowIfFailed(in_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cl.m_allocators[i])));

            std::wstringstream name;
            name << "Shared::m_commandLists.m_allocators[" << c << "][" << i << "]";
            cl.m_allocators[i]->SetName(name.str().c_str());
        }
        ThrowIfFailed(in_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cl.m_allocators[m_sharedFrameIndex].Get(), nullptr, IID_PPV_ARGS(&cl.m_commandList)));

        std::wstringstream name;
        name << "Shared::m_commandLists.m_commandList[" << c << "]";
        cl.m_commandList->SetName(name.str().c_str());
        cl.m_commandList->Close();
    }

    // advance frame number to the first frame...
    m_frameFenceValue++;


    UseDirectStorage(in_desc.m_useDirectStorage);
}

TileUpdateManager::~TileUpdateManager()
{
    // DataUploader destructor flushes outstanding commands, but we want to make sure that's the first thing that happens in the desctructor for TileUpdateManager
    Finish();
}

//-----------------------------------------------------------------------------
// performance and visualization
//-----------------------------------------------------------------------------
const TileUpdateManager::BatchTimes& TileUpdateManager::GetBatchTimes() const { return m_pDataUploader->GetStreamingTimes(); }
float TileUpdateManager::GetGpuStreamingTime() const { return m_pDataUploader->GetGpuStreamingTime(); }

// feedback resolve + readback
float TileUpdateManager::GetGpuTime() const { return m_gpuTimerResolve.GetTimes()[m_sharedFrameIndex].first; }
UINT TileUpdateManager::GetTotalNumUploads() const { return m_pDataUploader->GetTotalNumUploads(); }
UINT TileUpdateManager::GetTotalNumEvictions() const { return m_pDataUploader->GetTotalNumEvictions(); }
void TileUpdateManager::SetVisualizationMode(UINT in_mode) { m_pDataUploader->SetVisualizationMode(in_mode); }

// returns time since last query. expected usage is once per frame.
float TileUpdateManager::GetProcessFeedbackTime()
{
    // read latest cumulative time
    INT64 processFeedbackTime = m_processFeedbackTime;

    float t = m_cpuTimer.GetSecondsFromDelta(processFeedbackTime - m_previousFeedbackTime);

    // remember current time for next call
    m_previousFeedbackTime = processFeedbackTime;

    return t;
}

//-----------------------------------------------------------------------------
// set which file streaming system to use
// will reset even if previous setting was the same. so?
//-----------------------------------------------------------------------------
void TileUpdateManager::UseDirectStorage(bool in_useDS)
{
    Finish();
    auto streamerType = Streaming::DataUploader::StreamerType::Reference;
    if (in_useDS)
    {
        streamerType = Streaming::DataUploader::StreamerType::DirectStorage;
    }

    auto pOldStreamer = m_pDataUploader->SetStreamer(streamerType);

    for (auto& s : m_streamingResources)
    {
        s->SetFileHandle(m_pDataUploader.get());
    }

    delete pOldStreamer;
}

//-----------------------------------------------------------------------------
// kick off thread that continuously streams tiles
// gives StreamingResources opportunities to update feedback
//-----------------------------------------------------------------------------
void TileUpdateManager::StartProcessFeedbackThread()
{
    if (m_processFeedbackThreadRunning)
    {
        return;
    }

    m_processFeedbackThreadRunning = true;

    m_processFeedbackThread = std::thread([&]
        {
            DebugPrint(L"Created Feedback Thread\n");

            // NOTE: expects the streaming resource array size to be unchanged during thread lifetime

            // array of indices to resources that need tiles loaded/evicted
            std::vector<UINT> staleResources;
            staleResources.reserve(m_streamingResources.size());

            // flags to prevent duplicates in the staleResources array
            std::vector<BYTE> pending(m_streamingResources.size(), 0);

            UINT64 previousFrameFenceValue = m_frameFenceValue;
            while (m_processFeedbackThreadRunning)
            {
                // DEBUG: verify that no streaming resources have been added/removed during thread lifetime
                ASSERT(m_streamingResources.size() == pending.size());

                UINT64 frameFenceValue = m_frameFence->GetCompletedValue();

                // Only process feedback buffers once per frame
                if (previousFrameFenceValue != frameFenceValue)
                {
                    previousFrameFenceValue = frameFenceValue;

                    auto startTime = m_cpuTimer.GetTime();
                    UINT j = 0;
                    for (auto p : m_streamingResources)
                    {
                        // early exit, important for application exit or TUM::Finish() when adding/deleting objects
                        if (!m_processFeedbackThreadRunning) return;

                        p->NextFrame();
                        p->ProcessFeedback(frameFenceValue);
                        if (p->IsStale() && !pending[j])
                        {
                            staleResources.push_back(j);
                            pending[j] = 1;
                        }
                        j++;
                    }
                    m_processFeedbackTime += UINT64(m_cpuTimer.GetTime() - startTime);
                }

                // continuously push uploads and evictions
                for (UINT i = 0; i < staleResources.size(); i++)
                {
                    if (!m_processFeedbackThreadRunning) return;

                    UINT resourceIndex = staleResources[i];
                    auto p = m_streamingResources[resourceIndex];
                    p->QueueTiles();

                    // if all loads/evictions handled, remove from staleResource list
                    if (!p->IsStale())
                    {
                        pending[resourceIndex] = 0; // clear the flag that prevents duplicates
                        // compact the array by swapping this entry with the last
                        staleResources[i] = staleResources.back();
                        staleResources.resize(staleResources.size() - 1);
                    }
                }

#if (0 == USE_DEDICATED_UPDATERESIDENCY_THREAD)
                // continuously modify residency maps as a result of gpu completion events
                // FIXME? there seems to be enough time in this thread to also updateminmipmaps (scene dependent)
                // Note that UpdateMinMipMap() exits quickly if nothing to do
                for (auto p : m_streamingResources)
                {
                    p->UpdateMinMipMap();
                }
#endif
            }
            DebugPrint(L"Destroyed ProcessFeedback Thread\n");
        });

#if USE_DEDICATED_UPDATERESIDENCY_THREAD
    m_updateResidencyThread = std::thread([&]
        {
            DebugPrint(L"Created UpdateResidency Thread\n");
            // continuously modify residency maps as a result of gpu completion events
            // FIXME? probably not enough work to deserve it's own thread
            // Note that UpdateMinMipMap() exits quickly if nothing to do
            while (m_processFeedbackThreadRunning)
            {
                for (auto p : m_streamingResources)
                {
                    p->UpdateMinMipMap();
                }
                _mm_pause();
            }
            DebugPrint(L"Destroyed UpdateResidency Thread\n");
        });
#endif
}

//-----------------------------------------------------------------------------
// flushes all internal queues
// submits all outstanding command lists
// stops all processing threads
//-----------------------------------------------------------------------------
void TileUpdateManager::Finish()
{
    ASSERT(!GetWithinFrame());

    m_pDataUploader->FlushCommands();
    
    // stop TileUpdateManager threads
    // do not want ProcessFeedback generating more work
    // don't want UpdateResidency to write to min maps when that might be replaced
    m_processFeedbackThreadRunning = false;

    if (m_processFeedbackThread.joinable())
    {
        m_processFeedbackThread.join();
        DebugPrint(L"JOINED ProcessFeedback Thread\n");
    }

    if (m_updateResidencyThread.joinable())
    {
        m_updateResidencyThread.join();
        DebugPrint(L"JOINED UpdateResidency Thread\n");
    }
}

//-----------------------------------------------------------------------------
// Heap to old a number of 64KB tiles
//-----------------------------------------------------------------------------
Streaming::Heap* TileUpdateManager::CreateStreamingHeap(UINT in_maxNumTilesHeap)
{
    return new Streaming::Heap(m_pDataUploader->GetMappingQueue(), in_maxNumTilesHeap);
}

//-----------------------------------------------------------------------------
// create a StreamingResource, passing in a version of TileUpdateManager with extra methods
//-----------------------------------------------------------------------------
StreamingResource* TileUpdateManager::CreateStreamingResource(const std::wstring& in_filename, Streaming::Heap* in_pHeap)
{
    ASSERT(!m_processFeedbackThreadRunning);

    Streaming::FileStreamer::FileHandle* pFileHandle = m_pDataUploader->OpenFile(in_filename);
    Streaming::StreamingResourceTUM* pRsrc = new Streaming::StreamingResourceTUM(in_filename, pFileHandle, (Streaming::TileUpdateManagerSR*)this, in_pHeap);
    m_streamingResources.push_back(pRsrc);
    m_numStreamingResourcesChanged = true;
    return pRsrc;
}

//-----------------------------------------------------------------------------
// allocate residency map buffer large enough for numswapbuffers * min mip map buffers for each StreamingResource
// StreamingResource::SetResidencyMapOffsetBase() will populate the residency map with latest
// descriptor handle required to update the assoiated shader resource view
//-----------------------------------------------------------------------------
void TileUpdateManager::AllocateResidencyMap(D3D12_CPU_DESCRIPTOR_HANDLE in_descriptorHandle)
{
    static const UINT alignment = 32; // these are bytes, so align by 32 corresponds to SIMD32
    static const UINT minBufferSize = 64 * 1024; // multiple of 64KB page

    UINT oldBufferSize = 0;
    if (nullptr != m_residencyMap.m_resource)
    {
        oldBufferSize = (UINT)m_residencyMap.m_resource->GetDesc().Width;
    }

    // allocate residency map buffer large enough for numswapbuffers * min mip map buffers for each StreamingResource
    m_residencyMapOffsets.resize(m_streamingResources.size());
    UINT offset = 0;
    for (UINT i = 0; i < (UINT)m_residencyMapOffsets.size(); i++)
    {
        m_residencyMapOffsets[i] = offset;

        UINT minMipMapSize = m_streamingResources[i]->GetMinMipMapWidth() * m_streamingResources[i]->GetMinMipMapHeight();

        offset += minMipMapSize;

        offset = (offset + alignment - 1) & ~(alignment-1);
    }

    if (offset > oldBufferSize)
    {
        UINT bufferSize = std::max(offset, minBufferSize);
        m_residencyMap.Allocate(m_device.Get(), bufferSize);

        CreateMinMipMapView(in_descriptorHandle);

#if COPY_RESIDENCY_MAPS
        const auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
        const auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
        m_device->CreateCommittedResource(
            &heapProperties,
            D3D12_HEAP_FLAG_NONE, &resourceDesc,
            D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, nullptr,
            IID_PPV_ARGS(&m_residencyMapLocal));
        m_residencyMapLocal->SetName(L"m_residencyMapLocal");
#endif
    }

    // set offsets AFTER allocating resource. allows StreamingResource to initialize buffer state
    for (UINT i = 0; i < (UINT)m_streamingResources.size(); i++)
    {
        m_streamingResources[i]->SetResidencyMapOffsetBase(m_residencyMapOffsets[i]);
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void TileUpdateManager::CreateMinMipMapView(D3D12_CPU_DESCRIPTOR_HANDLE in_descriptorHandle)
{
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
    srvDesc.Buffer.NumElements = (UINT)m_residencyMap.m_resource->GetDesc().Width;
    srvDesc.Format = DXGI_FORMAT_R8_UINT;
    // there is only 1 channel
    srvDesc.Shader4ComponentMapping = D3D12_ENCODE_SHADER_4_COMPONENT_MAPPING(0, 0, 0, 0);

#if COPY_RESIDENCY_MAPS
    m_device->CreateShaderResourceView(m_residencyMapLocal.Get(), &srvDesc, in_descriptorHandle);
#else
    m_device->CreateShaderResourceView(m_residencyMap.m_resource.Get(), &srvDesc, in_descriptorHandle);
#endif
}

//-----------------------------------------------------------------------------
// Call this method once for each TileUpdateManager that shares heap/upload buffers
// expected to be called once per frame, before anything is drawn.
//-----------------------------------------------------------------------------
void TileUpdateManager::BeginFrame(ID3D12DescriptorHeap* in_pDescriptorHeap,
    D3D12_CPU_DESCRIPTOR_HANDLE in_minmipmapDescriptorHandle)
{
    ASSERT(!GetWithinFrame());
    m_withinFrame = true;

    StartProcessFeedbackThread();

    // if new StreamingResources have been created...
    if (m_numStreamingResourcesChanged)
    {
        m_numStreamingResourcesChanged = false;
        AllocateResidencyMap(in_minmipmapDescriptorHandle);
    }

    // the frame fence is used to optimize readback of feedback
    // only read back the feedback after the frame that writes to it has completed
    // note the signal is for the previous frame, the value is for "this" frame
    m_directCommandQueue->Signal(m_frameFence.Get(), m_frameFenceValue);
    m_frameFenceValue++;

    m_sharedFrameIndex = (m_sharedFrameIndex + 1) % m_numSwapBuffers;
    for (auto& cl : m_commandLists)
    {
        auto& allocator = cl.m_allocators[m_sharedFrameIndex];
        allocator->Reset();
        ThrowIfFailed(cl.m_commandList->Reset(allocator.Get(), nullptr));
    }
    ID3D12DescriptorHeap* ppHeaps[] = { in_pDescriptorHeap };
    GetCommandList(CommandListName::Before)->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

//-----------------------------------------------------------------------------
// note to self to create Clear() and Resolve() commands during EndFrame()
//-----------------------------------------------------------------------------
void TileUpdateManager::QueueFeedback(StreamingResource* in_pResource, D3D12_GPU_DESCRIPTOR_HANDLE in_gpuDescriptor)
{
    Streaming::StreamingResourceTUM* pResource = (Streaming::StreamingResourceTUM*)in_pResource;

    m_feedbackReadbacks.push_back({ pResource, in_gpuDescriptor });

    // add feedback clears
    pResource->ClearFeedback(GetCommandList(CommandListName::Before), in_gpuDescriptor);

    // barrier coalescing around blocks of commands in EndFrame():

    // after drawing, transition the opaque feedback resources from UAV to resolve source
    // transition the feedback decode target to resolve_dest
    m_barrierUavToResolveSrc.push_back(CD3DX12_RESOURCE_BARRIER::Transition(pResource->GetOpaqueFeedback(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_RESOLVE_SOURCE));

    // after resolving, transition the opaque resources back to UAV. Transition the resolve destination to copy source for read back on cpu
    m_barrierResolveSrcToUav.push_back(CD3DX12_RESOURCE_BARRIER::Transition(pResource->GetOpaqueFeedback(), D3D12_RESOURCE_STATE_RESOLVE_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));

#if RESOLVE_TO_TEXTURE
    // resolve to texture incurs a subsequent copy to linear buffer
    m_barrierUavToResolveSrc.push_back(CD3DX12_RESOURCE_BARRIER::Transition(pResource->GetResolvedFeedback(), D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_RESOLVE_DEST));
    m_barrierResolveSrcToUav.push_back(CD3DX12_RESOURCE_BARRIER::Transition(pResource->GetResolvedFeedback(), D3D12_RESOURCE_STATE_RESOLVE_DEST, D3D12_RESOURCE_STATE_COPY_SOURCE));
#endif
}

//-----------------------------------------------------------------------------
// Call this method once corresponding to BeginFrame()
// expected to be called once per frame, after everything was drawn.
//-----------------------------------------------------------------------------
TileUpdateManager::CommandLists TileUpdateManager::EndFrame()
{
    ASSERT(GetWithinFrame());
    // NOTE: we are "within frame" until the end of EndFrame()

    // transition packed mips if necessary
    // FIXME? if any 1 needs a transition, go ahead and check all of them. not worth optimizing.
    if (m_packedMipTransition)
    {
        m_packedMipTransition = false;
        for (auto o : m_streamingResources)
        {
            if (o->GetPackedMipsNeedTransition())
            {
                D3D12_RESOURCE_BARRIER b = CD3DX12_RESOURCE_BARRIER::Transition(
                    o->GetTiledResource(),
                    D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
                m_packedMipTransitionBarriers.push_back(b);
            }
        }
    }

    //------------------------------------------------------------------
    // before draw calls, do the following:
    //     - clear feedback buffers
    //     - resource barriers for aliasing and packed mip transitions
    //------------------------------------------------------------------
    {
        auto pCommandList = GetCommandList(CommandListName::Before);

        // get any packed mip transition barriers accumulated by DataUploader
        if (m_packedMipTransitionBarriers.size())
        {
            pCommandList->ResourceBarrier((UINT)m_packedMipTransitionBarriers.size(), m_packedMipTransitionBarriers.data());
            m_packedMipTransitionBarriers.clear();
        }

#if COPY_RESIDENCY_MAPS
        // FIXME: would rather update multiple times per frame, and only affected regions
        D3D12_RESOURCE_BARRIER residencyBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_residencyMapLocal.Get(),
            D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_COPY_DEST);
        pCommandList->ResourceBarrier(1, &residencyBarrier);

        pCommandList->CopyResource(m_residencyMapLocal.Get(), m_residencyMap.m_resource.Get());

        std::swap(residencyBarrier.Transition.StateBefore, residencyBarrier.Transition.StateAfter);
        pCommandList->ResourceBarrier(1, &residencyBarrier);
#endif
        pCommandList->Close();
    }

    //------------------------------------------------------------------
    // after draw calls,
    // resolve feedback and copy to readback buffers
    //------------------------------------------------------------------
    {
        auto pCommandList = GetCommandList(CommandListName::After);

        if (m_feedbackReadbacks.size())
        {
            m_gpuTimerResolve.BeginTimer(pCommandList, m_sharedFrameIndex);

            // transition all feeback resources UAV->RESOLVE_SOURCE
            // also transition the (non-opaque) resolved resources COPY_SOURCE->RESOLVE_DEST
            pCommandList->ResourceBarrier((UINT)m_barrierUavToResolveSrc.size(), m_barrierUavToResolveSrc.data());
            m_barrierUavToResolveSrc.clear();

            // do the feedback resolves
            for (auto& t : m_feedbackReadbacks)
            {
                t.m_pStreamingResource->ResolveFeedback(pCommandList);
            }

            // transition all feedback resources RESOLVE_SOURCE->UAV
            // also transition the (non-opaque) resolved resources RESOLVE_DEST->COPY_SOURCE
            pCommandList->ResourceBarrier((UINT)m_barrierResolveSrcToUav.size(), m_barrierResolveSrcToUav.data());
            m_barrierResolveSrcToUav.clear();

            m_gpuTimerResolve.EndTimer(pCommandList, m_sharedFrameIndex);
#if RESOLVE_TO_TEXTURE
            // copy readable feedback buffers to cpu
            for (auto& t : m_feedbackReadbacks)
            {
                t.m_pStreamingResource->ReadbackFeedback(pCommandList);
            }
#endif
            m_feedbackReadbacks.clear();

            m_gpuTimerResolve.ResolveTimer(pCommandList, m_sharedFrameIndex);
        }

        pCommandList->Close();
    }

    TileUpdateManager::CommandLists outputCommandLists;
    outputCommandLists.m_beforeDrawCommands = m_commandLists[(UINT)CommandListName::Before].m_commandList.Get();
    outputCommandLists.m_afterDrawCommands = m_commandLists[(UINT)CommandListName::After].m_commandList.Get();

    m_withinFrame = false;

    return outputCommandLists;
}
