#include "pch.h"
#include "Input.h"

void InputSystem::SetKey(Entity e, EInputMask flag, bool pressed)
{
	uint8& mask = KeyState[e];
	if (pressed)
	{
		mask |= flag;
	}
	else
	{
		mask &= ~flag;
	}
}
