#pragma once

#include <string>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

#include "ResourceManager.h"
#include "SpriteShader.h"
#include "GameObject.h"

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
    void Draw(
        ID3D11DeviceContext* context,
        const GameObject& gameObject,
        const DirectX::XMMATRIX& viewMatrix,
        const DirectX::XMMATRIX& projectionMatrix);

    void Draw(
        ID3D11DeviceContext* context,
        const std::vector<GameObject>& gameObjects,
        const DirectX::XMMATRIX& viewMatrix,
        const DirectX::XMMATRIX& projectionMatrix);

private:
    struct Vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 texcoord;
    };

private:
    bool InitializeQuad(ID3D11Device* device);
    void BindQuad(ID3D11DeviceContext* context);
    void DrawOne(
        ID3D11DeviceContext* context,
        const GameObject& gameObject,
        const DirectX::XMMATRIX& viewMatrix,
        const DirectX::XMMATRIX& projectionMatrix);

private:
    SpriteShader m_spriteShader;

    Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;

    UINT m_indexCount = 0;
};