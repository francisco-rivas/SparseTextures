#pragma once

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers.
#endif

#include <windows.h>
#include "VALAR.h"
using namespace DirectX;

using Microsoft::WRL::ComPtr;
struct EngineOptions;

class ImGuiRenderer
{
public:
    void Initialize(ID3D12Device* device, ID3D12DescriptorHeap* heap, HWND hwnd);
    void Render(ID3D12GraphicsCommandList5 * command, EngineOptions & config, Intel::VALAR_DESCRIPTOR & valar);

private:
	ID3D12Device* m_device;
	ID3D12DescriptorHeap* m_srvHeap;
    HWND m_window;
};
