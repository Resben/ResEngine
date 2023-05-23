/**
 * \file
 * \author Christien Alden (34119981)
*/
#include "FSMGraph.h"
#include "AEngine/Core/Types.h"
#include <map>
#include <sstream>
#include <stdexcept>

namespace AEngine
{
	FSMGraph::FSMGraph(std::vector<FSMState> states, int initialState)
		: m_states{ states }, m_currentState{ initialState }
	{
		if (m_states.size() == 0)
		{
			throw std::invalid_argument("FSMGraph must have at least one state");
		}
	}

	void FSMGraph::Init()
	{
		GetCurrentState().OnEntry();
	}

	FSMState& FSMGraph::GetCurrentState()
	{
		return m_states[m_currentState];
	}

	bool FSMGraph::GoToState(int state, bool force)
	{
		if (state == m_currentState)
		{
			return false;
		}

		// only check valid transition if not forcing
		if (!force && !m_states[m_currentState].HasTransition(state))
		{
			return false;
		}

		// update states
		m_states[m_currentState].OnExit();
		m_states[state].OnEntry();
		m_currentState = state;
		return true;
	}
}
