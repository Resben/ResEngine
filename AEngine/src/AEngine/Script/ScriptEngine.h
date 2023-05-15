#pragma once
#include "ScriptState.h"

namespace AEngine
{
	class ScriptEngine
	{
	public:
		static ScriptEngine& Instance();
		void Init();
		void LoadFile(const std::string &path);
		void LoadScript(const std::string &script);
		void Shutdown();

		ScriptState& GetState();

	private:
		ScriptEngine() = default;
		~ScriptEngine() = default;

		ScriptState m_state;
	};
}
