#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include "Texture.h"

class Sprite
{
public:
    Sprite() = default;
    explicit Sprite(Texture* texture);

    void SetTexture(Texture* texture);
    ID3D11ShaderResourceView* GetTexture() const;

    void SetColor(const DirectX::XMFLOAT4& color);
    const DirectX::XMFLOAT4& GetColor() const;
    const UINT GetWidth() const;
    const UINT GetHeight() const;

private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

    DirectX::XMFLOAT4 m_color = {
        1.0f, 1.0f, 1.0f, 1.0f
    };

    UINT m_width = 0;
    UINT m_height = 0;
};