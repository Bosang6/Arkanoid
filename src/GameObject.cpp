#include "GameObject.h"

GameObject::GameObject(const std::string& name)
    : m_name(name)
{
}

void GameObject::SetName(const std::string& name)
{
    m_name = name;
}

const std::string& GameObject::GetName() const
{
    return m_name;
}

void GameObject::SetActive(bool active)
{
    m_active = active;
}

bool GameObject::IsActive() const
{
    return m_active;
}

Transform2D& GameObject::GetTransform()
{
    return m_transform;
}

const Transform2D& GameObject::GetTransform() const
{
    return m_transform;
}

Sprite* GameObject::AddSprite(Texture* texture)
{
    m_sprite = std::make_unique<Sprite>();
    m_sprite->SetTexture(texture);

    return m_sprite.get();
}

void GameObject::RemoveSprite()
{
    m_sprite.reset();
}

Sprite* GameObject::GetSprite()
{
    return m_sprite.get();
}

const Sprite* GameObject::GetSprite() const
{
    return m_sprite.get();
}

bool GameObject::HasSprite() const
{
    return m_sprite != nullptr;
}