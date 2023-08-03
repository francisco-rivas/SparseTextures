// Copyright (C) 2022 Intel Corporation

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom
// the Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
// OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.

#include "stdafx.h"
#include "ColorBuffer.h"

ColorBuffer::ColorBuffer(ID3D12Device* device, DXGI_FORMAT format, uint32_t width, uint32_t height, uint32_t mipLevels)
{
    assert(device != nullptr);

    m_width = width;
    m_height = height;
    m_format = format;
    m_mipLevels = mipLevels;

    m_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    m_desc.Alignment = 0;
    m_desc.Width = m_width;
    m_desc.Height = m_height;
    m_desc.DepthOrArraySize = 1;
    m_desc.MipLevels = m_mipLevels;
    m_desc.Format = m_format;
    m_desc.SampleDesc.Count = 1;
    m_desc.SampleDesc.Quality = 0;
    m_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    m_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

    D3D12_HEAP_PROPERTIES heapProperties;
    heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
    heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProperties.CreationNodeMask = 0;
    heapProperties.VisibleNodeMask = 0;

    device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &m_desc, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, NULL, IID_PPV_ARGS(&m_buffer));
}

void ColorBuffer::CreateSRV(ID3D12Device* device, ID3D12DescriptorHeap* heap, uint32_t offset)
{
    assert(device != nullptr);
    assert(heap != nullptr);
    assert(m_buffer != nullptr);

    auto uavDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(heap->GetCPUDescriptorHandleForHeapStart(), offset, uavDescriptorSize);

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = m_format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2DArray.PlaneSlice = 0;
    srvDesc.Texture2DArray.FirstArraySlice = 0;
    srvDesc.Texture2DArray.MipLevels = m_mipLevels;
    srvDesc.Texture2DArray.ArraySize = 1;

    device->CreateShaderResourceView(m_buffer, &srvDesc, srvHandle);

    m_srvHandle = srvHandle;

}
void ColorBuffer::CreateUAV(ID3D12Device* device, ID3D12DescriptorHeap* heap, uint32_t offset)
{
    assert(device != nullptr);
    assert(heap != nullptr);
    assert(m_buffer != nullptr);

    auto uavDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    CD3DX12_CPU_DESCRIPTOR_HANDLE uavHandle(heap->GetCPUDescriptorHandleForHeapStart(), offset, uavDescriptorSize);

    D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
    uavDesc.Format = m_format;
    uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
    uavDesc.Texture2DArray.PlaneSlice = 0;
    uavDesc.Texture2DArray.FirstArraySlice = 0;
    uavDesc.Texture2DArray.MipSlice = 0;
    uavDesc.Texture2DArray.ArraySize = 1;

    device->CreateUnorderedAccessView(m_buffer, nullptr, &uavDesc, uavHandle);

    m_uavHandle = uavHandle;
}

void ColorBuffer::Release()
{
    if (m_buffer) {
        m_buffer->Release();
        m_buffer = nullptr;
    }
}