/**
 * \file
 * \brief The script engine public API
 * \author Christien Alden (34119981)
*/
#pragma once
#include <string>

namespace AEngine
{
	class ScriptState;

		/**
		 * \brief The script engine public API
		 * \author Christien Alden (34119981)
		*/
	class ScriptEngine
	{
	public:
			/**
			 * \brief Initializes the script engine
			*/
		static void Init();
			/**
			 * \brief Loads a script file
			 * \param[in] path The path to the script file
			*/
		static void LoadFile(const std::string &path);
			/**
			 * \brief Loads a script from a string
			 * \param[in] script The script to load
			*/
		static void LoadScript(const std::string &script);
			/**
			 * \brief Shuts down the script engine
			 * \todo Implement ScriptEngine::Shutdown
			*/
		static void Shutdown();
			/**
			 * \brief Gets the state of the script engine
			 * \return The state of the script engine
			*/
		static ScriptState& GetState();
	};
}
