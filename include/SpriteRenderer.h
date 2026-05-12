#pragma once

#include <string>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

#include "SpriteShader.h"

class SpriteRenderer
{
public:
    SpriteRenderer() = default;
    ~SpriteRenderer() = default;

    SpriteRenderer(const SpriteRenderer&) = delete;
    SpriteRenderer& operator=(const SpriteRenderer&) = delete;

public:
    bool Initialize(ID3D11Device* device, HWND hwnd, const wchar_t* vsFilename, const wchar_t* psFilename);
    void Shutdown();

    void DrawSprite(
        ID3D11DeviceContext* context,
        const DirectX::XMMATRIX& viewMatrix,
        const DirectX::XMMATRIX& projectionMatrix,
        ID3D11ShaderResourceView* texture,
        float x,
        float y,
        float width,
        float height);

    void DrawSprite(
        ID3D11DeviceContext* context,
        const DirectX::XMMATRIX& viewMatrix,
        const DirectX::XMMATRIX& projectionMatrix,
        const std::string& textureName,
        float x,
        float y,
        float width,
        float height);

private:
    struct Vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 texcoord;
    };

private:
    bool InitializeQuad(ID3D11Device* device);

private:
    SpriteShader m_spriteShader;

    Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;

    UINT m_indexCount = 0;
};