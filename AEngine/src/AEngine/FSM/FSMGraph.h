/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once
#include "FSMState.h"
#include <initializer_list>
#include <vector>

namespace AEngine
{
		/**
		 * \class FSMGraph
		 * \brief Holds the internal transition logic for the FSM
		 * \note This should not be used directly
		*/
	class FSMGraph
	{
	public:
			/**
			 * \param[in] states The states in the FSM
			 * \param[in] initialState The initial state
			 * \note The \p initialState parameter is optional and defaults to 0
			 * \throw std::invalid_argument if the states vector is empty
			*/
		FSMGraph(std::vector<FSMState> states, int initialState = 0);
		~FSMGraph() = default;

			/**
			 * \copydoc FSM::Init()
			*/
		void Init();
			/**
			 * \brief Gets the current FSM state
			 * \return The current fsm state
			*/
		FSMState& GetCurrentState();
			/**
			 * \brief Gets the current FSM state
			 * \return The current fsm state
			*/
		int GetCurrentStateIndex() const;

			/**
			 * \brief Go to the given state
			 * \param[in] state The state to go to
			 * \param[in] force Whether to force the switch
			 * \retval true The state was changed
			 * \retval false The state was not changed
			 * \note The \p force parameter is optional and defaults to false
			 * \note If \p force is true, the state will be changed even if the current state does not transition to the given state
			*/
		bool GoToState(int nextState, bool force = false);

	private:
			/**
			 * \brief Holds all the states in the FSM
			*/
		std::vector<FSMState> m_states;
			/**
			 * \brief The current state of the FSM
			*/
		int m_currentState;
			/**
			 * \brief The previous state of the FSM
			*/
		int m_previousState;
	};
}
