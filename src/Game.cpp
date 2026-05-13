#include "Game.h"

#include "ResourceManager.h"
bool Game::Initialize(
    ID3D11Device* device,
    ID3D11DeviceContext* context,
    HWND hwnd)
{
    if (!device || !context)
        return false;

    // 1. 加载一张 PNG 图片
    if (!ResourceManager::LoadTexture(
        device,
        context,
        L"Assets/Textures/block_solid.png",
        "block"))
    {
        return false;
    }

    // 2. 初始化 SpriteRenderer
    // SpriteRenderer 内部会初始化 SpriteShader、Quad VB/IB 等
    if (!m_spriteRenderer.Initialize(device, hwnd, L"Shaders/SpriteVertexShader.hlsl", L"Shaders/SpritePixelShader.hlsl"))
    {
        return false;
    }

    // 3. 初始化 GameObject
    GameObject block("Block1");

    Texture* texture = ResourceManager::GetTexture("block");

    block.GetTransform().position = { 0.0f, 0.0f };
    block.GetTransform().rotation = 0.0f;
    block.GetTransform().scale = { 1.0, 1.0 };

    // 颜色叠加
    Sprite* s = block.AddSprite(texture);
    s->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
    m_gameObjects.push_back(std::move(block));

    GameObject block1("Block1");

    block1.GetTransform().position = { 129.0f, 0.0f };
    block1.GetTransform().rotation = 1.0f;
    block1.GetTransform().scale = { 1, 1 };

    // 颜色叠加
    s = block1.AddSprite(texture);
    s->SetColor({ 0.0f, 1.0f, 0.5f, 1.0f });

    m_gameObjects.push_back(std::move(block1));

    return true;
}

void Game::Shutdown()
{
    m_spriteRenderer.Shutdown();

    // 如果 ResourceManager 是全局静态资源管理器，
    // 可以在 Game 结束时清空资源。
    ResourceManager::Clear();
}

bool Game::Update(float deltaTime)
{
    // 现在先不写游戏逻辑
    // 之后可以在这里更新球、挡板、砖块等
    return true;
}

bool Game::Render(
    ID3D11DeviceContext* context,
    const DirectX::XMMATRIX& viewMatrix,
    const DirectX::XMMATRIX& projectionMatrix)
{
    if (!context)
        return false;

    m_spriteRenderer.Draw(
        context,
        m_gameObjects,
        viewMatrix,
        projectionMatrix
    );

    return true;
}