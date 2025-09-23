#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef RENDERER_DLL_EXPROT
#define RENDERER_API __declspec(dllexport)
#else
#define RENDERER_API __declspec(dllimport)
#endif

struct IRenderer
{
	virtual ~IRenderer() = default;

	virtual bool Init(void* hwnd, int width, int height) = 0;
	virtual void Clear() = 0;

	virtual uint32 UploadTextureRGBA(const void* data, int width, int height) = 0;
	virtual void UnloadTexture(uint32 textureId) = 0;

	virtual void RenderFrame() = 0;
	virtual void DrawSprite(uint32 textureId, float posX, float posY, float width, float height, float u0, float v0, float u1, float v1, uint32 layer = 0) = 0;
};

extern "C"
{
	RENDERER_API IRenderer* Renderer_Create();
	RENDERER_API void Renderer_Destroy(IRenderer* renderer);
}