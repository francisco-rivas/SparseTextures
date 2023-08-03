#pragma once

#include <DirectXMath.h>
#include "Platform/Mouse.h"

class CameraBase
{
public:
    virtual void Reset(DirectX::XMFLOAT3 position) = 0;
    virtual void Reset(float radius, float phi, float theta) = 0;

    virtual void Update(Mouse::ButtonStateTracker tracker) = 0;
    virtual void Update(float zoomFactor, float deltaPhi, float deltaTheta) = 0;

    virtual void SetProjection(float fovAngleY, float aspectRatio, float nearZ, float farZ) = 0;

    DirectX::XMFLOAT4X4 ViewMatrix{};
    DirectX::XMFLOAT4X4 ViewProjectionMatrix{};
    DirectX::XMVECTOR Position{};

    inline float PositionX()
    {
        return Position.m128_f32[0];
    }

    inline float PositionY()
    {
        return Position.m128_f32[1];
    }

    inline float PositionZ()
    {
        return Position.m128_f32[2];
    }

    virtual float Pitch() = 0;
    virtual float Yaw() = 0;
};

namespace Camera
{
    extern CameraBase* g_camera;
};