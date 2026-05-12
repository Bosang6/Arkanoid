#pragma once
#include <DirectXMath.h>

class Camera2D
{
public:
    Camera2D() = default;

    void SetViewportSize(float width, float height);

    void SetPosition(float x, float y);
    void Move(float dx, float dy);

    void SetZoom(float zoom);
    float GetZoom() const;

    DirectX::XMMATRIX GetViewMatrix() const;
    DirectX::XMMATRIX GetProjectionMatrix() const;

    float GetWidth() const;
    float GetHeight() const;

private:
    float m_width = 1280.0f;
    float m_height = 720.0f;

    float m_x = 0.0f;
    float m_y = 0.0f;

    float m_nearZ = 0.0f;
    float m_farZ = 1.0f;

    float m_zoom = 1.0f;
};