#pragma once
#include "BaseComponent.h"
#include "glm.hpp"
class MovementInputComponent :
    public BaseComponent
{
public:

    MovementInputComponent(GameObject* pOwner);

    ~MovementInputComponent() override = default;

    MovementInputComponent(const MovementInputComponent& other) = delete;
    MovementInputComponent(MovementInputComponent&& other) noexcept = delete;
    MovementInputComponent& operator=(const MovementInputComponent& other) = delete;
    MovementInputComponent& operator=(MovementInputComponent&& other) noexcept = delete;

    void Update(float elapsedSec) override ;
    void Render() const override {}

    void AddMovementInput(const glm::vec2& direction);
    void RemoveInput() { m_MovementInput = glm::vec2(0,0); }
    glm::vec2 GetMovementInput()const;
private:
    glm::vec2 m_MovementInput;
    float m_Tolerance;

};

