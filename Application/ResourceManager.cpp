#include "pch.h"
#include "ResourceManager.h"

uint32 ResourceManager::LoadTexture(IRenderer* renderer, const std::wstring& path, const std::vector<BYTE>& pixels, int width, int height)
{
	auto iter = m_textureCache.find(path);
	if (iter != m_textureCache.end())
	{
		return iter->second;
	}

	uint32 id = renderer->UploadTextureRGBA(pixels.data(), width, height);
	m_textureCache[path] = id;
	return id;
}

void ResourceManager::UnloadAll(IRenderer* renderer)
{
	for (auto& t : m_textureCache)
	{
		renderer->UnloadTexture(t.second);
	}
	m_textureCache.clear();
}
