#include "SpriteRenderer.h"

#include "ResourceManager.h"

using namespace DirectX;

bool SpriteRenderer::Initialize(ID3D11Device* device, HWND hwnd, const wchar_t* vsFilename, const wchar_t* psFilename)
{
    if (!device)
        return false;

    if (!InitializeQuad(device))
        return false;

    if (!m_spriteShader.Initialize(device, hwnd, vsFilename, psFilename))
        return false;

    return true;
}

void SpriteRenderer::Shutdown()
{
    m_spriteShader.Shutdown();

    m_indexBuffer.Reset();
    m_vertexBuffer.Reset();

    m_indexCount = 0;
}

bool SpriteRenderer::InitializeQuad(ID3D11Device* device)
{
    /*
        创建一个 1x1 的 Quad。

        顶点位置：

        (0,0) -------- (1,0)
          |              |
          |              |
        (0,1) -------- (1,1)

        之后绘制不同大小的 Sprite 时，不改顶点数据，
        而是通过 world matrix 做 scale + translation。
    */

    Vertex vertices[4];

    vertices[0].position = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertices[0].texcoord = XMFLOAT2(0.0f, 0.0f);

    vertices[1].position = XMFLOAT3(1.0f, 0.0f, 0.0f);
    vertices[1].texcoord = XMFLOAT2(1.0f, 0.0f);

    vertices[2].position = XMFLOAT3(1.0f, 1.0f, 0.0f);
    vertices[2].texcoord = XMFLOAT2(1.0f, 1.0f);

    vertices[3].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
    vertices[3].texcoord = XMFLOAT2(0.0f, 1.0f);

    unsigned int indices[6] =
    {
        0, 1, 2,
        0, 2, 3
    };

    m_indexCount = 6;

    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * 4;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = vertices;

    HRESULT hr = device->CreateBuffer(
        &vertexBufferDesc,
        &vertexData,
        m_vertexBuffer.GetAddressOf()
    );

    if (FAILED(hr))
        return false;

    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned int) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = indices;

    hr = device->CreateBuffer(
        &indexBufferDesc,
        &indexData,
        m_indexBuffer.GetAddressOf()
    );

    if (FAILED(hr))
        return false;

    return true;
}

void SpriteRenderer::DrawSprite(
    ID3D11DeviceContext* context,
    const DirectX::XMMATRIX& viewMatrix,
    const XMMATRIX& projectionMatrix,
    ID3D11ShaderResourceView* texture,
    float x,
    float y,
    float width,
    float height)
{
    if (!context || !texture)
        return;

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    ID3D11Buffer* vertexBuffer = m_vertexBuffer.Get();
    ID3D11Buffer* indexBuffer = m_indexBuffer.Get();

    context->IASetVertexBuffers(
        0,
        1,
        &vertexBuffer,
        &stride,
        &offset
    );

    context->IASetIndexBuffer(
        indexBuffer,
        DXGI_FORMAT_R32_UINT,
        0
    );

    context->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );

    /*
        Quad 本身是 1x1。

        先缩放成 width x height，
        再移动到 x, y。

        因为我们使用的是：
        XMMatrixScaling(...) * XMMatrixTranslation(...)

        所以实际效果是：
        local vertex -> scale -> translate
    */
    XMMATRIX worldMatrix =
        XMMatrixScaling(width, height, 1.0f) *
        XMMatrixTranslation(x, y, 0.0f);

    m_spriteShader.Bind(context);

    bool result = m_spriteShader.SetParameters(
        context,
        worldMatrix,
        viewMatrix,
        projectionMatrix,
        texture
    );

    if (!result)
        return;

    context->DrawIndexed(m_indexCount, 0, 0);
}

void SpriteRenderer::DrawSprite(
    ID3D11DeviceContext* context,
    const XMMATRIX& viewMatrix,
    const XMMATRIX& projectionMatrix,
    const std::string& textureName,
    float x,
    float y,
    float width,
    float height)
{
    ID3D11ShaderResourceView* texture =
        ResourceManager::GetTextureSRV(textureName);
    DrawSprite(
        context,
        viewMatrix,
        projectionMatrix,
        texture,
        x,
        y,
        width,
        height
    );
}