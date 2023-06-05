#include "ScriptEngine.h"
#include "ScriptEngineImpl.h"

namespace AEngine
{
	void ScriptEngine::Init()
	{
		ScriptEngineImpl::Instance().Init();
	}

	void ScriptEngine::LoadFile(const std::string& path)
	{
		ScriptEngineImpl::Instance().LoadFile(path);
	}

	void ScriptEngine::LoadScript(const std::string &script)
	{
		ScriptEngineImpl::Instance().LoadScript(script);
	}

	void ScriptEngine::Shutdown()
	{
		ScriptEngineImpl::Instance().Shutdown();
	}

	ScriptState &ScriptEngine::GetState()
	{
		return ScriptEngineImpl::Instance().GetState();
	}
}
