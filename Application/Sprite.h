#pragma once

#include "Entity.h"
#include "Transform.h"

class SpriteComponent
{
public:
	// Renderer texture handle.
	uint32 TextureId = 0;

	// Pivot 
	Vector2 Pivot = {};
	
	Vector2 Size = {};
	Vector2 Uv0 = {};
	Vector2 Uv1 = {};
	Vector4 Color = {};
	int Layer = 0;
};

class RenderSystem
{
public:
	ComponentArray<TransformComponent>* Transforms = nullptr;
	ComponentArray<SpriteComponent>* Sprites = nullptr;
	IRenderer* Renderer = nullptr;

	void Render();
};