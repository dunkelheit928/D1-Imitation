#pragma once

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN             
// Windows Header Files
#include <windows.h>

// D3D
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
using namespace DirectX;

// WRL
#include <wrl.h>
using Microsoft::WRL::ComPtr;

// STL
#include <vector>
#include <unordered_map>
#include <string>

// Custom Header
#include "../Common/Math.h"
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;
#include "../Common/Type.h"

#include "RendererType.h"
#include "RendererMacro.h"
