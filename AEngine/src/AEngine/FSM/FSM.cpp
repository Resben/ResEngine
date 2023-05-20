#include "FSM.h"

namespace AEngine
{
	FSM::FSM(std::initializer_list<FSMState> states, int initialState)
		: m_graph(states, initialState)
	{

	}

	FSM::FSM(std::vector<FSMState> states, int initialState)
		: m_graph(states, initialState)
	{

	}

	void FSM::Init()
	{
		m_graph.Init();
	}

	void FSM::OnUpdate(TimeStep deltaTime)
	{
		int nextState = m_graph.GetCurrentState().OnUpdate(deltaTime);
		m_graph.GoToState(nextState);
	}

	std::string FSM::AnalyseTransitions() const
	{
		return m_graph.AnalyseTransitions();
	}
}
