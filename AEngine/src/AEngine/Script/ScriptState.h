

#pragma once

#include <sol/sol.hpp>
#include "AEngine/Core/KeyCodes.h"
#include "AEngine/Core/Input.h"

namespace AEngine
{
	bool PollMouseButton(AEMouse button);
	bool PollAlphaKey(AEKey key);
	bool PollNumKey(AEKey key);
	bool PollSpecialKey(AEKey key);
		
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
		 * @brief Method to load the script file
		 * @param std::string
		 **/
		void LoadFile(const std::string& path);

		/**
		 * @brief Method to call functions within script
		 * @param std::string
		 **/
		void CallFunction(const std::string& functionName);

		/**
		 * @brief Templated method to call functions within script
		 * @param std::string, <T> Args
		 **/
		template<typename... Args>
		void CallFunction(const std::string& functionName, Args&&... args);

		/**
		 * @brief Method to get the lua State
		 **/
		sol::state& GetState();


		/**
		 * @brief Method to Register Mouse Buttons
		 **/
		void RegisterMouseButtons();

		/**
		 * @brief Method to Register Alphabet Keys
		 **/
		void RegisterAlphaKeys();

		/**
		 * @brief Method to Register Number Keys
		 **/
		void RegisterNumKeys();

		/**
		 * @brief Method to Register Special Keys
		 **/
		void RegisterSpecialKeys();

		/**
		 * @brief Method to Register Helper functions
		 **/
		void RegisterFunctions();

		/**
		 * @brief Method to Register Vector3 as a user type within lua
		 **/
		void RegisterVec3();

		/**
		 * @brief Method to Register Vector2 as a user type with lua
		 **/
		void RegisterVec2();

		/**
		 * @brief Method to Register an Entity as a user type with lua
		 **/
		void RegisterEntity();

		/**
		 * @brief Method to Register a transform as a user type with lua
		 **/
		void RegisterTransform();

		/**
		 * @brief Method to Register a renderable component with lua
		 **/
		void RegisterRenderable();

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


