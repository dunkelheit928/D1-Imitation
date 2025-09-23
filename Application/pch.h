#pragma once

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN    

// Windows Header Files
#include <windows.h>
#include <wincodec.h>
#include <wrl.h>
using Microsoft::WRL::ComPtr;

// STL
#include <vector>
#include <chrono>
#include <unordered_map>
#include <string>

#include "../Common/Math.h"
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;
#include "../Common/Type.h"

#include "../Utils/Function.h"
#include "../Interface/IRenderer.h"

#include "Resource.h"

#include "Entity.h"
#include "Transform.h"
#include "Sprite.h"
#include "ResourceManager.h"
#include "Input.h"
#include "Movement.h"
