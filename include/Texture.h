#pragma once

#include <d3d11.h>
#include <string>
#include <wrl/client.h>
#include <WICTextureLoader.h>

class Texture
{
public:
    Texture() = default;
    ~Texture();

    bool LoadFromFile(
        ID3D11Device* device,
        ID3D11DeviceContext* context,
        const std::wstring& filename);

    void Shutdown();

    ID3D11ShaderResourceView* GetShaderResourceView() const;
    ID3D11Resource* GetResource() const;
    UINT GetWidth() const;
    UINT GetHeight() const;
private:
    void LoadTextureSize();

private:
    Microsoft::WRL::ComPtr<ID3D11Resource> m_texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureView;

    UINT m_width = 0;
    UINT m_height = 0;
};