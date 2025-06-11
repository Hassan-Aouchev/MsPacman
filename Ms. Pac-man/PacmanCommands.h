#pragma once
#include <Command.h>
#include <glm.hpp>

class GameObject;

class Move : public GameObjectCommand
{
public:
    Move(GameObject* gameObject, glm::vec2 direction);
    virtual ~Move() = default;
    void Execute() override;

private:
    glm::vec2 m_direction;
};

class Damage : public GameObjectCommand
{
public:
    Damage(GameObject* gameObject);
    virtual ~Damage() = default;
    void Execute() override;
};

class Score : public GameObjectCommand
{
public:
    Score(GameObject* gameObject);
    virtual ~Score() = default;
    void Execute() override;
};