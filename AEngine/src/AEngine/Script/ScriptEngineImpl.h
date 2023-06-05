#pragma once
#include "ScriptState.h"

namespace AEngine
{
		/**
		 * \brief The implementation of the script engine/
		 * \author Christien Alden (34119981)
		*/
	class ScriptEngineImpl
	{
	public:
			/**
			 * \brief Gets the singleton instance of the script engine
			 * \return The singleton instance of the script engine
			*/
		static ScriptEngineImpl& Instance();
			/**
			 * \copydoc ScriptEngine::Init
			*/
		void Init();
			/**
			 * \copydoc ScriptEngine::LoadFile
			*/
		void LoadFile(const std::string &path);
			/**
			 * \copydoc ScriptEngine::LoadScript
			*/
		void LoadScript(const std::string &script);
			/**
			 * \copydoc ScriptEngine::Shutdown
			*/
		void Shutdown();
			/**
			 * \copydoc ScriptEngine::GetState
			*/
		ScriptState& GetState();

	private:
		ScriptEngineImpl() = default;
		ScriptState m_state;
		bool m_isInitialized{ false };
	};
}
