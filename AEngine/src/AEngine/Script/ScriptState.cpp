#include "AEngine/Script/ScriptState.h"
#include "AEngine/Core/Application.h"

#include "AEngine/Scene/Entity.h"
#include "AEngine/Scene/Scene.h"
#include "AEngine/Scene/Components.h"
#include "AEngine/Math/Math.hpp"

namespace AEngine
{
	ScriptState::ScriptState()
	{
		m_state.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string);

		//registering functions used inside lua script
		RegisterFunctions();

		//registering inputs
		RegisterMouseButtons();
		RegisterAlphaKeys();
		RegisterNumKeys();
		RegisterSpecialKeys();
		
		//registering data structures
		RegisterVec3();
		RegisterEntity();
		RegisterTransform();
	}

	void ScriptState::LoadFile(const std::string& path)
	{
		m_state.script_file(path);
	}

	void ScriptState::CallFunction(const std::string& functionName)
	{
		sol::protected_function function = m_state[functionName];
		if (function.valid())
		{
			function();
		}
	}

	sol::state& ScriptState::GetState()
	{
		return m_state;
	}

	bool PollMouseButton(AEMouse button)
	{
		bool isButtonPressed = AEngine::Application::Instance().Input().IsMouseButtonPressed(button);
		return isButtonPressed;
	}

	bool PollAlphaKey(AEKey key)
	{
		bool isKeyPressed = AEngine::Application::Instance().Input().IsKeyPressed(key);
		return isKeyPressed;
	}

	bool PollNumKey(AEKey key)
	{
		bool isKeyPressed = AEngine::Application::Instance().Input().IsKeyPressed(key);
		return isKeyPressed;
	}

	bool PollSpecialKey(AEKey key)
	{
		bool isKeyPressed = AEngine::Application::Instance().Input().IsKeyPressed(key);
		return isKeyPressed;
	}

	Math::vec3& GetTranslation(Entity entity)
	{
		return entity.GetComponent<TransformComponent>()->translation;
	}

	TransformComponent& GetTransform(Entity entity)
	{
		return *entity.GetComponent<TransformComponent>();
	}

	void ScriptState::RegisterTransform()
	{
		sol::usertype<TransformComponent> transformType = m_state.new_usertype<TransformComponent>("transform",
			sol::constructors<TransformComponent()>());

		//transformType["GetTranslation"] = &GetTranslation;
		transformType["translation"] = &TransformComponent::translation;
		transformType["scale"] = &TransformComponent::scale;
	}

	void ScriptState::RegisterEntity()
	{
		sol::usertype<AEngine::Entity> entityType = m_state.new_usertype<AEngine::Entity>("entity",
			sol::constructors<AEngine::Entity(entt::entity, AEngine::Scene*)>());

		entityType["GetTransform"] = &GetTransform;
	}

	void ScriptState::RegisterVec3()
	{
		sol::usertype<AEngine::Math::vec3> vectorType = m_state.new_usertype<AEngine::Math::vec3>("vec3",
			sol::constructors<AEngine::Math::vec3(), AEngine::Math::vec3(float, float, float)>());

		vectorType["x"] = &AEngine::Math::vec3::x;
		vectorType["y"] = &AEngine::Math::vec3::y;
		vectorType["z"] = &AEngine::Math::vec3::z;
	}

	AEngine::Math::vec3 GetVector()
	{
		return AEngine::Math::vec3(2, 2, 2);
	}

	void ScriptState::RegisterFunctions()
	{
		m_state["PollMouseButton"] = PollMouseButton;
		m_state["PollAlphaKey"] = PollAlphaKey;
		m_state["PollNumKey"] = PollNumKey;
		m_state["PollSpecialKey"] = PollSpecialKey;
		
		m_state["GetVector"] = GetVector;
		m_state["GetTranslation"] = GetTranslation;
		m_state["GetTransform"] = GetTransform;
	}

	void ScriptState::RegisterMouseButtons()
	{
		m_state["MouseButton"] = m_state.create_table_with(
			"Button1", AEMouse::BUTTON_1,
			"Button2", AEMouse::BUTTON_2,
			"Button3", AEMouse::BUTTON_3,
			"Button4", AEMouse::BUTTON_4,
			"Button5", AEMouse::BUTTON_5,
			"Button6", AEMouse::BUTTON_6,
			"Button7", AEMouse::BUTTON_7,
			"Button8", AEMouse::BUTTON_8,
			"Left", AEMouse::BUTTON_LEFT,
			"Right", AEMouse::BUTTON_RIGHT,
			"Middle", AEMouse::BUTTON_MIDDLE
		);
	}

	void ScriptState::RegisterAlphaKeys()
	{
		m_state["AlphaKeyCode"] = m_state.create_table_with(
			"A", AEKey::A,
			"B", AEKey::B,
			"C", AEKey::C,
			"D", AEKey::D,
			"E", AEKey::E,
			"F", AEKey::F,
			"G", AEKey::G,
			"H", AEKey::H,
			"I", AEKey::I,
			"J", AEKey::J,
			"K", AEKey::K,
			"L", AEKey::L,
			"M", AEKey::M,
			"N", AEKey::N,
			"O", AEKey::O,
			"P", AEKey::P,
			"Q", AEKey::Q,
			"R", AEKey::R,
			"S", AEKey::S,
			"T", AEKey::T,
			"U", AEKey::U,
			"V", AEKey::V,
			"W", AEKey::W,
			"X", AEKey::X,
			"Y", AEKey::Y,
			"Z", AEKey::Z
		);
	}

	void ScriptState::RegisterNumKeys()
	{
		m_state["NumKeyCode"] = m_state.create_table_with(
			"NUM0", AEKey::NUM0,
			"NUM1", AEKey::NUM1,
			"NUM2", AEKey::NUM2,
			"NUM3", AEKey::NUM3,
			"NUM4", AEKey::NUM4,
			"NUM5", AEKey::NUM5,
			"NUM6", AEKey::NUM6,
			"NUM7", AEKey::NUM7,
			"NUM8", AEKey::NUM8,
			"NUM9", AEKey::NUM9,
			"KP_0", AEKey::KP_0,
			"KP_1", AEKey::KP_1,
			"KP_2", AEKey::KP_2,
			"KP_3", AEKey::KP_3,
			"KP_4", AEKey::KP_4,
			"KP_5", AEKey::KP_5,
			"KP_6", AEKey::KP_6,
			"KP_7", AEKey::KP_7,
			"KP_8", AEKey::KP_8,
			"KP_9", AEKey::KP_9
		);
	}
	void ScriptState::RegisterSpecialKeys()
	{
		m_state["SpecialKeyCode"] = m_state.create_table_with(
			"SPACE", AEKey::SPACE,
			"APOSTROPHE", AEKey::APOSTROPHE,
			"COMMA", AEKey::COMMA,
			"MINUS", AEKey::MINUS,
			"PERIOD", AEKey::PERIOD,
			"SLASH", AEKey::SLASH,
			"SEMICOLON", AEKey::SEMICOLON,
			"EQUAL", AEKey::EQUAL,
			"LEFT_BRACKET", AEKey::LEFT_BRACKET,
			"BACKSLASH", AEKey::BACKSLASH,
			"RIGHT_BRACKET", AEKey::RIGHT_BRACKET,
			"GRAVE_ACCENT", AEKey::GRAVE_ACCENT,
			"ESCAPE", AEKey::ESCAPE,
			"ENTER", AEKey::ENTER,
			"TAB", AEKey::TAB,
			"BACKSPACE", AEKey::BACKSPACE,
			"INSERT", AEKey::INSERT,
			"DEL", AEKey::DEL,
			"RIGHT", AEKey::RIGHT,
			"LEFT", AEKey::LEFT,
			"DOWN", AEKey::DOWN,
			"UP", AEKey::UP,
			"PAGE_UP", AEKey::PAGE_UP,
			"PAGE_DOWN", AEKey::PAGE_DOWN,
			"HOME", AEKey::HOME,
			"END", AEKey::END,
			"CAPS_LOCK", AEKey::CAPS_LOCK,
			"SCROLL_LOCK", AEKey::SCROLL_LOCK,
			"NUM_LOCK", AEKey::NUM_LOCK,
			"PRINT_SCREEN", AEKey::PRINT_SCREEN,
			"PAUSE", AEKey::PAUSE,
			"F1", AEKey::F1,
			"F2", AEKey::F2,
			"F3", AEKey::F3,
			"F4", AEKey::F4,
			"F5", AEKey::F5,
			"F6", AEKey::F6,
			"F7", AEKey::F7,
			"F8", AEKey::F8,
			"F9", AEKey::F9,
			"F10", AEKey::F10,
			"F11", AEKey::F11,
			"F12", AEKey::F12,
			"F13", AEKey::F13,
			"F14", AEKey::F14,
			"F15", AEKey::F15,
			"F16", AEKey::F16,
			"F17", AEKey::F17,
			"F18", AEKey::F18,
			"F19", AEKey::F19,
			"F20", AEKey::F20,
			"F21", AEKey::F21,
			"F22", AEKey::F22,
			"F23", AEKey::F23,
			"F24", AEKey::F24,
			"F25", AEKey::F25,
			"KP_DECIMAL", AEKey::KP_DECIMAL,
			"KP_DIVIDE", AEKey::KP_DIVIDE,
			"KP_MULTIPLY", AEKey::KP_MULTIPLY,
			"KP_SUBTRACT", AEKey::KP_SUBTRACT,
			"KP_ADD", AEKey::KP_ADD,
			"KP_ENTER", AEKey::KP_ENTER,
			"KP_EQUAL", AEKey::KP_EQUAL,
			"LEFT_SHIFT", AEKey::LEFT_SHIFT,
			"LEFT_CONTROL", AEKey::LEFT_CONTROL,
			"LEFT_ALT", AEKey::LEFT_ALT,
			"RIGHT_SHIFT", AEKey::RIGHT_SHIFT,
			"RIGHT_CONTROL", AEKey::RIGHT_CONTROL,
			"RIGHT_ALT", AEKey::RIGHT_ALT
		);
	}
}

