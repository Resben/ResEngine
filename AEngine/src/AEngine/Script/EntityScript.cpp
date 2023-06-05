#include "EntityScript.h"
#include "ScriptEngine.h"
#include "AEngine/Scene/Components.h"
#include "AEngine/Scene/Scene.h"
#include "AEngine/Scene/Entity.h"

namespace AEngine
{
	EntityScript::EntityScript(Entity& entity, ScriptState& state, const Script* script)
		: m_env(state), m_script(script)
	{
		m_env.LoadScript(m_script->GetData());
		SetLocal("entity", entity);
		OnStart();
	}

	EntityScript::~EntityScript()
	{
		OnDestroy();
	}

	void EntityScript::OnStart()
	{
		m_env.CallFunction("OnStart");
	}

	void EntityScript::OnUpdate(float deltaTime)
	{
		m_env.CallFunction("OnUpdate", deltaTime);
	}

	void EntityScript::OnFixedUpdate(float deltaTime)
	{
		m_env.CallFunction("OnFixedUpdate", deltaTime);
	}

	void EntityScript::OnLateUpdate(float deltaTime)
	{
		m_env.CallFunction("OnLateUpdate", deltaTime);
	}

	void EntityScript::OnDestroy()
	{
		m_env.CallFunction("OnDestroy");
	}

	const std::string& EntityScript::GetIdent() const
	{
		return m_script->GetIdent();
	}

	const std::string& EntityScript::GetPath() const
	{
		return m_script->GetPath();
	}
}
