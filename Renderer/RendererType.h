#pragma once

struct TextureRecord
{
	uint32 id;
	int32 width;
	int32 height;
	ComPtr<ID3D11ShaderResourceView> srv;
};

