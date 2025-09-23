#pragma once

enum EInputMask : uint8
{
	Input_None = 0,
	Input_Left = 1 << 0,
	Input_Right = 1 << 1,
	Input_Up = 1 << 2,
	Input_Down = 1 << 3
};

class InputSystem
{
public:
	ComponentArray<TransformComponent>* Transforms = nullptr;
	std::unordered_map<Entity, uint8> KeyState;

	void SetKey(Entity e, EInputMask flag, bool pressed);
};

