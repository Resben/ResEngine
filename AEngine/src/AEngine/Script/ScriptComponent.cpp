#include "ScriptComponent.h"

ScriptComponent::ScriptComponent(const std::string& scriptFileName) : m_state()
{
	m_state.LoadFile(scriptFileName);
	m_state.CallFunction("OnStart");
}

void ScriptComponent::OnStart()
{
	m_state.CallFunction("OnStart");
}

void ScriptComponent::OnUpdate(float deltaTime)
{
	m_state.CallFunction("OnUpdate", deltaTime);
}

void ScriptComponent::OnDestroy()
{
	m_state.CallFunction("OnDestroy");
}

//void ScriptComponent::GetTransformComponent()
//{
//	//TODO
//}
