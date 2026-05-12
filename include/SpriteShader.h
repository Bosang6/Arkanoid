#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#include <wrl/client.h>
#include "ResourceManager.h"
class SpriteShader
{
public:
    SpriteShader() = default;
    ~SpriteShader();

    bool Initialize(ID3D11Device* device, const wchar_t* vsFilename, const wchar_t* psFilename);
    void Shutdown();

    bool SetParameters(
        ID3D11DeviceContext* context,
        const DirectX::XMMATRIX& world,
        const DirectX::XMMATRIX& view,
        const DirectX::XMMATRIX& projection,
        ID3D11ShaderResourceView* texture);

    void Bind(ID3D11DeviceContext* context);

private:

    struct MatrixBufferType
    {
        DirectX::XMMATRIX world;
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX projection;
    };

private:
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_matrixBuffer;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
};