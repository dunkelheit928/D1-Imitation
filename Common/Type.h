#pragma once

using BYTE = unsigned __int8;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;

struct Vertex
{
	float x;
	float y;
	float z;
	float u;
	float v;
};

struct InstanceData
{
	Vector2 pos;
	Vector2 size;
	Vector2 uv0;
	Vector2 uv1;
	Vector4 color;
};

struct Sprite
{
	uint32 textureId;
	InstanceData instData;
};
