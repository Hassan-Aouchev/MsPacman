#pragma once

class GameObject;

class Command
{
public:
    virtual ~Command() = default;
    virtual void Execute() = 0;
};

class GameObjectCommand : public Command
{
public:
    GameObjectCommand(GameObject* gameObject) : m_GameObject{ gameObject } {}
    virtual ~GameObjectCommand() = default;

    void Execute() override = 0;

    bool isValid{ true };

protected:
    GameObject* GetGameObject() const { return m_GameObject; }

private:
    GameObject* m_GameObject;
};