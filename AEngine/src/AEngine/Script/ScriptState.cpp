#include "ScriptState.h"
#include "AEngine/Core/Logger.h"

namespace AEngine
{
//--------------------------------------------------------------------------------
// ScriptState
//--------------------------------------------------------------------------------
	ScriptState::ScriptState()
	{
		m_state.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math);
		sol::protected_function::set_default_handler(sol::make_reference(m_state, [](sol::error error){
			AE_LOG_LUA_ERROR("{}", error.what());
		}));
	}

	void ScriptState::LoadFile(const std::string& path)
	{
		m_state.script_file(path);
	}

	void ScriptState::LoadScript(const std::string & script)
	{
		m_state.script(script);
	}

	void ScriptState::CallFunction(const std::string& functionName)
	{
		sol::protected_function function = m_state[functionName];
		if (function.valid())
		{
			function();
		}
	}

	sol::state& ScriptState::GetNative()
	{
		return m_state;
	}
}
