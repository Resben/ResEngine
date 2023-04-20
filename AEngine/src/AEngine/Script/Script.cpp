#include "Script.h"

#include "AEngine/Scene/Components.h"
#include "AEngine/Scene/Scene.h"
#include "AEngine/Scene/Entity.h"

namespace AEngine
{
	Script::Script(const std::string& ident, const std::string& fname) 
		: Asset(ident, fname), m_state()
	{
		m_state.LoadFile(fname);
		m_state.CallFunction("OnStart");
	}

	void Script::OnStart()
	{
		m_state.CallFunction("OnStart");
	}

	void Script::OnUpdate(float deltaTime, Entity entity)
	{
		m_state.CallFunction("OnUpdate", deltaTime, entity);
	}

	void Script::OnDestroy()
	{
		m_state.CallFunction("OnDestroy");
	}
	
	std::shared_ptr<Script> Script::Create(const std::string& ident, const std::string& fname)
	{
		return std::make_shared<Script>(ident, fname);
	}
}
