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

    LoadTextureSize();

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

// 不能直接从 CreateWICTextureFromFile() 获取图片大小
// 它会帮你创建 ID3D11Resource 和 ID3D11ShaderResourceView
// 图片尺寸可以从创建出来的 ID3D11Resource 里查询。
void Texture::LoadTextureSize()
{
    if (!m_texture) return;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;

    HRESULT hr = m_texture.As(&texture2D);

    if (FAILED(hr) || !texture2D) return;

    D3D11_TEXTURE2D_DESC desc{};
    texture2D->GetDesc(&desc);

    m_width = desc.Width;
    m_height = desc.Height;
}

UINT Texture::GetWidth() const
{
    return m_width;
}

UINT Texture::GetHeight() const
{
    return m_height;
}