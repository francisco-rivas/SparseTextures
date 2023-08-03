// ------------------------------------------------------------
// Copyright(c) 2018-2022 Jesse Yurkovich
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// See the LICENSE file in the repo root for full license information.
// ------------------------------------------------------------
#pragma once

#include "Camera.h"

class OrbitCamera : public CameraBase
{
public:
    virtual void Reset(DirectX::XMFLOAT3 position) override;
    virtual void Reset(float radius, float phi, float theta) override;

    virtual void Update(Mouse::ButtonStateTracker tracker) override;
    virtual void Update(float zoomFactor, float deltaPhi, float deltaTheta) override;

    virtual void SetProjection(float fovAngleY, float aspectRatio, float nearZ, float farZ) override;

    virtual float Pitch()
    {
        return m_phi;
    }

    virtual float Yaw()
    {
        return m_theta;
    }

private:
    DirectX::XMFLOAT4X4 m_projectionMatrix{};

    float m_radius{};
    float m_phi{};
    float m_theta{};

    DirectX::XMINT2 m_lastCursorPos{};
    void TrackLastCursorPosition(Mouse::State const & state) noexcept;

    void Calculate();

    void Dolly(float zoomFactor) noexcept;
    void RotateLeft(float deltaTheta) noexcept;
    void RotateUp(float deltaPhi) noexcept;
};

namespace Camera
{
    extern OrbitCamera g_orbitCamera;
};