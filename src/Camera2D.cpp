#include "Camera2D.h"

using namespace DirectX;

void Camera2D::SetViewportSize(float width, float height)
{
    m_width = width;
    m_height = height;
}

void Camera2D::SetPosition(float x, float y)
{
    m_x = x;
    m_y = y;
}

void Camera2D::Move(float dx, float dy)
{
    m_x += dx;
    m_y += dy;
}

void Camera2D::SetZoom(float zoom)
{
    if (zoom <= 0.0f)
        zoom = 0.01f;

    m_zoom = zoom;
}

float Camera2D::GetZoom() const
{
    return m_zoom;
}

DirectX::XMMATRIX Camera2D::GetViewMatrix() const
{
    /*
        2D Camera 的 View Matrix 可以理解为：
        Camera 往右移动，世界看起来往左移动。

        所以这里使用 -m_x, -m_y。
    */
    return XMMatrixTranslation(-m_x, -m_y, 0.0f);
}

DirectX::XMMATRIX Camera2D::GetProjectionMatrix() const
{
    /*
        使用 OffCenterLH 的好处是：
        左上角是 (0, 0)
        右下角是 (width, height)

        这比 XMMatrixOrthographicLH 更适合 2D 游戏。
    */
    float viewWidth = m_width / m_zoom;
    float viewHeight = m_height / m_zoom;

    return XMMatrixOrthographicOffCenterLH(
        0.0f,
        viewWidth,
        viewHeight,
        0.0f,
        m_nearZ,
        m_farZ
    );
}

float Camera2D::GetWidth() const
{
    return m_width;
}

float Camera2D::GetHeight() const
{
    return m_height;
}