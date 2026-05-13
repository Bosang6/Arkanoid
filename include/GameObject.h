#pragma once

#include <memory>
#include <string>

#include "Transform2D.h"
#include "Sprite.h"

class GameObject
{
public:
    GameObject() = default;
    explicit GameObject(const std::string& name);
    ~GameObject() = default;

    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    GameObject(GameObject&&) noexcept = default;
    GameObject& operator=(GameObject&&) noexcept = default;

public:
    void SetName(const std::string& name);
    const std::string& GetName() const;

    void SetActive(bool active);
    bool IsActive() const;

    Transform2D& GetTransform();
    const Transform2D& GetTransform() const;

    Sprite* AddSprite(ID3D11ShaderResourceView* texture);
    Sprite* AddSprite(Texture* texture);
    void RemoveSprite();

    Sprite* GetSprite();
    const Sprite* GetSprite() const;

    bool HasSprite() const;

private:
    std::string m_name;
    bool m_active = true;

    Transform2D m_transform;

    std::unique_ptr<Sprite> m_sprite;
};