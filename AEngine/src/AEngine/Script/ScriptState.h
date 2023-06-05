#pragma once
#include <sol/sol.hpp>

namespace AEngine
{
		/**
		 * @class ScriptState
		 * @brief ScriptState Class basic Scripting Engine
		 * @author Geoff Candy (34183006)
		 * @version 01
		 * @date 19/04/2023
		**/
	class ScriptState
	{
	public:
			/**
			 * @brief Default Constructor
			 **/
		ScriptState();

			/**
			 * \brief Method to load a file into the script state
			 * \param[in] path The path to the file to load
			 */
		void LoadFile(const std::string& path);

		void LoadScript(const std::string& script);

			/**
			 * @brief Method to call functions within script
			 * @param[in] functionName The name of the function to call
			 **/
		void CallFunction(const std::string& functionName);

			/**
			 * @brief Templated method to call functions within script
			 * @param std::string, <T> Args
			 **/
		template<typename... Args>
		void CallFunction(const std::string& functionName, Args&&... args);

			/**
			 * \brief Method to get the native state
			 * \return sol::state&
			 */
		sol::state& GetNative();

	private:
		sol::state m_state;
	};

		//templated function to call functions within lua
	template<typename ...Args>
	inline void ScriptState::CallFunction(const std::string& functionName, Args && ...args)
	{
		sol::protected_function function = m_state[functionName];
		if (function.valid())
		{
			function(std::forward<Args>(args)...);
		}
	}
}
