#pragma once
#include <DirectXMath.h>

struct Transform2D
{
    DirectX::XMFLOAT2 position = { 0.0f, 0.0f };
    float rotation = 0.0f; // radians
    DirectX::XMFLOAT2 scale = { 1.0f, 1.0f };

    DirectX::XMMATRIX GetWorldMatrix() const;
};