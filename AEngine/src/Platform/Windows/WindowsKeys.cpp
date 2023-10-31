/**
 * @file
 * @author Christien Alden (34119981)
 * Converts GLFW keys into AEKeys
**/
#pragma once
#include <map>
#include <GLFW/glfw3.h>
#include "WindowsKeys.h"

namespace AEngine
{
	static const std::map<int, enum AEKey> keyMap = {
		// common
		{GLFW_KEY_SPACE,         AEKey::SPACE},
		{GLFW_KEY_APOSTROPHE,    AEKey::APOSTROPHE},
		{GLFW_KEY_COMMA,         AEKey::COMMA},
		{GLFW_KEY_MINUS,         AEKey::MINUS},
		{GLFW_KEY_PERIOD,        AEKey::PERIOD},
		{GLFW_KEY_SLASH,         AEKey::SLASH},
		{GLFW_KEY_0,             AEKey::NUM0},
		{GLFW_KEY_1,             AEKey::NUM1},
		{GLFW_KEY_2,             AEKey::NUM2},
		{GLFW_KEY_3,             AEKey::NUM3},
		{GLFW_KEY_4,             AEKey::NUM4},
		{GLFW_KEY_5,             AEKey::NUM5},
		{GLFW_KEY_6,             AEKey::NUM6},
		{GLFW_KEY_7,             AEKey::NUM7},
		{GLFW_KEY_8,             AEKey::NUM8},
		{GLFW_KEY_9,             AEKey::NUM9},
		{GLFW_KEY_SEMICOLON,     AEKey::SEMICOLON},
		{GLFW_KEY_EQUAL,         AEKey::EQUAL},
		{GLFW_KEY_A,             AEKey::A},
		{GLFW_KEY_B,             AEKey::B},
		{GLFW_KEY_C,             AEKey::C},
		{GLFW_KEY_D,             AEKey::D},
		{GLFW_KEY_E,             AEKey::E},
		{GLFW_KEY_F,             AEKey::F},
		{GLFW_KEY_G,             AEKey::G},
		{GLFW_KEY_H,             AEKey::H},
		{GLFW_KEY_I,             AEKey::I},
		{GLFW_KEY_J,             AEKey::J},
		{GLFW_KEY_K,             AEKey::K},
		{GLFW_KEY_L,             AEKey::L},
		{GLFW_KEY_M,             AEKey::M},
		{GLFW_KEY_N,             AEKey::N},
		{GLFW_KEY_O,             AEKey::O},
		{GLFW_KEY_P,             AEKey::P},
		{GLFW_KEY_Q,             AEKey::Q},
		{GLFW_KEY_R,             AEKey::R},
		{GLFW_KEY_S,             AEKey::S},
		{GLFW_KEY_T,             AEKey::T},
		{GLFW_KEY_U,             AEKey::U},
		{GLFW_KEY_V,             AEKey::V},
		{GLFW_KEY_W,             AEKey::W},
		{GLFW_KEY_X,             AEKey::X},
		{GLFW_KEY_Y,             AEKey::Y},
		{GLFW_KEY_Z,             AEKey::Z},
		{GLFW_KEY_LEFT_BRACKET,  AEKey::LEFT_BRACKET},
		{GLFW_KEY_BACKSLASH,     AEKey::BACKSLASH},
		{GLFW_KEY_RIGHT_BRACKET, AEKey::RIGHT_BRACKET},
		{GLFW_KEY_GRAVE_ACCENT,  AEKey::GRAVE_ACCENT},

		// function
		{GLFW_KEY_ESCAPE,        AEKey::ESCAPE},
		{GLFW_KEY_ENTER,         AEKey::ENTER},
		{GLFW_KEY_TAB,           AEKey::TAB},
		{GLFW_KEY_BACKSPACE,     AEKey::BACKSPACE},
		{GLFW_KEY_INSERT,        AEKey::INSERT},
		{GLFW_KEY_DELETE,        AEKey::DEL},
		{GLFW_KEY_RIGHT,         AEKey::RIGHT},
		{GLFW_KEY_LEFT,          AEKey::LEFT},
		{GLFW_KEY_DOWN,          AEKey::DOWN},
		{GLFW_KEY_UP,            AEKey::UP},
		{GLFW_KEY_PAGE_UP,       AEKey::PAGE_UP},
		{GLFW_KEY_PAGE_DOWN,     AEKey::PAGE_DOWN},
		{GLFW_KEY_HOME,          AEKey::HOME},
		{GLFW_KEY_END,           AEKey::END},
		{GLFW_KEY_CAPS_LOCK,     AEKey::CAPS_LOCK},
		{GLFW_KEY_SCROLL_LOCK,   AEKey::SCROLL_LOCK},
		{GLFW_KEY_NUM_LOCK,      AEKey::NUM_LOCK},
		{GLFW_KEY_PRINT_SCREEN,  AEKey::PRINT_SCREEN},
		{GLFW_KEY_PAUSE,         AEKey::PAUSE},
		{GLFW_KEY_F1,            AEKey::F1},
		{GLFW_KEY_F2,            AEKey::F2},
		{GLFW_KEY_F3,            AEKey::F3},
		{GLFW_KEY_F4,            AEKey::F4},
		{GLFW_KEY_F5,            AEKey::F5},
		{GLFW_KEY_F6,            AEKey::F6},
		{GLFW_KEY_F7,            AEKey::F7},
		{GLFW_KEY_F8,            AEKey::F8},
		{GLFW_KEY_F9,            AEKey::F9},
		{GLFW_KEY_F10,           AEKey::F10},
		{GLFW_KEY_F11,           AEKey::F11},
		{GLFW_KEY_F12,           AEKey::F12},
		{GLFW_KEY_F13,           AEKey::F13},
		{GLFW_KEY_F14,           AEKey::F14},
		{GLFW_KEY_F15,           AEKey::F15},
		{GLFW_KEY_F16,           AEKey::F16},
		{GLFW_KEY_F17,           AEKey::F17},
		{GLFW_KEY_F18,           AEKey::F18},
		{GLFW_KEY_F19,           AEKey::F19},
		{GLFW_KEY_F20,           AEKey::F20},
		{GLFW_KEY_F21,           AEKey::F21},
		{GLFW_KEY_F22,           AEKey::F22},
		{GLFW_KEY_F23,           AEKey::F23},
		{GLFW_KEY_F24,           AEKey::F24},
		{GLFW_KEY_F25,           AEKey::F25},
		{GLFW_KEY_KP_0,          AEKey::KP_0},
		{GLFW_KEY_KP_1,          AEKey::KP_1},
		{GLFW_KEY_KP_2,          AEKey::KP_2},
		{GLFW_KEY_KP_3,          AEKey::KP_3},
		{GLFW_KEY_KP_4,          AEKey::KP_4},
		{GLFW_KEY_KP_5,          AEKey::KP_5},
		{GLFW_KEY_KP_6,          AEKey::KP_6},
		{GLFW_KEY_KP_7,          AEKey::KP_7},
		{GLFW_KEY_KP_8,          AEKey::KP_8},
		{GLFW_KEY_KP_9,          AEKey::KP_9},
		{GLFW_KEY_KP_DECIMAL,    AEKey::KP_DECIMAL},
		{GLFW_KEY_KP_DIVIDE,     AEKey::KP_DIVIDE},
		{GLFW_KEY_KP_MULTIPLY,   AEKey::KP_MULTIPLY},
		{GLFW_KEY_KP_SUBTRACT,   AEKey::KP_SUBTRACT},
		{GLFW_KEY_KP_ADD,        AEKey::KP_ADD},
		{GLFW_KEY_KP_ENTER,      AEKey::KP_ENTER},
		{GLFW_KEY_KP_EQUAL,      AEKey::KP_EQUAL},
		{GLFW_KEY_LEFT_SHIFT,    AEKey::LEFT_SHIFT},
		{GLFW_KEY_LEFT_CONTROL,  AEKey::LEFT_CONTROL},
		{GLFW_KEY_LEFT_ALT,      AEKey::LEFT_ALT},
		{GLFW_KEY_RIGHT_SHIFT,   AEKey::RIGHT_SHIFT},
		{GLFW_KEY_RIGHT_CONTROL, AEKey::RIGHT_CONTROL},
		{GLFW_KEY_RIGHT_ALT,     AEKey::RIGHT_ALT}
	};

	static const std::map<int, AEMouse> mouseMap = {
		{GLFW_MOUSE_BUTTON_1,      AEMouse::BUTTON_1},
		{GLFW_MOUSE_BUTTON_2,      AEMouse::BUTTON_2},
		{GLFW_MOUSE_BUTTON_3,      AEMouse::BUTTON_3},
		{GLFW_MOUSE_BUTTON_4,      AEMouse::BUTTON_4},
		{GLFW_MOUSE_BUTTON_5,      AEMouse::BUTTON_5},
		{GLFW_MOUSE_BUTTON_6,      AEMouse::BUTTON_6},
		{GLFW_MOUSE_BUTTON_7,      AEMouse::BUTTON_7},
		{GLFW_MOUSE_BUTTON_8,      AEMouse::BUTTON_8},
		{GLFW_MOUSE_BUTTON_LEFT,   AEMouse::BUTTON_LEFT},
		{GLFW_MOUSE_BUTTON_RIGHT,  AEMouse::BUTTON_RIGHT},
		{GLFW_MOUSE_BUTTON_MIDDLE, AEMouse::BUTTON_MIDDLE}
	};

	AEKey ToAEKey(int key)
	{
		std::map<int, enum AEKey>::const_iterator it = keyMap.find(key);
		if (it == keyMap.end())
		{
			return AEKey::INVALID;
		}
		else
		{
			return it->second;
		}
	}

	int ToGLFWKey(AEKey aeKey)
	{
		std::map<int, enum AEKey>::const_iterator it;
		for (it = keyMap.begin(); it != keyMap.end(); ++it)
		{
			if (it->second == aeKey)
			{
				return it->first;
			}
		}

		return GLFW_KEY_UNKNOWN;
	}

	AEMouse ToAEMouse(int mouse)
	{
		std::map<int, AEMouse>::const_iterator it = mouseMap.find(mouse);
		if (it == mouseMap.end())
		{
			return AEMouse::INVALID;
		}
		else
		{
			return it->second;
		}
	}

	int ToGLFWMouse(AEMouse mouse)
	{
		std::map<int, AEMouse>::const_iterator it;
		for (it = mouseMap.begin(); it != mouseMap.end(); ++it)
		{
			if (it->second == mouse)
			{
				return it->first;
			}
		}

		return GLFW_KEY_UNKNOWN;
	}
}
