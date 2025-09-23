#pragma once

#include "Entity.h"

class TransformComponent
{
public:
	Vector2 Scale = { 1.0f, 1.0f };
	float Rotation = 0.0f;
	Vector2 Position = {};
};

class TransformSystem
{
public:
	ComponentArray<TransformComponent>* Transforms;

	void Update(float deltaTime);
};

