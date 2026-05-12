#include "Texture.h"

Texture::~Texture()
{
    Shutdown();
}

bool Texture::LoadFromFile(
    ID3D11Device* device,
    ID3D11DeviceContext* context,
    const std::wstring& filename)
{
    if (!device)
        return false;

    m_texture.Reset();
    m_textureView.Reset();

    HRESULT hr = DirectX::CreateWICTextureFromFile(
        device,
        context,
        filename.c_str(),
        m_texture.GetAddressOf(),
        m_textureView.GetAddressOf()
    );

    return SUCCEEDED(hr);
}

void Texture::Shutdown()
{
    m_texture.Reset();
    m_textureView.Reset();
}

ID3D11ShaderResourceView* Texture::GetShaderResourceView() const
{
    return m_textureView.Get();
}

ID3D11Resource* Texture::GetResource() const
{
    return m_texture.Get();
}