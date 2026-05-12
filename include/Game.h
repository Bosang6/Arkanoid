#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "SpriteRenderer.h"

class Game
{
public:
    Game() = default;
    ~Game() = default;

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

public:
    bool Initialize(
        ID3D11Device* device,
        ID3D11DeviceContext* context,
        HWND hwnd);

    void Shutdown();

    bool Update(float deltaTime);

    bool Render(
        ID3D11DeviceContext* context,
        const DirectX::XMMATRIX& viewMatrix,
        const DirectX::XMMATRIX& projectionMatrix);

private:
    SpriteRenderer m_spriteRenderer;

    // TEST
    GameObject m_block;
};