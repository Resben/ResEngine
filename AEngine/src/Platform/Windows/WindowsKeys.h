#pragma once
#include "AEngine/Core/KeyCodes.h"

namespace AEngine
{
	// convert from glfw key to AEKey
	enum AEKey ToAEKey(int key);
	// convert from AEKey to glfw key
	int ToGLFWKey(enum AEKey key);
	// convert from glfw mouse to AEMouse
	enum AEMouse ToAEMouse(int mouse);
	// convert from AEMouse to glfw mouse
	int ToGLFWMouse(enum AEMouse mouse);
}
