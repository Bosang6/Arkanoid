#include "SpriteRenderer.h"

using namespace DirectX;

bool SpriteRenderer::Initialize(ID3D11Device* device, HWND hwnd, const wchar_t* vsFilename, const wchar_t* psFilename)
{
    if (!device)
        return false;

    if (!InitializeQuad(device))
        return false;

    if (!m_spriteShader.Initialize(device, vsFilename, psFilename))
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

        (-0.5, -0.5) --------  (0.5, -0.5)
              |                  |
              |                  |
        (-0.5,  0.5) --------  (0.5,  0.5)

        之后绘制不同大小的 Sprite 时，不改顶点数据，
        而是通过 world matrix 做 scale + translation。
    */

    Vertex vertices[4];

    vertices[0].position = XMFLOAT3(-0.5f, -0.5f, 0.0f);
    vertices[0].texcoord = XMFLOAT2(0.0f, 0.0f);

    vertices[1].position = XMFLOAT3(0.5f, -0.5f, 0.0f);
    vertices[1].texcoord = XMFLOAT2(1.0f, 0.0f);

    vertices[2].position = XMFLOAT3(0.5f, 0.5f, 0.0f);
    vertices[2].texcoord = XMFLOAT2(1.0f, 1.0f);

    vertices[3].position = XMFLOAT3(-0.5f, 0.5f, 0.0f);
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

void SpriteRenderer::BindQuad(ID3D11DeviceContext* context)
{
    if (!context)
        return;

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    ID3D11Buffer* vertexBuffer = m_vertexBuffer.Get();

    context->IASetVertexBuffers(
        0,
        1,
        &vertexBuffer,
        &stride,
        &offset
    );

    context->IASetIndexBuffer(
        m_indexBuffer.Get(),
        DXGI_FORMAT_R32_UINT,
        0
    );

    context->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );
}

void SpriteRenderer::Draw(
    ID3D11DeviceContext* context,
    const GameObject& gameObject,
    const XMMATRIX& viewMatrix,
    const XMMATRIX& projectionMatrix)
{
    if (!context)
        return;

    if (!gameObject.IsActive())
        return;

    const Sprite* sprite = gameObject.GetSprite();

    if (!sprite)
        return;

    ID3D11ShaderResourceView* texture = sprite->GetTexture();

    if (!texture)
        return;

    if (!m_vertexBuffer || !m_indexBuffer)
        return;

    XMMATRIX worldMatrix = gameObject.GetTransform().GetWorldMatrix();

    //BindQuad(context);

    bool result = m_spriteShader.SetParameters(
        context,
        worldMatrix,
        viewMatrix,
        projectionMatrix,
        sprite->GetColor(),
        texture
    );

    if (!result)
        return;

    m_spriteShader.Bind(context);

    context->DrawIndexed(
        m_indexCount,
        0,
        0
    );
}

void SpriteRenderer::Draw(
    ID3D11DeviceContext* context,
    const std::vector<GameObject>& gameObjects,
    const XMMATRIX& viewMatrix,
    const XMMATRIX& projectionMatrix)
{
    if (!context)
        return;

    if (!m_vertexBuffer || !m_indexBuffer)
        return;

    BindQuad(context);
    m_spriteShader.Bind(context);

    for (const GameObject& gameObject : gameObjects)
    {
        DrawOne(
            context,
            gameObject,
            viewMatrix,
            projectionMatrix
        );
    }
}

void SpriteRenderer::DrawOne(
    ID3D11DeviceContext* context,
    const GameObject& gameObject,
    const XMMATRIX& viewMatrix,
    const XMMATRIX& projectionMatrix)
{
    if (!gameObject.IsActive())
        return;

    const Sprite* sprite = gameObject.GetSprite();

    if (!sprite)
        return;

    ID3D11ShaderResourceView* texture = sprite->GetTexture();

    if (!texture)
        return;

    XMMATRIX sizeMatrix = DirectX::XMMatrixScaling(
        sprite->GetWidth(),
        sprite->GetHeight(),
        1.0f
    );

    XMMATRIX transformMatrix =
        gameObject.GetTransform().GetWorldMatrix();

    XMMATRIX worldMatrix =
        sizeMatrix * transformMatrix;

    bool result = m_spriteShader.SetParameters(
        context,
        worldMatrix,
        viewMatrix,
        projectionMatrix,
        sprite->GetColor(),
        texture
    );

    if (!result)
        return;

    context->DrawIndexed(
        m_indexCount,
        0,
        0
    );
}