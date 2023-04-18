#pragma once

#include <sol/sol.hpp>
#include "AEngine/Core/KeyCodes.h"
#include "AEngine/Core/Input.h"

class ScriptState 
{
public:

	ScriptState();
	void LoadFile(const std::string& path);
	void CallFunction(const std::string& functionName);

	template<typename... Args>
	void CallFunction(const std::string& functionName, Args&&... args);

	sol::state& GetState();

	void Register();
private:
	sol::state m_state;
};

template<typename ...Args>
inline void ScriptState::CallFunction(const std::string& functionName, Args && ...args)
{
	sol::protected_function function = m_state[functionName];
	if (function.valid())
	{
		function(std::forward<Args>(args)...);
	}
}

