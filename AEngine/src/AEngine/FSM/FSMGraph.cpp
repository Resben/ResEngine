#include <map>
#include <stdexcept>
#include <sstream>
#include "FSMGraph.h"
#include "AEngine/Core/Types.h"

namespace AEngine
{
	FSMGraph::FSMGraph(std::initializer_list<FSMState> states, int initialState)
		: m_states(states), m_currentState(initialState)
	{
		if (m_states.size() == 0)
		{
			throw std::invalid_argument("FSMGraph must have at least one state");
		}
	}

	FSMGraph::FSMGraph(std::vector<FSMState> states, int initialState)
		: m_states(states), m_currentState(initialState)
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

	//--------------------------------------------------------------------------------
	// Analysis
	//--------------------------------------------------------------------------------
	struct StateInfo
	{
		std::set<int> incoming;
		std::set<int> outgoing;
	};


	std::string FSMGraph::AnalyseTransitions() const
	{
		// generate state transition info
		std::map<int, StateInfo> stateInfo;
		for (Size_t si = 0; si < m_states.size(); ++si)
		{
			for (int ti : m_states[si].m_transitions)
			{
				stateInfo[si].outgoing.insert(ti);
				stateInfo[ti].incoming.insert(si);
			}
		}

		std::stringstream ss;

		for (auto& [si, info] : stateInfo)
		{
			ss << "State " << si
			<< "\n\t Incoming: ";
				for (int index : stateInfo[si].incoming)
				{
					ss << index << " ";
				}
			ss << "\n\t Outgoing: ";
				for (int index : stateInfo[si].outgoing)
				{
					ss << index << " ";
				}
			ss << '\n';
		}

		return ss.str();
	}
}
