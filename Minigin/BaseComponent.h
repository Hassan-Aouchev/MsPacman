#pragma once
#include <memory>
class GameObject;
class BaseComponent
{
    GameObject* m_pOwner = nullptr;
public:
    virtual ~BaseComponent() =default;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() const = 0;

    GameObject* GetOwner() const {return m_pOwner;}
protected:
    explicit BaseComponent(GameObject* pOwner):m_pOwner(pOwner){}
};

