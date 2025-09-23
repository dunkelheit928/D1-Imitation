#pragma once

class MovementComponent
{
public:
	float Speed = 200.0f;
};

class MovementSystem
{
public:
	ComponentArray<TransformComponent>* Transforms = nullptr;
	ComponentArray<MovementComponent>* Movements = nullptr;
	InputSystem* InputSystem = nullptr;

	void Update(float deltaTime);
};

