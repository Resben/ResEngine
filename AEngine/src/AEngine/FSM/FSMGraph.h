#pragma once
#include <initializer_list>
#include <vector>
#include "FSMState.h"

namespace AEngine
{
	class FSMGraph
	{
	public:
			/**
			 * \brief Construct a new FSMGraph object
			 * \param[in] states The states in the FSM
			 * \param[in] initialState The initial state
			 * \note The \p initialState parameter is optional and defaults to 0
			 * \warning The states vector should not be empty
			 * \throw std::invalid_argument if the states vector is empty
			*/
		FSMGraph(std::initializer_list<FSMState> states, int initialState = 0);
		FSMGraph(std::vector<FSMState> states, int initialState = 0);
			/**
			 * \brief Destroy the FSMGraph object
			*/
		~FSMGraph() = default;
			/**
			 * \brief Runs the onEntry function of the initial state
			*/
		void Init();
			/**
			 * \brief Gets the current FSM state
			 * \return The current fsm state
			*/
		FSMState& GetCurrentState();
			/**
			 * \brief Go to the given state
			 * \param[in] state The state to go to
			 * \param[in] force Whether to force the switch
			 * \retval true The state was changed
			 * \retval false The state was not changed
			 * \note The \p force parameter is optional and defaults to false
			 * \note If \p force is true, the state will be changed even if the current state does not transition to the given state
			*/
		bool GoToState(int state, bool force = false);

		std::string AnalyseTransitions() const;

	private:
		std::vector<FSMState> m_states;
		int m_currentState;
	};
}
