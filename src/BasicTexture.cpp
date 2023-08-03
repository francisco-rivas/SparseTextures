//#include "stdafx.h"
#include "pch.h"
#include "BasicTexture.h"
#include "DebugHelper.h"
#include "d3dx12.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image\stb_image.h"
#include "stb_image\stb_image_write.h"

//Texture::Texture(char* fileName, ComPtr<ID3D12Device> device, ComPtr<ID3D12GraphicsCommandList5> cmdList, ComPtr<ID3D12DescriptorHeap> heap, int descriptor, int descriptorSize)
//Texture::Texture(char* fileName, ComPtr<ID3D12Device> device, ComPtr<ID3D12GraphicsCommandList5> cmdList, CD3DX12_CPU_DESCRIPTOR_HANDLE in_srvDescHandle)
Texture::Texture(char* fileName, ID3D12Device* device, ComPtr<ID3D12GraphicsCommandList1> cmdList, CD3DX12_CPU_DESCRIPTOR_HANDLE in_srvDescHandle)
{
	//m_descriptor = descriptor;
	m_textureData = stbi_load(fileName, &m_width, &m_height, &m_components, 4);

	// Create the texture.
	{
		// Describe and create a Texture2D.
		D3D12_RESOURCE_DESC textureDesc = {};
		textureDesc.MipLevels = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.Width = m_width;
		textureDesc.Height = m_height;
		textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		textureDesc.DepthOrArraySize = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

		CD3DX12_HEAP_PROPERTIES heapDefault(D3D12_HEAP_TYPE_DEFAULT);

		ThrowIfFailed(device->CreateCommittedResource(
			//&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			&heapDefault,
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&m_texture)));

		const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_texture.Get(), 0, 1);

		CD3DX12_HEAP_PROPERTIES heapUpload(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC descBuffer = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);

		// Create the GPU upload buffer.
		ThrowIfFailed(device->CreateCommittedResource(
			//&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			&heapUpload,
			D3D12_HEAP_FLAG_NONE,
			//&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
			&descBuffer,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&textureUploadHeap)));

		// Copy data to the intermediate upload heap and then schedule a copy 
		// from the upload heap to the Texture2D.
		std::vector<UINT8> texture = Data();

		D3D12_SUBRESOURCE_DATA textureData = {};
		textureData.pData = &texture[0];
		textureData.RowPitch = m_width * 4;
		textureData.SlicePitch = textureData.RowPitch * m_height;

		UpdateSubresources(cmdList.Get(), m_texture.Get(), textureUploadHeap.Get(), 0, 0, 1, &textureData);
		CD3DX12_RESOURCE_BARRIER barrierTransition = CD3DX12_RESOURCE_BARRIER::Transition(m_texture.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		//cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_texture.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
		cmdList->ResourceBarrier(1, &barrierTransition);

		// Describe and create a SRV for the texture.
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
		//CD3DX12_CPU_DESCRIPTOR_HANDLE cpu(heap->GetCPUDescriptorHandleForHeapStart());
		//device->CreateShaderResourceView(m_texture.Get(), &srvDesc, cpu.Offset(m_descriptor, descriptorSize));
		device->CreateShaderResourceView(m_texture.Get(), &srvDesc, in_srvDescHandle);
	}
}

std::vector<UINT8> Texture::Data()
{
	std::vector<uint8_t> textureData(&m_textureData[0], &m_textureData[m_width * m_height * m_components]);

	return textureData;
}