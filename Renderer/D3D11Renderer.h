#pragma once

#define RENDERER_DLL_EXPROT
#include "../Interface/IRenderer.h"

class D3D11Renderer : public IRenderer
{
public:
	~D3D11Renderer();

	// ~Begin of IRenderer interface
	virtual bool Init(void* hwnd, int width, int height) override;
	void Clear() override;

	virtual uint32 UploadTextureRGBA(const void* data, int width, int height) override;
	virtual void UnloadTexture(uint32 textureId) override;

	virtual void RenderFrame() override;
	virtual void DrawSprite(uint32 textureId, float posX, float posY, float width, float height, float u0, float v0, float u1, float v1, uint32 layer = 0) override;
	// ~End of IRenderer interface

private:
	static const uint32 MAX_INSTANCE_COUNT = 8192;

	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_context;
	ComPtr<IDXGISwapChain> m_swapchain;
	ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	ComPtr<ID3D11Buffer> m_constBuffer;
	ComPtr<ID3D11Buffer> m_instanceBuffer;
	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11PixelShader> m_pixelShader;
	ComPtr<ID3D11InputLayout> m_inputLayout;
	ComPtr<ID3D11SamplerState> m_sampler;
	HWND m_hwnd = nullptr;
	int32 m_width = 0;
	int32 m_height = 0;

	std::unordered_map<uint32, TextureRecord> m_textures;
	int32 m_lastTextureId = 1;

	std::vector<Sprite> m_drawList;

	// ~ Draw call
	void DrawSprite_Implementation();
};

