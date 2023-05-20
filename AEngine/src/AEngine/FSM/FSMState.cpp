#include <fstream>
#include "FSMState.h"

namespace AEngine
{
	FSMState::FSMState()
		: m_name(""), m_transitions(), m_onUpdate(nullptr), m_onEntry(nullptr), m_onExit(nullptr)
	{
	}

	FSMState::FSMState(
		const std::string& name,
		std::set<int> transitions,
		std::function<int(TimeStep)> onUpdate,
		std::function<void()> onEntry,
		std::function<void()> onExit
	) : m_name(name),
		m_transitions(transitions),
		m_onUpdate(onUpdate), m_onEntry(onEntry), m_onExit(onExit)
	{

	}

	void FSMState::OnEntry()
	{
		if (!m_onEntry)
		{
			return;
		}

		m_onEntry();
	}

	int FSMState::OnUpdate(TimeStep deltaTime)
	{
		if (!m_onUpdate)
		{
			return -1;
		}

		return m_onUpdate(deltaTime);
	}

	void FSMState::OnExit()
	{
		if (!m_onExit)
		{
			return;
		}

		m_onExit();
	}

	bool FSMState::HasTransition(int state) const
	{
		return m_transitions.find(state) != m_transitions.end();
	}

	std::string FSMState::GetName() const
	{
		return m_name;
	}
}
