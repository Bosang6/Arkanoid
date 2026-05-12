#include "ResourceManager.h"

std::unordered_map<std::string, std::unique_ptr<Texture>>
ResourceManager::s_textures;

std::unordered_map<std::string, ResourceManager::VertexShaderPtr>
ResourceManager::s_vertexShaders;

std::unordered_map<std::string, ResourceManager::PixelShaderPtr>
ResourceManager::s_pixelShaders;

std::unordered_map<std::string, ResourceManager::BlobPtr>
ResourceManager::s_vertexShaderBlobs;

bool ResourceManager::LoadVertexShader(
    ID3D11Device* device,
    const std::wstring& file,
    const std::string& entryPoint,
    const std::string& name)
{
    if (s_vertexShaders.find(name) != s_vertexShaders.end())
        return true;

    BlobPtr shaderBlob;
    BlobPtr errorBlob;

    HRESULT hr = D3DCompileFromFile(
        file.c_str(),
        nullptr,
        nullptr,
        entryPoint.c_str(),
        "vs_5_0",
        D3D10_SHADER_ENABLE_STRICTNESS,
        0,
        shaderBlob.GetAddressOf(),
        errorBlob.GetAddressOf()
    );

    if (FAILED(hr))
    {
        if (errorBlob)
        {
            OutputDebugStringA(
                static_cast<const char*>(errorBlob->GetBufferPointer())
            );
        }

        return false;
    }

    VertexShaderPtr vertexShader;

    hr = device->CreateVertexShader(
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        nullptr,
        vertexShader.GetAddressOf()
    );

    if (FAILED(hr))
        return false;

    s_vertexShaders[name] = vertexShader;
    s_vertexShaderBlobs[name] = shaderBlob;

    return true;
}

bool ResourceManager::LoadPixelShader(
    ID3D11Device* device,
    const std::wstring& file,
    const std::string& entryPoint,
    const std::string& name)
{
    if (s_pixelShaders.find(name) != s_pixelShaders.end())
        return true;

    BlobPtr shaderBlob;
    BlobPtr errorBlob;

    HRESULT hr = D3DCompileFromFile(
        file.c_str(),
        nullptr,
        nullptr,
        entryPoint.c_str(),
        "ps_5_0",
        D3D10_SHADER_ENABLE_STRICTNESS,
        0,
        shaderBlob.GetAddressOf(),
        errorBlob.GetAddressOf()
    );

    if (FAILED(hr))
    {
        if (errorBlob)
        {
            OutputDebugStringA(
                static_cast<const char*>(errorBlob->GetBufferPointer())
            );
        }

        return false;
    }

    PixelShaderPtr pixelShader;

    hr = device->CreatePixelShader(
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(),
        nullptr,
        pixelShader.GetAddressOf()
    );

    if (FAILED(hr))
        return false;

    s_pixelShaders[name] = pixelShader;

    return true;
}

ID3D11VertexShader* ResourceManager::GetVertexShader(const std::string& name)
{
    auto it = s_vertexShaders.find(name);

    if (it == s_vertexShaders.end())
        return nullptr;

    return it->second.Get();
}

ID3D11PixelShader* ResourceManager::GetPixelShader(const std::string& name)
{
    auto it = s_pixelShaders.find(name);

    if (it == s_pixelShaders.end())
        return nullptr;

    return it->second.Get();
}

ID3DBlob* ResourceManager::GetVertexShaderBlob(const std::string& name)
{
    auto it = s_vertexShaderBlobs.find(name);

    if (it == s_vertexShaderBlobs.end())
        return nullptr;

    return it->second.Get();
}

bool ResourceManager::LoadTexture(
    ID3D11Device* device,
    ID3D11DeviceContext* context,
    const std::wstring& filename,
    const std::string& name)
{
    if (!device)
        return false;

    if (s_textures.find(name) != s_textures.end())
        return true;

    auto texture = std::make_unique<Texture>();

    if (!texture->LoadFromFile(device, context, filename))
        return false;

    s_textures[name] = std::move(texture);

    return true;
}

Texture* ResourceManager::GetTexture(const std::string& name)
{
    auto it = s_textures.find(name);

    if (it == s_textures.end())
        return nullptr;

    return it->second.get();
}

ID3D11ShaderResourceView* ResourceManager::GetTextureSRV(const std::string& name)
{
    Texture* texture = GetTexture(name);

    if (!texture)
        return nullptr;

    return texture->GetShaderResourceView();
}

void ResourceManager::Clear()
{
    s_vertexShaderBlobs.clear();
    s_pixelShaders.clear();
    s_vertexShaders.clear();
    s_textures.clear();
}