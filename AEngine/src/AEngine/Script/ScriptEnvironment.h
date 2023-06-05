#pragma once
#include <sol/sol.hpp>

namespace AEngine
{
	class ScriptState;

	class ScriptEnvironment
	{
	public:
		ScriptEnvironment(ScriptState& state);

					/**
			 * \brief Method to load a file into the script state
			 * \param[in] path The path to the file to load
			 */
		void LoadFile(const std::string& path);

			/**
			 * \brief Method to load a script into the script state
			 * \param[in] script The script to load
			*/
		void LoadScript(const std::string& script);

			/**
			 * @brief Method to call functions within script
			 * @param[in] functionName The name of the function to call
			 **/
		void CallFunction(const std::string& functionName);

		template <typename T>
		void SetLocal(const std::string& name, T value);

			/**
			 * @brief Templated method to call functions within script
			 * @param std::string, <T> Args
			 **/
		template<typename... Args>
		void CallFunction(const std::string& functionName, Args&&... args);

	private:
		sol::state& m_state;
		sol::environment m_environment;
	};

		// templated function to set local variables within lua
	template<typename T>
	inline void ScriptEnvironment::SetLocal(const std::string& name, T value)
	{
		m_environment.set(name, value);
	}

		//templated function to call functions within lua
	template<typename ...Args>
	inline void ScriptEnvironment::CallFunction(const std::string& functionName, Args && ...args)
	{
		sol::protected_function function = m_environment[functionName];
		if (function.valid())
		{
			function(std::forward<Args>(args)...);
		}
	}
}
