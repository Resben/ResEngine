#pragma once
#include <functional>
#include <set>
#include <string>
#include "AEngine/Core/TimeStep.h"

namespace AEngine
{
	class FSMState
	{
	public:
			/**
			 * \brief Construct a new FSMState object
			*/
		FSMState();
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
			/**
			 * \brief Destroy the FSMState object
			*/
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
		const std::string m_name;
		std::set<int> m_transitions;

		std::function<void()> m_onEntry;
		std::function<int(TimeStep)> m_onUpdate;
		std::function<void()> m_onExit;
	};
}
