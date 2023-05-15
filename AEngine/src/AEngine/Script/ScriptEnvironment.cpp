#include "ScriptEnvironment.h"
#include "ScriptState.h"

namespace AEngine
{
	ScriptEnvironment::ScriptEnvironment(ScriptState& state)
		: m_state(state.GetNative()), m_environment(m_state, sol::create, m_state.globals())
	{
		
	}

	void ScriptEnvironment::LoadFile(const std::string& path)
	{
		m_state.script_file(path, m_environment);
	}

	void ScriptEnvironment::LoadScript(const std::string &script)
	{
		m_state.script(script, m_environment);
	}

	void ScriptEnvironment::CallFunction(const std::string& functionName)
	{
		sol::protected_function function = m_environment[functionName];
		if (function.valid())
		{
			function();
		}
	}
}
