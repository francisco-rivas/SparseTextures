#include "stdafx.h"
#include "ImGuiRenderer.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx12.h>
#include "EngineOptions.h"
#include "Camera.h"
#include "FreeCamera.h"
#include "OrbitCamera.h"

void ImGuiRenderer::Initialize(ID3D12Device* device, ID3D12DescriptorHeap* heap, HWND hwnd)
{
    m_device = device;  
    m_srvHeap = heap;

#ifdef IMGUI_ENABLED
    CD3DX12_CPU_DESCRIPTOR_HANDLE cpu(m_srvHeap->GetCPUDescriptorHandleForHeapStart());
    CD3DX12_GPU_DESCRIPTOR_HANDLE gpu(m_srvHeap->GetGPUDescriptorHandleForHeapStart());

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos + ImGuiBackendFlags_HasSetMousePos;

    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    //ImGui::StyleColorsLight();

    const UINT cbvSrvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    // Setup Platform/Renderer bindings
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX12_Init(device, 1, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
        cpu.Offset(0, cbvSrvDescriptorSize),
        gpu.Offset(0, cbvSrvDescriptorSize));

    m_window = hwnd;
#endif //IMGUI_ENABLED
}

void ImGuiRenderer::Render(ID3D12GraphicsCommandList5 * command, EngineOptions & config, Intel::VALAR_DESCRIPTOR & desc)
{
#ifdef IMGUI_ENABLED
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    RECT rect;
    GetWindowRect(m_window, &rect);
    ImVec2 v(0, 0);
    ImVec2 s(500, (float)rect.bottom);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);

    ImGui::Begin("Vertigo", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowPos(v);
    ImGui::SetWindowSize(s);

    ImGui::Text("Camera Pos: %2.2f %2.2f %2.2f", Camera::g_camera->PositionX(), Camera::g_camera->PositionY(), Camera::g_camera->PositionZ());
    ImGui::Text("Camera Yaw/Pitch: %2.2f %2.2f", Camera::g_camera->Yaw(), Camera::g_camera->Pitch());
    static bool orbitCamera = false;
    const char * camereTypes[] = { "Free Camera", "Orbit Camera" };
    static int selectedCamera = 0;
    ImGui::Combo("Camera Type", &selectedCamera, camereTypes, IM_ARRAYSIZE(camereTypes), 2);
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    static int imageOffset = 1;
    int incrementSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    CD3DX12_GPU_DESCRIPTOR_HANDLE gpu(m_srvHeap->GetGPUDescriptorHandleForHeapStart());
    ImGui::Image((ImTextureID)gpu.Offset(imageOffset, incrementSize).ptr, ImVec2((float)1920 / 6, (float)1080 / 6));

    if (ImGui::Button("Prev")) {
        imageOffset--;

        if (imageOffset < 0)
            imageOffset = 0;
    }
    ImGui::SameLine();
    if (ImGui::Button("Next")) {
        imageOffset++;
    }
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Checkbox("Enable Materials", &config.EnableMaterials);
    ImGui::Checkbox("Enable IBL", &config.EnableIBL);
    ImGui::Checkbox("Show Ground Plane", &config.EnableGround);
    ImGui::Checkbox("Auto Rotate", &config.AutoRotate);
    ImGui::Checkbox("Wireframe", &config.WireFrame);
    char buf[10];
    sprintf_s(buf, "MSAA %dx", config.MSAASamples);
    ImGui::Checkbox(buf, &config.EnableMSAA);
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("VALAR Support: %s", desc.m_hwFeatures.m_isVALARSupported ? "True" : "False");
    ImGui::Text("VRS Tier 1: %s", desc.m_hwFeatures.m_vrsTier1Support ? "True" : "False");
    ImGui::Text("VRS Tier 2: %s", desc.m_hwFeatures.m_vrsTier2Support ? "True" : "False");
    ImGui::Text("Tile Size: %d", desc.m_hwFeatures.m_shadingRateTileSize);
    ImGui::Checkbox("Enable VALAR", &desc.m_enabled);

    if (desc.m_enabled)
    {
        ImGui::SliderFloat("Sensitivity Threshold", &desc.m_sensitivityThreshold, 0, 1);
        ImGui::SliderFloat("Quarter Rate Modifier", &desc.m_quarterRateShadingModifier, 0, 20);
        ImGui::SliderFloat("Environment Luminance", &desc.m_environmentLuminance, 0, 20);
        ImGui::SliderFloat("Weber-Fechner Constant", &desc.m_weberFechnerConstant, 0, 20);
        ImGui::Checkbox("Weber-Fechner Mode", &desc.m_weberFechnerMode);
        ImGui::Checkbox("Allow Quarter Rate", &desc.m_allowQuarterRateShading);
        ImGui::Checkbox("Debug Overlay", &desc.m_debugOverlay);
    }

    ImGui::End();

    ImGui::PopStyleVar();

    ImGui::Render();
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), command);

    if (selectedCamera == 1) {
        Camera::g_camera = &Camera::g_orbitCamera;
    } else {
        Camera::g_camera = &Camera::g_freeCamera;
    }
#endif

}