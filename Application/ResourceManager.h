#pragma once

class ResourceManager
{
public:
	uint32 LoadTexture(IRenderer* renderer, const std::wstring& path, const std::vector<BYTE>& pixels, int width, int height);
	void UnloadAll(IRenderer* renderer);

private:
	std::unordered_map<std::wstring, uint32> m_textureCache;
};

