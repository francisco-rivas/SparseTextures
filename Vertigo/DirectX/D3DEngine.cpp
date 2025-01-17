// ------------------------------------------------------------
// Copyright(c) 2018-2022 Jesse Yurkovich
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// See the LICENSE file in the repo root for full license information.
// ------------------------------------------------------------
#include "stdafx.h"

#include "D3DEngine.h"
#include "D3DGraph.h"
#include "D3DMeshInstance.h"
#include "Engine.h"
#include "EngineOptions.h"
#include "ImageData.h"
#include "D3DMSAARenderContext.h"

using Microsoft::WRL::ComPtr;

D3DEngine::D3DEngine(EngineOptions const & config)
    : Engine(config)
{
    //m_camera = new D3DOrbitCamera();
    Camera::g_camera = &Camera::g_freeCamera;
    m_imGuiRenderer = new ImGuiRenderer();
}

D3DEngine::~D3DEngine()
{
    if (m_deviceResources != nullptr)
    {
        m_deviceResources->WaitForGpu();
    }
}

void D3DEngine::InitializeCore(HWND window)
{
    if (Config.ModelPath.rfind(".glb") != std::string::npos)
    {
        m_gltfScene = fx::gltf::LoadFromBinary(Config.ModelPath);
    }
    else
    {
        m_gltfScene = fx::gltf::LoadFromText(Config.ModelPath);
    }

    m_gltfGround = fx::gltf::LoadFromText("Assets/ground_plane.gltf");

    const uint32_t BackBufferCount = 3;
    m_deviceResources = std::make_unique<D3DDeviceResources>(BackBufferCount, D3D_FEATURE_LEVEL_11_1, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB);
    m_deviceResources->SetWindow(window, Config.Width, Config.Height);
    m_deviceResources->RegisterDeviceNotify(this);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_imGuiRenderer->Initialize(m_deviceResources->GetD3DDevice(), m_cbvHeap.Get(), window);

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    m_height = Config.Height;
    m_width = Config.Width;
}

void D3DEngine::Update(float elapsedTime) noexcept
{
    if (Config.AutoRotate)
    {
        m_curRotationAngleRad += elapsedTime / 3.0f;
        if (m_curRotationAngleRad >= DirectX::XM_2PI)
        {
            m_curRotationAngleRad -= DirectX::XM_2PI;
        }
    }

    Camera::g_camera->Update(MouseTracker);
}

void D3DEngine::Render()
{
    // Prepare the command list to render a new frame.
    PrepareRender();

    D3DFrameResource const & currentFrame = m_deviceResources->GetCurrentFrameResource();

    const DirectX::XMMATRIX viewProj = DirectX::XMLoadFloat4x4(&Camera::g_camera->ViewProjectionMatrix);

    SceneConstantBuffer sceneParameters{};
    sceneParameters.ViewProj = DirectX::XMMatrixTranspose(viewProj);
    sceneParameters.Camera = Camera::g_camera->Position;
    sceneParameters.DirectionalLight = m_directionalLight;
    sceneParameters.PointLights[0] = m_pointLights[0];
    sceneParameters.PointLights[1] = m_pointLights[1];
    sceneParameters.EnableIBL = Config.EnableIBL;
    currentFrame.SceneCB->CopyData(0, sceneParameters);

    // Set the root signature and pipeline state for the command list
    ID3D12DescriptorHeap * descriptorHeaps[] = { m_cbvHeap.Get() };

    ID3D12GraphicsCommandList5 * commandList = m_deviceResources->GetCommandList();
    commandList->SetDescriptorHeaps(1, descriptorHeaps);
    commandList->SetGraphicsRootSignature(m_rootSignature.Get());
    commandList->SetGraphicsRootConstantBufferView(0, currentFrame.SceneCB->GetGPUVirtualAddress(0));
    commandList->SetGraphicsRootShaderResourceView(2, currentFrame.MeshDataBuffer->GetGPUVirtualAddress(0));
    commandList->SetGraphicsRootDescriptorTable(3, m_cbvHeap->GetGPUDescriptorHandleForHeapStart());
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    D3D12_SHADING_RATE_COMBINER combiners[2] = { D3D12_SHADING_RATE_COMBINER_PASSTHROUGH, D3D12_SHADING_RATE_COMBINER_OVERRIDE };
   
    Intel::VALAR_RETURN_CODE retCode = {};
    Intel::VALAR_DESCRIPTOR& valarDesc = m_deviceResources->GetVALARDescriptor();
    valarDesc.m_colorBuffer = m_deviceResources->m_valarColorBuffer->GetCommittedResource();
    valarDesc.m_valarBuffer = m_deviceResources->GetVALARBuffer();
    valarDesc.m_velocityBuffer = m_deviceResources->GetMSAARenderTarget(); // MWD TODO: temporary value need to fix
    valarDesc.m_uavHeap = m_deviceResources->m_valarDescriptorHeap.Get();
    valarDesc.m_commandList = commandList;
    valarDesc.m_bufferHeight = m_height;
    valarDesc.m_bufferWidth = m_width;

    retCode = Intel::VALAR_SetScreenSpaceCombiners(valarDesc);
    assert(retCode == Intel::VALAR_RETURN_CODE_SUCCESS);

    retCode = Intel::VALAR_ApplyMask(valarDesc);
    assert(retCode == Intel::VALAR_RETURN_CODE_SUCCESS);

    auto backBuffer = currentFrame.RenderTarget.Get();
   
    if (Config.EnableMSAA && m_deviceResources->IsMSAASupportedOnDevice())
    {
        D3DMSAARenderContext msaaRenderContext{
            commandList,
            currentFrame,
            viewProj,
            0,
            ShaderOptions::None,
            Config.EnableMaterials ? ShaderOptions::None : ShaderOptions::USE_AUTO_COLOR,
            m_msaaPipelineStateMap
        };
        
        retCode = Intel::VALAR_SetHeroAssetCombiners(valarDesc);
        assert(retCode == Intel::VALAR_RETURN_CODE_SUCCESS);

        if (Config.EnableGround)
        {
            msaaRenderContext.OverrideShaderOptions = ShaderOptions::IS_GROUND;
            m_groundMesh.SetWorldMatrix(DirectX::XMMatrixScaling(-100, 1, 100), DirectX::XMFLOAT3(0, m_transformedBox.Min.y, 0), 0, 1);
            m_groundMesh.Render(msaaRenderContext);
        }

        retCode = Intel::VALAR_SetScreenSpaceCombiners(valarDesc);
        assert(retCode == Intel::VALAR_RETURN_CODE_SUCCESS);

        for (auto & meshInstance : m_meshInstances)
        {
            msaaRenderContext.OverrideShaderOptions = ShaderOptions::None;
            D3DMesh & mesh = m_meshes[meshInstance.MeshIndex];
            mesh.SetWorldMatrix(meshInstance.Transform, m_boundingBox.CenterTranslation, m_curRotationAngleRad, m_autoScaleFactor);
            mesh.Render(msaaRenderContext);
        }

        D3D12_RESOURCE_BARRIER barriers[2];

        barriers[0] = CD3DX12_RESOURCE_BARRIER::Transition(
            m_deviceResources->GetMSAARenderTarget(),
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_RESOLVE_SOURCE);
        barriers[1] = CD3DX12_RESOURCE_BARRIER::Transition(
            backBuffer,
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_RESOLVE_DEST);
        commandList->ResourceBarrier(2, barriers);

        commandList->ResolveSubresource(backBuffer, 0, m_deviceResources->GetMSAARenderTarget(), 0, m_deviceResources->GetBackBufferFormat());

        barriers[0] = CD3DX12_RESOURCE_BARRIER::Transition(backBuffer, 
            D3D12_RESOURCE_STATE_RESOLVE_DEST, 
            D3D12_RESOURCE_STATE_RENDER_TARGET);
        barriers[1] = CD3DX12_RESOURCE_BARRIER::Transition(m_deviceResources->GetMSAARenderTarget(), 
            D3D12_RESOURCE_STATE_RESOLVE_SOURCE, 
            D3D12_RESOURCE_STATE_RENDER_TARGET);
        commandList->ResourceBarrier(2, barriers);
    }
    else
    {
        D3DRenderContext renderContext{
            commandList,
            currentFrame,
            viewProj,
            0,
            ShaderOptions::USE_WIREFRAME_PSO,
            Config.EnableMaterials ? ShaderOptions::None : ShaderOptions::USE_AUTO_COLOR,
            m_pipelineStateMap,
            m_wireframePipelineStateMap
        };

        retCode = Intel::VALAR_SetHeroAssetCombiners(valarDesc);
        assert(retCode == Intel::VALAR_RETURN_CODE_SUCCESS);

        if (Config.EnableGround)
        {
            renderContext.OverrideShaderOptions = ShaderOptions::IS_GROUND;
            m_groundMesh.SetWorldMatrix(DirectX::XMMatrixScaling(-100, 1, 100), DirectX::XMFLOAT3(0, m_transformedBox.Min.y, 0), 0, 1);
            m_groundMesh.Render(renderContext, Config.WireFrame);
        }

        retCode = Intel::VALAR_SetScreenSpaceCombiners(valarDesc);
        assert(retCode == Intel::VALAR_RETURN_CODE_SUCCESS);

        for (auto & meshInstance : m_meshInstances)
        {
            renderContext.OverrideShaderOptions = ShaderOptions::None;
            D3DMesh & mesh = m_meshes[meshInstance.MeshIndex];
            mesh.SetWorldMatrix(meshInstance.Transform, m_boundingBox.CenterTranslation, m_curRotationAngleRad, m_autoScaleFactor);
            mesh.Render(renderContext, Config.WireFrame);
        }
    }

    D3D12_RESOURCE_BARRIER barriers[2];

    barriers[0] = CD3DX12_RESOURCE_BARRIER::Transition(
        backBuffer,
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_COPY_SOURCE);
    barriers[1] = CD3DX12_RESOURCE_BARRIER::Transition(
        m_deviceResources->m_valarColorBuffer->GetCommittedResource(),
        D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
        D3D12_RESOURCE_STATE_COPY_DEST);
    commandList->ResourceBarrier(2, barriers);

    commandList->CopyResource(m_deviceResources->m_valarColorBuffer->GetCommittedResource(), backBuffer);

    barriers[0] = CD3DX12_RESOURCE_BARRIER::Transition(
        backBuffer,
        D3D12_RESOURCE_STATE_COPY_SOURCE,
        D3D12_RESOURCE_STATE_RENDER_TARGET);
    barriers[1] = CD3DX12_RESOURCE_BARRIER::Transition(
        m_deviceResources->m_valarColorBuffer->GetCommittedResource(),
        D3D12_RESOURCE_STATE_COPY_DEST,
        D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
    commandList->ResourceBarrier(2, barriers);
    
    retCode = Intel::VALAR_ComputeMask(valarDesc);
    assert(retCode == Intel::VALAR_RETURN_CODE_SUCCESS);

    retCode = Intel::VALAR_DebugOverlay(valarDesc);
    assert(retCode == Intel::VALAR_RETURN_CODE_SUCCESS);

    barriers[0] = CD3DX12_RESOURCE_BARRIER::Transition(
        backBuffer,
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_COPY_DEST);
    barriers[1] = CD3DX12_RESOURCE_BARRIER::Transition(
        m_deviceResources->m_valarColorBuffer->GetCommittedResource(),
        D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
        D3D12_RESOURCE_STATE_COPY_SOURCE);
    commandList->ResourceBarrier(2, barriers);

    commandList->CopyResource(backBuffer, m_deviceResources->m_valarColorBuffer->GetCommittedResource());

    barriers[0] = CD3DX12_RESOURCE_BARRIER::Transition(
        backBuffer,
        D3D12_RESOURCE_STATE_COPY_DEST,
        D3D12_RESOURCE_STATE_RENDER_TARGET);
    barriers[1] = CD3DX12_RESOURCE_BARRIER::Transition(
        m_deviceResources->m_valarColorBuffer->GetCommittedResource(),
        D3D12_RESOURCE_STATE_COPY_SOURCE,
        D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
    commandList->ResourceBarrier(2, barriers);

    Config.MSAASamples = m_deviceResources->m_msaaSampleCount;

    retCode = Intel::VALAR_ResetMask(valarDesc);
    assert(retCode == Intel::VALAR_RETURN_CODE_SUCCESS);

    // need to reset after assigning heaps for VALAR
    PrepareRender(true);
    commandList->SetDescriptorHeaps(1, descriptorHeaps);
    commandList->SetGraphicsRootSignature(m_rootSignature.Get());
    commandList->SetGraphicsRootConstantBufferView(0, currentFrame.SceneCB->GetGPUVirtualAddress(0));
    commandList->SetGraphicsRootShaderResourceView(2, currentFrame.MeshDataBuffer->GetGPUVirtualAddress(0));
    commandList->SetGraphicsRootDescriptorTable(3, m_cbvHeap->GetGPUDescriptorHandleForHeapStart());
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_imGuiRenderer->Render(commandList, Config, valarDesc);

    CompleteRender();
}

void D3DEngine::ChangeWindowSizeCore(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
    {
        return;
    }

    CreateWindowSizeDependentResources();

    m_width = width;
    m_height = height;
}

void D3DEngine::CreateDeviceDependentResources()
{
    m_deviceResources->PrepareCommandList();

    BuildEnvironmentMaps();
    BuildScene();
    BuildRootSignature();
    BuildPipelineStateObjects();
    BuildUploadBuffers();
    BuildDescriptorHeaps();

    m_deviceResources->ExecuteCommandList();
    m_deviceResources->WaitForGpu();

    m_environment.FinishUpload();

    for (auto & texture : m_textures)
    {
        texture.FinishUpload();
    }

    for (auto & mesh : m_meshes)
    {
        mesh.FinishUpload();
    }

    // Initialize the camera system
    Camera::g_orbitCamera.Reset(DirectX::XMFLOAT3(Config.CameraX, Config.CameraY, Config.CameraZ));
    Camera::g_freeCamera.Reset(DirectX::XMFLOAT3(Config.CameraX, Config.CameraY, Config.CameraZ));

    // Initialize the lighting parameters
    m_directionalLight.Direction = DirectX::XMFLOAT3(-0.57f, 0.57f, 0.57f);
    m_directionalLight.Strength = DirectX::XMFLOAT3(0.8f, 0.63f, 0.37f);
    m_pointLights[0].Position = DirectX::XMFLOAT3(-6.0f, 6.0f, 6.0f);
    m_pointLights[0].Strength = DirectX::XMFLOAT3(0.8f, 0.8f, 0.8f);
    m_pointLights[0].FalloffStart = 0.0f;
    m_pointLights[0].FalloffEnd = 100.0f;
    m_pointLights[1].Position = DirectX::XMFLOAT3(6.0f, 6.0f, 6.0f);
    m_pointLights[1].Strength = DirectX::XMFLOAT3(0.8f, 0.8f, 0.8f);
    m_pointLights[1].FalloffStart = 0.0f;
    m_pointLights[1].FalloffEnd = 100.0f;
}

void D3DEngine::CreateWindowSizeDependentResources()
{
    // Initialize the projection matrix
    const auto size = m_deviceResources->GetOutputSize();
    Camera::g_orbitCamera.SetProjection(DirectX::XM_PIDIV4, static_cast<float>(size.right) / size.bottom, 0.01f, 400.0f);
    Camera::g_freeCamera.SetProjection(DirectX::XM_PIDIV4, static_cast<float>(size.right) / size.bottom, 0.01f, 400.0f);
}

void D3DEngine::PrepareRender(bool imGui)
{
    if (!imGui) m_deviceResources->Prepare();

    ID3D12GraphicsCommandList5 * commandList = m_deviceResources->GetCommandList();

    if (imGui)
    {
        // Clear the views.
        const auto rtvDescriptor = m_deviceResources->GetRenderTargetView();
        const auto dsvDescriptor = m_deviceResources->GetDepthStencilView();

        commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
        return;
    }
    
    if (Config.EnableMSAA && m_deviceResources->IsMSAASupportedOnDevice())
    {
        // Clear the views.
        const auto rtvDescriptor = m_deviceResources->GetMSAARenderTargetView();
        const auto dsvDescriptor = m_deviceResources->GetMSAADepthStencilView();

        commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);

        // Use linear clear color for gamma-correct rendering.
        const DirectX::XMVECTORF32 Background = { 0.0f, 0.2f, 0.4f, 1.0f };
        commandList->ClearRenderTargetView(rtvDescriptor, Background, 0, nullptr);

        commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

        // Set the viewport and scissor rect.
        const auto viewport = m_deviceResources->GetScreenViewport();
        const auto scissorRect = m_deviceResources->GetScissorRect();
        commandList->RSSetViewports(1, &viewport);
        commandList->RSSetScissorRects(1, &scissorRect);
    }
    else
    {
        // Clear the views.
        const auto rtvDescriptor = m_deviceResources->GetRenderTargetView();
        const auto dsvDescriptor = m_deviceResources->GetDepthStencilView();

        commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);

        // Use linear clear color for gamma-correct rendering.
        const DirectX::XMVECTORF32 Background = { 0.26f, 0.24f, 0.24f, 1.0f };
        commandList->ClearRenderTargetView(rtvDescriptor, Background, 0, nullptr);
        commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

        // Set the viewport and scissor rect.
        const auto viewport = m_deviceResources->GetScreenViewport();
        const auto scissorRect = m_deviceResources->GetScissorRect();
        commandList->RSSetViewports(1, &viewport);
        commandList->RSSetScissorRects(1, &scissorRect);
    }
}

void D3DEngine::CompleteRender()
{
    // Show the new frame.
    m_deviceResources->Present();
}

void D3DEngine::BuildEnvironmentMaps()
{
    Logger::WriteLine("Building environment maps...");
    m_environment.Create(m_deviceResources.get());
}

void D3DEngine::BuildScene()
{
    Logger::WriteLine("Building scene...");

    if (Config.EnableMaterials)
    {
        Logger::WriteLine("  Building textures...");
        m_textures.resize(m_gltfScene.textures.size());
        for (uint32_t i = 0; i < m_gltfScene.textures.size(); i++)
        {
            ImageData image(m_gltfScene, i, Config.ModelPath);
            m_textures[i].Create(image, m_deviceResources.get());
        }
    }

    Logger::WriteLine("  Building meshes...");
    m_meshes.resize(m_gltfScene.meshes.size());
    for (uint32_t i = 0; i < m_gltfScene.meshes.size(); i++)
    {
        m_meshes[i].Create(m_gltfScene, i, m_deviceResources.get());
    }

    m_groundMesh.Create(m_gltfGround, 0, m_deviceResources.get());

    if (!m_gltfScene.scenes.empty())
    {
        Logger::WriteLine("  Building scene graph...");
        std::vector<Graph::Node> graphNodes(m_gltfScene.nodes.size());
        const DirectX::XMMATRIX rootTransform = DirectX::XMMatrixMultiply(DirectX::XMMatrixIdentity(), DirectX::XMMatrixScaling(-1, 1, 1));
        for (const uint32_t sceneNode : m_gltfScene.scenes[0].nodes)
        {
            Graph::Visit(m_gltfScene, sceneNode, rootTransform, graphNodes);
        }

        Logger::WriteLine("  Traversing scene...");
        for (auto & graphNode : graphNodes)
        {
            if (graphNode.MeshIndex >= 0)
            {
                m_meshInstances.push_back({ graphNode.CurrentTransform, static_cast<uint32_t>(graphNode.MeshIndex) });
                Util::UnionBBox(m_boundingBox, Util::TransformBBox(m_meshes[graphNode.MeshIndex].MeshBBox(), graphNode.CurrentTransform));
            }
        }
    }
    else
    {
        Logger::WriteLine("  No scene graph data. Displaying individual meshes...");
        for (uint32_t i = 0; i < m_gltfScene.meshes.size(); i++)
        {
            m_meshInstances.push_back({ DirectX::XMMatrixIdentity(), i });
            Util::UnionBBox(m_boundingBox, m_meshes[i].MeshBBox());
        }
    }

    const DirectX::XMVECTOR sizeVec = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&m_boundingBox.Max), DirectX::XMLoadFloat3(&m_boundingBox.Min));
    DirectX::XMFLOAT3 size{};
    DirectX::XMStoreFloat3(&size, sizeVec);
    m_autoScaleFactor = 4.0f / std::max({ size.x, size.y, size.z });

    m_transformedBox = Util::TransformBBox(m_boundingBox, m_boundingBox.CenterTranslation, m_autoScaleFactor);

    Logger::WriteLine("    Found {0} mesh instance(s)", m_meshInstances.size());
    Logger::WriteLine("    Scene bbox       : [{0},{1},{2}] [{3},{4},{5}]", m_boundingBox.Min.x, m_boundingBox.Min.y, m_boundingBox.Min.z, m_boundingBox.Max.x, m_boundingBox.Max.y, m_boundingBox.Max.z);
    Logger::WriteLine("    Scene translation: [{0},{1},{2}]", m_boundingBox.CenterTranslation.x, m_boundingBox.CenterTranslation.y, m_boundingBox.CenterTranslation.z);
    Logger::WriteLine("    Auto-scale factor: {0:F4}", m_autoScaleFactor);
}

void D3DEngine::BuildRootSignature()
{
    CD3DX12_DESCRIPTOR_RANGE srvTable;
    srvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 128, 0);

    CD3DX12_ROOT_PARAMETER slotRootParameter[4]{};
    slotRootParameter[0].InitAsConstantBufferView(0);
    slotRootParameter[1].InitAsConstantBufferView(1);
    slotRootParameter[2].InitAsShaderResourceView(0, 1);
    slotRootParameter[3].InitAsDescriptorTable(1, &srvTable, D3D12_SHADER_VISIBILITY_PIXEL);

    const std::array<const CD3DX12_STATIC_SAMPLER_DESC, 2> staticSamplers{
        CD3DX12_STATIC_SAMPLER_DESC(
            0, // shaderRegister
            D3D12_FILTER_ANISOTROPIC, // filter
            D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressU
            D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressV
            D3D12_TEXTURE_ADDRESS_MODE_WRAP, // addressW
            0.0f, // mipLODBias
            8), // maxAnisotropy

        CD3DX12_STATIC_SAMPLER_DESC(
            1, // shaderRegister
            D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
            D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressU
            D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressV
            D3D12_TEXTURE_ADDRESS_MODE_CLAMP) // addressW
    };

    const CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(
        4,
        slotRootParameter,
        static_cast<UINT>(staticSamplers.size()),
        staticSamplers.data(),
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;
    const HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
    if (FAILED(hr))
    {
        if (error)
        {
            throw std::runtime_error(static_cast<const char *>(error->GetBufferPointer()));
        }

        throw COMUtil::com_exception(hr);
    }

    ID3D12Device * device = m_deviceResources->GetD3DDevice();
    COMUtil::ThrowIfFailed(device->CreateRootSignature(
        0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(m_rootSignature.ReleaseAndGetAddressOf())));
}

void D3DEngine::BuildDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
    cbvHeapDesc.NumDescriptors = 128;
    cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    cbvHeapDesc.NodeMask = 0;

    ID3D12Device * device = m_deviceResources->GetD3DDevice();
    COMUtil::ThrowIfFailed(device->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_cbvHeap)));

    const uint32_t size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(m_cbvHeap->GetCPUDescriptorHandleForHeapStart());

    // IBL textures get first few slots in the table...
    m_environment.CreateSRV(device, hDescriptor);

    // Textures get whatever is left...
    for (auto & texture : m_textures)
    {
        texture.CreateSRV(device, hDescriptor);
        hDescriptor.Offset(1, size);
    }
}

void D3DEngine::BuildPipelineStateObjects()
{
    std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, D3DMesh::SlotVertex, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, D3DMesh::SlotNormal, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, D3DMesh::SlotTangent, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, D3DMesh::SlotTexCoord0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    Logger::WriteLine("Compiling shaders...");
    auto standardVS = Util::CompileShader(L"DirectX\\Shaders\\Default.hlsl", "StandardVS", "vs_5_1", nullptr);

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { &inputLayout[0], static_cast<UINT>(inputLayout.size()) };
    psoDesc.pRootSignature = m_rootSignature.Get();
    psoDesc.VS = { standardVS->GetBufferPointer(), standardVS->GetBufferSize() };
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.BlendState.RenderTarget[0].BlendEnable = true;
    psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.DSVFormat = m_deviceResources->GetDepthBufferFormat();
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = m_deviceResources->GetBackBufferFormat();
    psoDesc.SampleDesc.Count = 1;

    // Compile shaders for all the required options of the meshes (including our default ones)...
    CompileShaderPerumutation("GroundPS", ShaderOptions::IS_GROUND, psoDesc);
    CompileShaderPerumutation("UberPS", ShaderOptions::USE_AUTO_COLOR, psoDesc);
    for (auto const & mesh : m_meshes) {
        std::vector<ShaderOptions> requiredOptions = mesh.GetRequiredShaderOptions();
        for (const ShaderOptions options : requiredOptions) {
            if (m_pipelineStateMap[options] == nullptr) {
                CompileShaderPerumutation("UberPS", options, psoDesc);
            }
        }
    }

    psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;

    CompileShaderPerumutation("GroundPS", ShaderOptions::IS_GROUND | ShaderOptions::USE_WIREFRAME_PSO, psoDesc, true);
    CompileShaderPerumutation("UberPS", ShaderOptions::USE_AUTO_COLOR | ShaderOptions::USE_WIREFRAME_PSO, psoDesc, true);
    for (auto const & mesh : m_meshes) {
        std::vector<ShaderOptions> requiredOptions = mesh.GetRequiredShaderOptions();
        for (const ShaderOptions options : requiredOptions) {
            if (m_wireframePipelineStateMap[options] == nullptr) {
                CompileShaderPerumutation("UberPS", options | ShaderOptions::USE_WIREFRAME_PSO, psoDesc, true);
            }
        }
    }

    D3D12_GRAPHICS_PIPELINE_STATE_DESC msaaPsoDesc = {};
    msaaPsoDesc.InputLayout = { &inputLayout[0], static_cast<UINT>(inputLayout.size()) };
    msaaPsoDesc.pRootSignature = m_rootSignature.Get();
    msaaPsoDesc.VS = { standardVS->GetBufferPointer(), standardVS->GetBufferSize() };
    msaaPsoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    msaaPsoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
    msaaPsoDesc.RasterizerState.MultisampleEnable = false;
    msaaPsoDesc.DepthStencilState.DepthEnable = false;
    msaaPsoDesc.DepthStencilState.StencilEnable = false;
    msaaPsoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    msaaPsoDesc.BlendState.RenderTarget[0].BlendEnable = true;
    msaaPsoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    msaaPsoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    msaaPsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    msaaPsoDesc.DSVFormat = m_deviceResources->GetDepthBufferFormat();
    msaaPsoDesc.SampleMask = UINT_MAX;
    msaaPsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    msaaPsoDesc.NumRenderTargets = 1;
    msaaPsoDesc.RTVFormats[0] = m_deviceResources->GetBackBufferFormat();
    msaaPsoDesc.SampleDesc.Count = m_deviceResources->m_msaaSampleCount;
    msaaPsoDesc.SampleDesc.Quality = 0U;

    CompileShaderPerumutation("GroundPS", ShaderOptions::IS_GROUND | ShaderOptions::IS_MSAA, msaaPsoDesc, false, true);
    CompileShaderPerumutation("UberPS", ShaderOptions::USE_AUTO_COLOR | ShaderOptions::IS_MSAA, msaaPsoDesc, false, true);
    for (auto const & mesh : m_meshes) {
        std::vector<ShaderOptions> requiredOptions = mesh.GetRequiredShaderOptions();
        for (const ShaderOptions options : requiredOptions) {
            if (m_msaaPipelineStateMap[options] == nullptr) {
                CompileShaderPerumutation("UberPS", options | ShaderOptions::IS_MSAA, msaaPsoDesc, false, true);
            }
        }
    }
}

void D3DEngine::CompileShaderPerumutation(std::string const & entryPoint, ShaderOptions options, D3D12_GRAPHICS_PIPELINE_STATE_DESC & psoDescTemplate, bool wireframe, bool msaa)
{
    // Keep rooted until compile completes since D3D_SHADER_MACRO is just a view over this data...
    std::vector<std::string> defines = GetShaderDefines(options | (Config.EnableIBL ? ShaderOptions::USE_IBL : ShaderOptions::None));

    std::vector<D3D_SHADER_MACRO> shaderDefines;
    for (auto const & define : defines) {
        shaderDefines.emplace_back(D3D_SHADER_MACRO{ define.c_str(), "1" });
    }

    shaderDefines.emplace_back(D3D_SHADER_MACRO{ nullptr, nullptr });

    auto permutedPS = Util::CompileShader(L"DirectX\\Shaders\\Default.hlsl", entryPoint, "ps_5_1", shaderDefines.data());
    psoDescTemplate.PS = { permutedPS->GetBufferPointer(), permutedPS->GetBufferSize() };

    Microsoft::WRL::ComPtr<ID3D12PipelineState> pso;
    ID3D12Device * device = m_deviceResources->GetD3DDevice();
    COMUtil::ThrowIfFailed(device->CreateGraphicsPipelineState(&psoDescTemplate, IID_PPV_ARGS(pso.ReleaseAndGetAddressOf())));

    if (wireframe) {
        m_wireframePipelineStateMap[options] = pso;
    } else if (msaa) {
        m_msaaPipelineStateMap[options] = pso;
    } else {
        m_pipelineStateMap[options] = pso;
    }
}

void D3DEngine::BuildUploadBuffers()
{
    std::size_t cbCount = 0;
    for (auto & meshInstance : m_meshInstances)
    {
        D3DMesh const & mesh = m_meshes[meshInstance.MeshIndex];
        cbCount += mesh.MeshPartCount();
    }

    m_deviceResources->CreateUploadBuffers(1, cbCount + 1); // +1 for ground plane
}

void D3DEngine::OnDeviceLost()
{
    m_rootSignature.Reset();

    for (auto & mesh : m_meshes)
    {
        mesh.Reset();
    }
}

void D3DEngine::OnDeviceRestored()
{
    CreateDeviceDependentResources();
    CreateWindowSizeDependentResources();
}
