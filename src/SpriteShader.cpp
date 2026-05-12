#include "SpriteShader.h"
using namespace DirectX;

SpriteShader::~SpriteShader()
{
    Shutdown();
}

bool SpriteShader::Initialize(ID3D11Device* device, const wchar_t* vsFilename, const wchar_t* psFilename)
{
    // vertex shader
    bool result = ResourceManager::LoadVertexShader(
        device,
        vsFilename,
        "VSMain",
        "SpriteVS"
    );

    if (!result)
        return false;

    // pixel shader
    result = ResourceManager::LoadPixelShader(
        device,
        psFilename,
        "PSMain",
        "SpritePS"
    );

    if (!result)
        return false;

    // layout setting
    ID3DBlob* vertexShaderBlob = ResourceManager::GetVertexShaderBlob("SpriteVS");

    if (!vertexShaderBlob)
        return false;

    D3D11_INPUT_ELEMENT_DESC layout[2] = {};

    layout[0].SemanticName = "POSITION";
    layout[0].SemanticIndex = 0;
    layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    layout[0].InputSlot = 0;
    layout[0].AlignedByteOffset = 0;
    layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    layout[0].InstanceDataStepRate = 0;

    layout[1].SemanticName = "TEXCOORD";
    layout[1].SemanticIndex = 0;
    layout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    layout[1].InputSlot = 0;
    layout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    layout[1].InstanceDataStepRate = 0;

    HRESULT hr = device->CreateInputLayout(
        layout,
        2,
        vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(),
        m_inputLayout.GetAddressOf()
    );

    if (FAILED(hr))
        return false;

    // constant buffer
    D3D11_BUFFER_DESC matrixBufferDesc = {};
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hr = device->CreateBuffer(
        &matrixBufferDesc,
        nullptr,
        m_matrixBuffer.GetAddressOf()
    );

    if (FAILED(hr))
        return false;

    // Sampler State
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    hr = device->CreateSamplerState(
        &samplerDesc,
        m_samplerState.GetAddressOf()
    );

    if (FAILED(hr))
        return false;

    return true;
}

void SpriteShader::Shutdown()
{
    m_samplerState.Reset();
    m_matrixBuffer.Reset();
    m_inputLayout.Reset();
}

/// <summary>
/// Set MVP matrix and texture
/// </summary>
/// <param name="context">Device content</param>
/// <param name="world">Model Matrix</param>
/// <param name="view">View Matrix</param>
/// <param name="projection">Projection Matrix</param>
/// <param name="texture">ShaderResourceView</param>
/// <returns></returns>
bool SpriteShader::SetParameters(
    ID3D11DeviceContext* context,
    const XMMATRIX& world,
    const XMMATRIX& view,
    const XMMATRIX& projection,
    ID3D11ShaderResourceView* texture)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource = {};

    HRESULT result = context->Map(
        m_matrixBuffer.Get(),
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mappedResource
    );

    if (FAILED(result))
        return false;

    MatrixBufferType* dataPtr =
        reinterpret_cast<MatrixBufferType*>(mappedResource.pData);

    dataPtr->world = XMMatrixTranspose(world);
    dataPtr->view = XMMatrixTranspose(view);
    dataPtr->projection = XMMatrixTranspose(projection);

    context->Unmap(m_matrixBuffer.Get(), 0);

    ID3D11Buffer* matrixBuffer = m_matrixBuffer.Get();
    context->VSSetConstantBuffers(0, 1, &matrixBuffer);

    context->PSSetShaderResources(0, 1, &texture);

    ID3D11SamplerState* samplerState = m_samplerState.Get();
    context->PSSetSamplers(0, 1, &samplerState);

    return true;
}

void SpriteShader::Bind(ID3D11DeviceContext* context)
{
    context->IASetInputLayout(m_inputLayout.Get());

    context->VSSetShader(
        ResourceManager::GetVertexShader("SpriteVS"),
        nullptr,
        0
    );

    context->PSSetShader(
        ResourceManager::GetPixelShader("SpritePS"),
        nullptr,
        0
    );
}