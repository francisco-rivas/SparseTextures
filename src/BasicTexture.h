#pragma once
//#include "DXSample.h"
//#include <d3d12.h>

using namespace DirectX;
using Microsoft::WRL::ComPtr;

class Texture
{
public:
	Texture() : m_height(0), m_width(0), m_components(0), m_textureData(nullptr) {}
	//Texture(char* filename, ComPtr<ID3D12Device> device, ComPtr<ID3D12GraphicsCommandList5> cmdList, ComPtr<ID3D12DescriptorHeap> heap, int descriptor, int descriptorSize);
	//Texture(char* fileName, ComPtr<ID3D12Device> device, ComPtr<ID3D12GraphicsCommandList5> cmdList, CD3DX12_CPU_DESCRIPTOR_HANDLE in_srvDescHandle);
	Texture(char* fileName, ID3D12Device* device, ComPtr<ID3D12GraphicsCommandList1> cmdList, CD3DX12_CPU_DESCRIPTOR_HANDLE in_srvDescHandle);

	~Texture()
	{
		if (m_textureData != nullptr)
			delete m_textureData;
	}

	//void LoadFromFile(char* fileName);

	int Width() { return m_height; }
	int Height() { return m_height; }
	int ColorComponents() { return m_components; }
	std::vector<UINT8> Data();
private:

	int m_width;
	int m_height;
	int m_components;

	UINT8* m_textureData = nullptr;
	ComPtr<ID3D12Resource> m_texture;

	// Note: ComPtr's are CPU objects but this resource needs to stay in scope until
	// the command list that references it has finished executing on the GPU.
	// We will flush the GPU at the end of this method to ensure the resource is not
	// prematurely destroyed.
	ComPtr<ID3D12Resource> textureUploadHeap;

};
