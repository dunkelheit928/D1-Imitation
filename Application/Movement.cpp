#include "pch.h"
#include "Movement.h"

void MovementSystem::Update(float deltaTime)
{
    for (const auto& entity : Movements->Entities())
    {
        MovementComponent* movementComponent = Movements->Get(entity);
        TransformComponent* transformComponent = Transforms->Get(entity);
        if (movementComponent == nullptr || transformComponent == nullptr)
        {
            continue;
        }

        uint8_t flags = InputSystem->KeyState[entity];
        if (flags & Input_Left)
        {
            transformComponent->Position.x -= movementComponent->Speed * deltaTime;
        }
        if (flags & Input_Right)
        {
            transformComponent->Position.x += movementComponent->Speed * deltaTime;
        }
        if (flags & Input_Up)
        {
            transformComponent->Position.y -= movementComponent->Speed * deltaTime;
        }
        if (flags & Input_Down)
        {
            transformComponent->Position.y += movementComponent->Speed * deltaTime;
        }
    }
}