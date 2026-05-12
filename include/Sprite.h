#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

class Sprite
{
public:
    Sprite() = default;
    explicit Sprite(ID3D11ShaderResourceView* texture);

    void SetTexture(ID3D11ShaderResourceView* texture);
    ID3D11ShaderResourceView* GetTexture() const;

    void SetColor(const DirectX::XMFLOAT4& color);
    const DirectX::XMFLOAT4& GetColor() const;

private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

    DirectX::XMFLOAT4 m_color = {
        1.0f, 1.0f, 1.0f, 1.0f
    };
};