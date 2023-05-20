#pragma once
#include "AEngine/Core/TimeStep.h"
#include "FSMGraph.h"
#include "FSMState.h"
#include <initializer_list>

namespace AEngine
{
	class FSM
	{
	public:
			/**
			 * \brief Construct a new FSM object
			*/
		FSM() = default;
			/**
			 * \brief Construct a new FSM object
			 * \param[in] states The states in the FSM
			 * \param[in] initialState The initial state
			 * \note The \p initialState parameter is optional and defaults to 0
			*/
		FSM(std::initializer_list<FSMState> states, int initialState = 0);
		FSM(std::vector<FSMState> states, int initialState = 0);
			/**
			 * \brief Destroy the FSM object
			*/
		~FSM() = default;
			/**
			 * \brief Runs the onEntry function of the initial state
			*/
		void Init();
			/**
			 * \brief Updates the FSM object
			 * \param[in] deltaTime The time since the last frame
			*/
		void OnUpdate(TimeStep deltaTime);

		std::string AnalyseTransitions() const;

	private:
		FSMGraph m_graph;
	};
}
