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

    /*
        在屏幕中画一张 PNG。

        x = 100
        y = 100
        width = 256
        height = 256

        如果你的 Camera2D 使用的是：
        XMMatrixOrthographicOffCenterLH(0, width, height, 0, near, far)

        那么坐标含义就是：
        左上角是 (0, 0)
        往右 x 增加
        往下 y 增加
    */
    m_spriteRenderer.DrawSprite(
        context,
        viewMatrix,
        projectionMatrix,
        "block",
        0.0f,
        0.0f,
        256.0f,
        256.0f
    );

    return true;
}