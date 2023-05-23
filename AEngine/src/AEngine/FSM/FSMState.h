/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once
#include "AEngine/Core/TimeStep.h"
#include <functional>
#include <set>
#include <string>

namespace AEngine
{
		/**
		 * \class FSMState
		 * \brief A state in a finite state machine
		*/
	class FSMState
	{
	public:
		FSMState() = default;
			/**
			 * \brief Construct a new FSMState object
			 * \param[in] name The name of the state
			 * \param[in] transitions The states that this state can transition to
			 * \param[in] onUpdate The function to call when the state is updated
			 * \param[in] onEntry The function to call when the state is entered
			 * \param[in] onExit The function to call when the state is exited
			 * \note The onUpdate function should return the state to transition to
			 * \note The onEntry and onExit functions are optional
			*/
		FSMState(
			const std::string& name,
			std::set<int> transitions,
			std::function<int(TimeStep)> onUpdate,
			std::function<void()> onEntry = nullptr,
			std::function<void()> onExit = nullptr
		);
		~FSMState() = default;

			/**
			 * \brief Call the onEntry function
			*/
		void OnEntry();
			/**
			 * \brief Call the onUpdate function
			 * \param[in] deltaTime The time since the last frame
			 * \return The state to transition to
			 * \note If the state does not transition, return the current state
			*/
		int OnUpdate(TimeStep deltaTime);
			/**
			 * \brief Call the onExit function
			*/
		void OnExit();

			/**
			 * \brief Check if the state can transition to the given state
			 * \param[in] state The state to check
			 * \retval true The state can transition to the given state
			 * \retval false The state cannot transition to the given state
			*/
		bool HasTransition(int state) const;
			/**
			 * \brief Get the name of the state
			 * \return The name of the state
			*/
		std::string GetName() const;

		friend class FSMGraph;

	private:
			/**
			 * \brief The name of the state
			*/
		const std::string m_name;
			/**
			 * \brief The states that this state can transition to
			*/
		std::set<int> m_transitions;

			/**
			 * \brief The function to call when the state is entered
			*/
		std::function<void()> m_onEntry;
			/**
			 * \brief The function to call when the state is updated
			*/
		std::function<int(TimeStep)> m_onUpdate;
			/**
			 * \brief The function to call when the state is exited
			*/
		std::function<void()> m_onExit;
	};
}
