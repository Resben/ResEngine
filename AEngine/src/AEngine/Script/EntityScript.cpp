#include "EntityScript.h"
#include "ScriptEngine.h"
#include "AEngine/Scene/Components.h"
#include "AEngine/Scene/Entity.h"
#include "AEngine/Scene/Scene.h"

namespace AEngine
{
	EntityScript::EntityScript(ScriptState& state, const std::string& ident, const std::string& fname) 
		: Asset(ident, fname), m_env(state)
	{
		m_env.LoadFile(fname);
		OnStart();
	}

	void EntityScript::OnStart()
	{
		m_env.CallFunction("OnStart");
	}

	void EntityScript::OnUpdate(float deltaTime, Entity entity)
	{
		m_env.CallFunction("OnUpdate", deltaTime, entity);
	}

	void EntityScript::OnDestroy()
	{
		m_env.CallFunction("OnDestroy");
	}
	
	SharedPtr<EntityScript> EntityScript::Create(const std::string& ident, const std::string& fname)
	{
		return MakeShared<EntityScript>(ScriptEngine::Instance().GetState(), ident, fname);
	}
}
