#include "stdafx.h"

#include "FreeCamera.h"

using namespace DirectX;

void FreeCamera::Reset(DirectX::XMFLOAT3 position)
{
    constexpr DirectX::XMVECTOR zAxis = { { 0, 0, 1, 0 } };
    constexpr DirectX::XMVECTOR yAxis = { { 0, 1, 0, 0 } };

    position.y = -position.y;
    const DirectX::XMVECTOR original = DirectX::XMLoadFloat3(&position);

    position.y = 0;
    const DirectX::XMVECTOR projected = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&position));

    const int xFlip = position.x < 0 ? -1 : 1;

    Reset(
        DirectX::XMVectorGetX(DirectX::XMVector3Length(original)),
        DirectX::XMVectorGetX(DirectX::XMVector3AngleBetweenNormals(DirectX::XMVector3Normalize(original), yAxis)),
        xFlip * DirectX::XMVectorGetX(DirectX::XMVector3AngleBetweenNormals(projected, zAxis)));
}

void FreeCamera::Reset(float radius, float phi, float theta)
{
    m_radius = 1;
    m_pitch = 0;
    m_yaw = 0;

    Update(radius, phi, theta);
}

void FreeCamera::Update(float zoomFactor, float deltaPhi, float deltaTheta)
{
    Calculate();
}

void FreeCamera::Update(Mouse::ButtonStateTracker tracker)
{
    using ButtonState = Mouse::ButtonStateTracker::ButtonState;

    bool changed = false;

    if (GetKeyState('A') & 0x8000)
    {
        m_translateX -= m_speed;
        changed = true;
    }

    if (GetKeyState('D') & 0x8000)
    {
        m_translateX += m_speed;
        changed = true;
    }

    if (GetKeyState('S') & 0x8000)
    {
        m_translateZ -= m_speed;
        changed = true;
    }

    if (GetKeyState('W') & 0x8000)
    {
        m_translateZ += m_speed;
        changed = true;
    }

    if (GetKeyState('Q') & 0x8000)
    {
        m_translateY += m_speed;
        changed = true;
    }

    if (GetKeyState('Z') & 0x8000)
    {
        m_translateY -= m_speed;
        changed = true;
    }

    if (GetKeyState('E') & 0x8000)
    {
        m_roll += m_speed;
        changed = true;
    }

    if (GetKeyState('C') & 0x8000)
    {
        m_roll -= m_speed;
        changed = true;
    }

    const Mouse::State state = tracker.GetLastState();
    if (tracker.rightButton == ButtonState::PRESSED)
    {
        TrackLastCursorPosition(state);
    }
    else if (tracker.rightButton == ButtonState::HELD)
    {
        RotateLeft((DirectX::XM_2PI * (state.x - m_lastCursorPos.x) / 540.0f) * m_speed);
        RotateUp((DirectX::XM_2PI * (state.y - m_lastCursorPos.y) / 540.0f) * m_speed);

        changed = true;
        TrackLastCursorPosition(state);
    }

    if (changed)
    {
        Calculate();
    }
}

void FreeCamera::SetProjection(float fovAngleY, float aspectRatio, float nearZ, float farZ)
{
    const DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);
    const DirectX::XMMATRIX viewProj = DirectX::XMLoadFloat4x4(&ViewMatrix) * projection;

    DirectX::XMStoreFloat4x4(&m_projectionMatrix, projection);
    DirectX::XMStoreFloat4x4(&ViewProjectionMatrix, viewProj);
}

void FreeCamera::Calculate()
{
    auto camRotationMatrix = XMMatrixRotationRollPitchYaw(-m_pitch, m_yaw, m_roll);
    camTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
    camTarget = XMVector3Normalize(camTarget);

    XMMATRIX RotateYTempMatrix;
    RotateYTempMatrix = XMMatrixRotationY(m_yaw);

    camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
    camUp = XMVector3TransformCoord(camUp, RotateYTempMatrix);
    camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);

    Position += m_translateX * camRight;
    Position += m_translateZ * camForward;
    Position += m_translateY * camUp;

    m_translateX = 0.0f;
    m_translateY = 0.0f;
    m_translateZ = 0.0f;

    camTarget = Position + camTarget;

    DirectX::XMStoreFloat4x4(&ViewMatrix, DirectX::XMMatrixLookAtLH(Position, camTarget, camUp) );
    DirectX::XMStoreFloat4x4(&ViewProjectionMatrix, DirectX::XMLoadFloat4x4(&ViewMatrix) * DirectX::XMLoadFloat4x4(&m_projectionMatrix));
}

void FreeCamera::TrackLastCursorPosition(Mouse::State const & state) noexcept
{
    m_lastCursorPos.x = state.x;
    m_lastCursorPos.y = state.y;
}

void FreeCamera::RotateLeft(float deltaTheta) noexcept
{
    m_yaw += deltaTheta;
}

void FreeCamera::RotateUp(float deltaPhi) noexcept
{
    constexpr float Epsilon = 0.000001f;

    m_pitch -= deltaPhi;
    m_pitch = std::clamp(m_pitch, -DirectX::XM_PIDIV2 + Epsilon, DirectX::XM_PIDIV2 - Epsilon);
}
