#include "ScriptComponent.h"

#include "AEngine/Scene/Components.h"
#include "AEngine/Scene/Scene.h"
#include "AEngine/Scene/Entity.h"

namespace AEngine
{
	ScriptComponent::ScriptComponent(const std::string& scriptFileName) : m_state()
	{
		m_state.LoadFile(scriptFileName);
		m_state.CallFunction("OnStart");
	}

	void ScriptComponent::OnStart()
	{
		m_state.CallFunction("OnStart");
	}

	void ScriptComponent::OnUpdate(float deltaTime, Entity entity)
	{
		m_state.CallFunction("OnUpdate", deltaTime, entity);
	}

	void ScriptComponent::OnDestroy()
	{
		m_state.CallFunction("OnDestroy");
	}
}
