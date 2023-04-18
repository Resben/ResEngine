#include "AEngine/Script/ScriptState.h"

ScriptState::ScriptState()
{
	m_state.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string);
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
