#include "Sprite.h"

Sprite::Sprite(ID3D11ShaderResourceView* texture)
    : m_texture(texture)
{
}

void Sprite::SetTexture(ID3D11ShaderResourceView* texture)
{
    m_texture = texture;
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