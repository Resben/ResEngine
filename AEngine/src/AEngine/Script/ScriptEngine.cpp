#include "ScriptEngine.h"
#include "AEngine/Core/Application.h"
#include "AEngine/Core/Input.h"
#include "AEngine/Core/KeyCodes.h"
#include "AEngine/Math/Math.h"
#include "AEngine/Scene/Components.h"
#include "AEngine/Scene/Entity.h"

namespace AEngine
{
	ScriptEngine& ScriptEngine::Instance()
	{
		static ScriptEngine instance;
		return instance;
	}

	void ScriptEngine::LoadFile(const std::string& path)
	{
		m_state.LoadFile(path);
	}

	void ScriptEngine::LoadScript(const std::string &script)
	{
		m_state.LoadScript(script);
	}

	void ScriptEngine::Shutdown()
	{
		/// \todo Implement ScriptEngine::Shutdown
	}

	ScriptState &ScriptEngine::GetState()
	{
		return m_state;
	}

//--------------------------------------------------------------------------------
// Initialisation
//--------------------------------------------------------------------------------
	void RegisterInputPolling(sol::state& state);
	void RegisterMouseCodes(sol::state& state);
	void RegisterKeyCodes(sol::state& state);
	void RegisterMathNamespace(sol::state& state);
	void RegisterVec2(sol::state& state);
	void RegisterVec3(sol::state& state);
	void RegisterQuat(sol::state& state);
	void RegisterEntity(sol::state& state);
	void RegisterTransformComponent(sol::state& state);
	void RegisterRenderableComponent(sol::state& state);
	void RegisterApplication(sol::state& state);

	void ScriptEngine::Init()
	{
		Instance();

		// caputure reference to internal sol state
		sol::state& solState = m_state.GetNative();

		// input functions
		RegisterInputPolling(solState);
		RegisterKeyCodes(solState);
		RegisterMouseCodes(solState);

		// math functions
		RegisterMathNamespace(solState);
		RegisterVec2(solState);
		RegisterVec3(solState);
		RegisterQuat(solState);

		// ecs functions
		RegisterEntity(solState);
		RegisterTransformComponent(solState);
		RegisterRenderableComponent(solState);

		// app
		RegisterApplication(solState);
	}

//--------------------------------------------------------------------------------
// Input Functions
//--------------------------------------------------------------------------------
	inline void RegisterInputPolling(sol::state& state)
	{
		state["PollKey"] = [](AEKey key) -> bool {
			return Application::Instance().Input().IsKeyPressed(key);
		};
		state["PollMouse"] = [](AEMouse mouse) -> bool {
			return Application::Instance().Input().IsMouseButtonPressed(mouse);
		};

		state["GetMouseDelta"] = []() -> Math::vec2 {
			return Application::Instance().Input().GetMouseDelta();
		};

		state["GetMousePosition"] = []() -> Math::vec2 {
			return Application::Instance().Input().GetMousePosition();
		};
	}

	void RegisterMouseCodes(sol::state& state)
	{
		state.new_enum<AEMouse>("AEMouse", {
			{"BUTTON1", AEMouse::BUTTON_1},
			{"BUTTON2", AEMouse::BUTTON_2},
			{"BUTTON3", AEMouse::BUTTON_3},
			{"BUTTON4", AEMouse::BUTTON_4},
			{"BUTTON5", AEMouse::BUTTON_5},
			{"BUTTON6", AEMouse::BUTTON_6},
			{"BUTTON7", AEMouse::BUTTON_7},
			{"BUTTON8", AEMouse::BUTTON_8},
			{"LEFT", AEMouse::BUTTON_LEFT},
			{"RIGHT", AEMouse::BUTTON_RIGHT},
			{"MIDDLE", AEMouse::BUTTON_MIDDLE}
		});
	}

	void RegisterKeyCodes(sol::state& state)
	{
		state.new_enum<AEKey>("AEKey", {
			// alpha keys
			{"A", AEKey::A},
			{"B", AEKey::B},
			{"C", AEKey::C},
			{"D", AEKey::D},
			{"E", AEKey::E},
			{"F", AEKey::F},
			{"G", AEKey::G},
			{"H", AEKey::H},
			{"I", AEKey::I},
			{"J", AEKey::J},
			{"K", AEKey::K},
			{"L", AEKey::L},
			{"M", AEKey::M},
			{"N", AEKey::N},
			{"O", AEKey::O},
			{"P", AEKey::P},
			{"Q", AEKey::Q},
			{"R", AEKey::R},
			{"S", AEKey::S},
			{"T", AEKey::T},
			{"U", AEKey::U},
			{"V", AEKey::V},
			{"W", AEKey::W},
			{"X", AEKey::X},
			{"Y", AEKey::Y},
			{"Z", AEKey::Z},

			// number keys
			{"NUM0", AEKey::NUM0},
			{"NUM1", AEKey::NUM1},
			{"NUM2", AEKey::NUM2},
			{"NUM3", AEKey::NUM3},
			{"NUM4", AEKey::NUM4},
			{"NUM5", AEKey::NUM5},
			{"NUM6", AEKey::NUM6},
			{"NUM7", AEKey::NUM7},
			{"NUM8", AEKey::NUM8},
			{"NUM9", AEKey::NUM9},
			{"KP_0", AEKey::KP_0},
			{"KP_1", AEKey::KP_1},
			{"KP_2", AEKey::KP_2},
			{"KP_3", AEKey::KP_3},
			{"KP_4", AEKey::KP_4},
			{"KP_5", AEKey::KP_5},
			{"KP_6", AEKey::KP_6},
			{"KP_7", AEKey::KP_7},
			{"KP_8", AEKey::KP_8},
			{"KP_9", AEKey::KP_9},

			// special keys
			{"SPACE", AEKey::SPACE},
			{"APOSTROPHE", AEKey::APOSTROPHE},
			{"COMMA", AEKey::COMMA},
			{"MINUS", AEKey::MINUS},
			{"PERIOD", AEKey::PERIOD},
			{"SLASH", AEKey::SLASH},
			{"SEMICOLON", AEKey::SEMICOLON},
			{"EQUAL", AEKey::EQUAL},
			{"LEFT_BRACKET", AEKey::LEFT_BRACKET},
			{"BACKSLASH", AEKey::BACKSLASH},
			{"RIGHT_BRACKET", AEKey::RIGHT_BRACKET},
			{"GRAVE_ACCENT", AEKey::GRAVE_ACCENT},
			{"ESCAPE", AEKey::ESCAPE},
			{"ENTER", AEKey::ENTER},
			{"TAB", AEKey::TAB},
			{"BACKSPACE", AEKey::BACKSPACE},
			{"INSERT", AEKey::INSERT},
			{"DEL", AEKey::DEL},
			{"RIGHT", AEKey::RIGHT},
			{"LEFT", AEKey::LEFT},
			{"DOWN", AEKey::DOWN},
			{"UP", AEKey::UP},
			{"PAGE_UP", AEKey::PAGE_UP},
			{"PAGE_DOWN", AEKey::PAGE_DOWN},
			{"HOME", AEKey::HOME},
			{"END", AEKey::END},
			{"CAPS_LOCK", AEKey::CAPS_LOCK},
			{"SCROLL_LOCK", AEKey::SCROLL_LOCK},
			{"NUM_LOCK", AEKey::NUM_LOCK},
			{"PRINT_SCREEN", AEKey::PRINT_SCREEN},
			{"PAUSE", AEKey::PAUSE},
			{"F1", AEKey::F1},
			{"F2", AEKey::F2},
			{"F3", AEKey::F3},
			{"F4", AEKey::F4},
			{"F5", AEKey::F5},
			{"F6", AEKey::F6},
			{"F7", AEKey::F7},
			{"F8", AEKey::F8},
			{"F9", AEKey::F9},
			{"F10", AEKey::F10},
			{"F11", AEKey::F11},
			{"F12", AEKey::F12},
			{"F13", AEKey::F13},
			{"F14", AEKey::F14},
			{"F15", AEKey::F15},
			{"F16", AEKey::F16},
			{"F17", AEKey::F17},
			{"F18", AEKey::F18},
			{"F19", AEKey::F19},
			{"F20", AEKey::F20},
			{"F21", AEKey::F21},
			{"F22", AEKey::F22},
			{"F23", AEKey::F23},
			{"F24", AEKey::F24},
			{"F25", AEKey::F25},
			{"KP_DECIMAL", AEKey::KP_DECIMAL},
			{"KP_DIVIDE", AEKey::KP_DIVIDE},
			{"KP_MULTIPLY", AEKey::KP_MULTIPLY},
			{"KP_SUBTRACT", AEKey::KP_SUBTRACT},
			{"KP_ADD", AEKey::KP_ADD},
			{"KP_ENTER", AEKey::KP_ENTER},
			{"KP_EQUAL", AEKey::KP_EQUAL},
			{"LEFT_SHIFT", AEKey::LEFT_SHIFT},
			{"LEFT_CONTROL", AEKey::LEFT_CONTROL},
			{"LEFT_ALT", AEKey::LEFT_ALT},
			{"RIGHT_SHIFT", AEKey::RIGHT_SHIFT},
			{"RIGHT_CONTROL", AEKey::RIGHT_CONTROL},
			{"RIGHT_ALT", AEKey::RIGHT_ALT}
		});
	};

//--------------------------------------------------------------------------------
// Math Functions
//--------------------------------------------------------------------------------
	void RegisterMathNamespace(sol::state& state)
	{
		auto rotate_overload = sol::overload(
			[](const Math::quat& q, float angle, const Math::vec3& axis) -> Math::quat {
				return Math::rotate(q, angle, axis);
			}
		);

		auto radians = [](float degrees) -> float { 
			return Math::radians(degrees);
		};

		auto degrees = [](float radians) -> float {
			return Math::degrees(radians);
		};

		state["Math"] = state.create_table();
		state["Math"]["Rotate"] = rotate_overload;
		state["Math"]["RotateVec"] = Math::rotateVec;
		state["Math"]["Radians"] = radians;
		state["Math"]["Degrees"] = degrees;
	}

	void RegisterVec2(sol::state& state)
	{
		auto add_overload = sol::overload(
			[](const Math::vec2& v1, const Math::vec2& v2) -> Math::vec2 { return v1 + v2; },
			[](const Math::vec2& v, float f) -> Math::vec2 { return v + f; },
			[](float f, const Math::vec2& v) -> Math::vec2 { return f + v; }
		);

		auto sub_overload = sol::overload(
			[](const Math::vec2& v1, const Math::vec2& v2) -> Math::vec2 { return v1 - v2; },
			[](const Math::vec2& v, float f) -> Math::vec2 { return v - f; },
			[](float f, const Math::vec2& v) -> Math::vec2 { return f - v; }
		);

		auto mult_overload = sol::overload(
			[](const Math::vec2& v1, const Math::vec2& v2) -> Math::vec2 { return v1 * v2; },
			[](const Math::vec2& v, float f) -> Math::vec2 { return v * f; },
			[](float f, const Math::vec2& v) -> Math::vec2 { return f * v; }
		);

		auto div_overload = sol::overload(
			[](const Math::vec2& v1, const Math::vec2& v2) -> Math::vec2 { return v1 / v2; },
			[](const Math::vec2& v, float f) -> Math::vec2 { return v / f; },
			[](float f, const Math::vec2& v) -> Math::vec2 { return f / v; }
		);

		auto unary_minus = [](const Math::vec2& v) -> Math::vec2 { return -v; };
		auto equal_to = [](const Math::vec2& v1, const Math::vec2& v2) -> Math::vec2 { return Math::equal(v1, v2); };

		state.new_usertype<Math::vec2>("vec2",
			sol::constructors<Math::vec2(), Math::vec2(float, float)>(),
			"x", &Math::vec2::x,
			"y", &Math::vec2::y,
			sol::meta_function::addition, add_overload,
			sol::meta_function::subtraction, sub_overload,
			sol::meta_function::multiplication, mult_overload,
			sol::meta_function::division, div_overload,
			sol::meta_function::unary_minus, unary_minus,
			sol::meta_function::equal_to, equal_to
		);
	}

	void RegisterVec3(sol::state& state)
	{
		auto add_overload = sol::overload(
			[](const Math::vec3& v1, const Math::vec3& v2) -> Math::vec3 { return v1 + v2; },
			[](const Math::vec3& v, float f) -> Math::vec3 { return v + f; },
			[](float f, const Math::vec3& v) -> Math::vec3 { return f + v; }
		);

		auto sub_overload = sol::overload(
			[](const Math::vec3& v1, const Math::vec3& v2) -> Math::vec3 { return v1 - v2; },
			[](const Math::vec3& v, float f) -> Math::vec3 { return v - f; },
			[](float f, const Math::vec3& v) -> Math::vec3 { return f - v; }
		);

		auto mult_overload = sol::overload(
			[](const Math::vec3& v1, const Math::vec3& v2) -> Math::vec3 { return v1 * v2; },
			[](const Math::vec3& v, float f) -> Math::vec3 { return v * f; },
			[](float f, const Math::vec3& v) -> Math::vec3 { return f * v; }
		);

		auto div_overload = sol::overload(
			[](const Math::vec3& v1, const Math::vec3& v2) -> Math::vec3 { return v1 / v2; },
			[](const Math::vec3& v, float f) -> Math::vec3 { return v / f; },
			[](float f, const Math::vec3& v) -> Math::vec3 { return f / v; }
		);

		auto unary_minus = [](const Math::vec3& v) -> Math::vec3 { return -v; };
		auto equal_to = [](const Math::vec3& v1, const Math::vec3& v2) -> Math::vec3 { return Math::equal(v1, v2); };

		state.new_usertype<Math::vec3>("vec3",
			sol::constructors<Math::vec3(), Math::vec3(float, float, float)>(),
			"x", &Math::vec3::x,
			"y", &Math::vec3::y,
			"z", &Math::vec3::z,
			"Rotate", &Math::rotateVec,
			sol::meta_function::addition, add_overload,
			sol::meta_function::subtraction, sub_overload,
			sol::meta_function::multiplication, mult_overload,
			sol::meta_function::division, div_overload,
			sol::meta_function::unary_minus, unary_minus,
			sol::meta_function::equal_to, equal_to
		);
	}

	void RegisterQuat(sol::state& state)
	{
		state.new_usertype<Math::quat>("quat",
			sol::constructors<
				Math::quat(),
				Math::quat(float, float, float, float)
			>(),
			"x", &Math::quat::x,
			"y", &Math::quat::y,
			"z", &Math::quat::z,
			"w", &Math::quat::w
		);
	}

//--------------------------------------------------------------------------------
// Entity Component System
//--------------------------------------------------------------------------------
	void RegisterRenderableComponent(sol::state& state)
	{
		state.new_usertype<RenderableComponent>("render",
			sol::constructors<RenderableComponent()>(),
			"active", &RenderableComponent::active
		);
	}

	void RegisterTransformComponent(sol::state& state)
	{
		state.new_usertype<TransformComponent>("transform",
			sol::constructors<TransformComponent()>(),
			"translation", &TransformComponent::translation,
			"rotation", &TransformComponent::rotation,
			"scale", &TransformComponent::scale
		);
	}

	void RegisterEntity(sol::state& state)
	{
		state.new_usertype<Entity>("entity",
			sol::constructors<Entity(entt::entity, Scene*)>(),
			"GetTransform", &Entity::GetComponent<TransformComponent>,
			"GetRenderable", &Entity::GetComponent<RenderableComponent>
		);
	}

//--------------------------------------------------------------------------------
// Application
//--------------------------------------------------------------------------------
	void RegisterApplication(sol::state& state)
	{
		auto terminate = []() {
			Application::Instance().Terminate();
		};

		state.new_usertype<Application>("app",
			sol::no_constructor,
			"Terminate", terminate
		);
	}
}
