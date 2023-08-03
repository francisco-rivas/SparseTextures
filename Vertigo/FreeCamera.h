#pragma once

#include "Camera.h"

class FreeCamera : public CameraBase
{
public:
    virtual void Reset(DirectX::XMFLOAT3 position) override;
    virtual void Reset(float radius, float phi, float theta) override;

    virtual void Update(Mouse::ButtonStateTracker tracker) override;
    virtual void Update(float zoomFactor, float deltaPhi, float deltaTheta) override;

    virtual void SetProjection(float fovAngleY, float aspectRatio, float nearZ, float farZ) override;

    virtual float Pitch()
    {
        return m_pitch;
    }

    virtual float Yaw()
    {
        return m_yaw;
    }
private:
    DirectX::XMFLOAT4X4 m_projectionMatrix{};

    DirectX::XMVECTOR camTarget = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    DirectX::XMVECTOR DefaultForward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    DirectX::XMVECTOR DefaultRight = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    DirectX::XMVECTOR camForward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    DirectX::XMVECTOR camRight = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    DirectX::XMVECTOR camUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    float m_radius{};
    float m_yaw{};
    float m_pitch{};
    float m_roll{};

    float m_translateX = 0.0f;
    float m_translateY = 0.0f;
    float m_translateZ = 0.0f;

    float m_speed = 0.25f;

    DirectX::XMINT2 m_lastCursorPos{};
    void TrackLastCursorPosition(Mouse::State const & state) noexcept;

    void Calculate();

    void RotateLeft(float deltaTheta) noexcept;
    void RotateUp(float deltaPhi) noexcept;
};

namespace Camera
{
    extern FreeCamera g_freeCamera;
};