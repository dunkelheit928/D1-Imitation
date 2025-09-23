#include "pch.h"
#include "Sprite.h"

void RenderSystem::Render()
{
	if (Renderer == nullptr)
	{
		return;
	}

	const std::vector<Entity>& entities = Sprites->Entities();
	const std::vector<SpriteComponent>& spriteData = Sprites->Data();
	for (uint32 i = 0; i < entities.size(); i++)
	{
		Entity e = entities[i];
		const SpriteComponent& s = spriteData[i];
		TransformComponent* t = Transforms->Get(e);
		if (t == nullptr)
		{
			continue;
		}

		const Vector2 pos = t->Position - s.Size * s.Pivot * t->Scale;
		Renderer->DrawSprite(s.TextureId, pos.x, pos.y, s.Size.x, s.Size.y, s.Uv0.x, s.Uv0.y, s.Uv1.x, s.Uv1.y, (uint32)s.Layer);
	}
}
