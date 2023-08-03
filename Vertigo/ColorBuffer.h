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
#pragma once

#ifndef _COLOR_BUFFER_H_
#define _COLOR_BUFFER_H_

using namespace Microsoft::WRL;

class ColorBuffer
{
public:
    ColorBuffer(ID3D12Device* device, DXGI_FORMAT format, uint32_t width, uint32_t height, uint32_t mipLevels = 1);
    
    void CreateSRV(ID3D12Device* device, ID3D12DescriptorHeap* heap, uint32_t offset);
    void CreateUAV(ID3D12Device* device, ID3D12DescriptorHeap* heap, uint32_t offset);

    void Release();

    CD3DX12_CPU_DESCRIPTOR_HANDLE GetSRVHandle() { return m_srvHandle; }
    CD3DX12_CPU_DESCRIPTOR_HANDLE GetUAVHandle() { return m_uavHandle; }

    ID3D12Resource* GetCommittedResource() { return m_buffer; }

private:
    ColorBuffer() {};

    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_mipLevels;
    DXGI_FORMAT m_format;
    D3D12_RESOURCE_DESC m_desc;
    CD3DX12_CPU_DESCRIPTOR_HANDLE m_srvHandle;
    CD3DX12_CPU_DESCRIPTOR_HANDLE m_uavHandle;

    ID3D12Resource* m_buffer = nullptr;
};

#endif