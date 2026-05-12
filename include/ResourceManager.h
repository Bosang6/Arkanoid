#pragma once
#include <string>
#include <unordered_map>
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <memory>
#include "Texture.h"

class ResourceManager
{
public:
    using VertexShaderPtr = Microsoft::WRL::ComPtr<ID3D11VertexShader>;
    using PixelShaderPtr = Microsoft::WRL::ComPtr<ID3D11PixelShader>;
    using BlobPtr = Microsoft::WRL::ComPtr<ID3DBlob>;
    using TexturePtr = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>;

public:
    static bool LoadVertexShader(
        ID3D11Device* device,
        const std::wstring& file,
        const std::string& entryPoint,
        const std::string& name);

    static bool LoadPixelShader(
        ID3D11Device* device,
        const std::wstring& file,
        const std::string& entryPoint,
        const std::string& name);

    static ID3D11VertexShader* GetVertexShader(const std::string& name);
    static ID3D11PixelShader* GetPixelShader(const std::string& name);

    static ID3DBlob* GetVertexShaderBlob(const std::string& name);

    static bool LoadTexture(
        ID3D11Device* device,
        ID3D11DeviceContext* context,
        const std::wstring& filename,
        const std::string& name);

    static Texture* GetTexture(const std::string& name);
    static ID3D11ShaderResourceView* GetTextureSRV(const std::string& name);

    static void Clear();

private:
    ResourceManager() = delete;

private:
    static std::unordered_map<std::string, VertexShaderPtr> s_vertexShaders;
    static std::unordered_map<std::string, PixelShaderPtr> s_pixelShaders;

    // vertexShader bytecode
    static std::unordered_map<std::string, BlobPtr> s_vertexShaderBlobs;

    static std::unordered_map<std::string, std::unique_ptr<Texture>> s_textures;
};