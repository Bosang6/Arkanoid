#include "Transform2D.h"
using namespace DirectX;
XMMATRIX Transform2D::GetWorldMatrix() const
{
    XMMATRIX scaleMatrix = XMMatrixScaling(
        scale.x,
        scale.y,
        1.0f
    );

    XMMATRIX rotationMatrix = XMMatrixRotationZ(rotation);

    XMMATRIX translationMatrix = XMMatrixTranslation(
        position.x,
        position.y,
        0.0f
    );

    return scaleMatrix * rotationMatrix * translationMatrix;
}