#include "Sprite.h"

Sprite::Sprite(Texture* texture)
{
    SetTexture(texture);
}

void Sprite::SetTexture(Texture* texture)
{
    m_texture = texture->GetShaderResourceView();
    m_width = texture->GetWidth();
    m_height = texture->GetHeight();
}

ID3D11ShaderResourceView* Sprite::GetTexture() const
{
    return m_texture.Get();
}

void Sprite::SetColor(const DirectX::XMFLOAT4& color)
{
    m_color = color;
}

const DirectX::XMFLOAT4& Sprite::GetColor() const
{
    return m_color;
}

const UINT Sprite::GetWidth() const
{
    return m_width;
}

const UINT Sprite::GetHeight() const
{
    return m_height;
}