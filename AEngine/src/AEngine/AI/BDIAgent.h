#pragma once
#include "Predicate.h"
#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace AEngine
{
		/**
		 * \class BDIAgent
		 * \brief BDI Model of an intelligent agent
		 * \details
		 * This class implements the BDI model of an intelligent agent. It is a
		 * simple implementation that uses strings to represent beliefs, desires,
		 * and intentions. The agent will evaluate the predicates of its desires
		 * to determine which one to activate. The agent will then evaluate the
		 * predicates of its intentions to determine which one to activate, if any.
		*/
	class BDIAgent
	{
	public:
		using Concept = std::pair<std::string, float>;

	public:
			/**
			 * \brief Construct an agent
			 * \param[in] debugName The name of the agent, used for debugging
			*/
		BDIAgent(const std::string& debugName);

			/**
			 * \brief Get the name of the agent
			 * \returns The name of the agent
			*/
		const std::string& GetName() const;

			/**
			 * \brief Updates the internal state of the agent
			 * \details
			 * Call this method when you want the agent to update its beliefs,
			 * desires, and intentions. This will also call the action of the
			 * highest priority intention, if it is higher than the current
			 * activation level.
			*/
		void OnUpdate();
			/**
			 * \brief Manually sets the activation level of the agent
			 * \param[in] level The new activation level, must be >= 0.0f.
			 * \details
			 * An \p level of 0.0f will mean any intention can be activated, while
			 * an \p level of 1.0f will mean only the highest priority intention can
			 * be activated.
			*/
		void SetActivationLevel(float level);
			/**
			 * \brief Get the activation level of the agent
			 * \returns The activation level
			*/
		float GetActivationLevel() const;
			/**
			 * \brief Sets the activation threshold of the agent
			 * \param[in] threshold The new activation threshold, must be >= 0.0f.
			 * \details
			 * The activation threshold determines the activation level required to
			 * activate an intention over the current action. The value is determined
			 * by the current activation level multiplied by the threshold.\n
			 * For example:\n
			 * If the activation level is 0.5 and the threshold is 1.2, then the
			 * minimum activation level required to activate an intention is 0.6.
			 * The purpose of this is to prevent the agent from switching between
			 * actions too often.
			 * \note
			 * This does not force a reevaluation of the agent's desires and
			 * intentions. Instead, it will wait until the next update to reevaluate
			 * them.
			*/
		void SetIntentionThreshold(float threshold);
			/**
			 * \brief Get the activation threshold of the agent
			 * \returns The activation threshold
			*/
		float GetIntentionThreshold() const;
			/**
			 * \brief Add a belief to the agent
			 * \param[in] belief The belief to add
			 * \returns True if the belief was added, false otherwise
			*/
		bool AddBelief(const std::string& belief);

			/**
			 * \brief Remove a belief from the agent
			 * \param[in] belief The belief to remove
			 * \returns True if the belief was removed, false otherwise
			*/
		bool RemoveBelief(const std::string& belief);

			/**
			 * \brief Add a desire to the agent
			 * \param[in] desire The desire to add
			 * \param[in] predicate The predicate that activates the desire
			 * \param[in] priority The priority of the desire, higher is more urgent
			 * \returns True if the desire was added, false otherwise
			 * \note An invalid predicate will cause the desire to not be added
			*/
		bool AddDesire(
			const std::string& desire,
			const std::string& predicate,
			float priority
		);

			/**
			 * \brief Remove a desire from the agent
			 * \param[in] desire The desire to remove
			 * \returns True if the desire was removed, false otherwise
			*/
		bool RemoveDesire(const std::string& desire);

			/**
			 * \brief Add an intention to the agent
			 * \param[in] intention The intention to add
			 * \param[in] predicate The predicate that activates the intention
			 * \returns True if the intention was added, false othewise
			 * \note An invalid predicate will cause the intention to not be added
			*/
		bool AddIntention(
			const std::string& intention,
			const std::string& predicate,
			const std::function<void(const std::string&)>& action
		);

			/**
			 * \brief Remove an intention from the agent
			 * \param[in] intention The intention to remove
			 * \returns True if the intention was removed, false otherwise
			*/
		bool RemoveIntention(const std::string& intention);

			/**
			 * \brief Get the beliefs of the agent
			 * \returns The beliefs of the agent
			*/
		const std::set<std::string>& GetBeliefs() const;
			/**
			 * \brief Get the active desires of the agent
			 * \returns The active desires of the agent
			*/
		const std::vector<Concept>& GetActiveDesires() const;
			/**
			 * \brief Get the active intentions of the agent
			 * \returns The active intentions of the agent
			*/
		const std::vector<Concept>& GetActiveIntentions() const;
			/**
			 * \brief Get the potential desires of the agent
			 * \returns The potential desires of the agent
			 * \note
			 * This is used for debugging purposes. It is not recommended to use
			 * this in a production environment.
			*/
		std::vector<Concept> GetPotentialDesires() const;
			/**
			 * \brief Get the potential intentions of the agent
			 * \returns The potential intentions of the agent
			 * \note
			 * This is used for debugging purposes. It is not recommended to use
			 * this in a production environment.
			*/
		std::vector<std::string> GetPotentialIntentions() const;

	private:
		struct Desire
		{
			Predicate predicate;
			float priority{ 0.5f };

			Desire() = default;
			Desire(
				const std::string& predicate,
				float priority
			) : predicate{ predicate }, priority{ priority } {}
		};

		struct Intention
		{
			Predicate predicate;
			std::function<void(const std::string&)> action{ nullptr };

			Intention() = default;
			Intention(
				const std::string& predicate,
				const std::function<void(const std::string&)>& action
			) : predicate{ predicate }, action{ action } {}
		};

	private:
		const std::string m_debugName;   ///< The name of the agent, used for debugging
		float m_activationLevel;         ///< The activation level of the current action
		float m_intentionThreshold;     ///< A multiplier for the activation level required to activate an intention
		bool m_shouldReevaluate;         ///< True if the agent needs to refresh its beliefs, desires, and intentions

		std::map<std::string, Desire> m_potentialDesires;         ///< The potential desires of the agent
		std::map<std::string, Intention> m_potentialIntentions;   ///< The potential intentions of the agent

			/**
			 * \brief The currently held beliefs of the agent
			 * \details
			 * There are simple strings that are updated and removed as the agent
			 * learns new information. Normally these are set externally, but there
			 * is also the option to set them within one of the agent's actions.
			*/
		std::set<std::string> m_beliefs;

			/**
			 * \defgroup Sorted BDI structures
			 * \brief Used to sort the desires and intentions of the agent, into descending order
			 * \details
			 * The maps are used to determine which desire or intention to activate.
			 * The vectors are used to sort the maps; the highest valued intention
			 * is then chosen to be activated.
			*/
		std::map<std::string, float> m_activeDesires;      ///< \ingroup Sorted BDI structures
		std::map<std::string, float> m_activeIntentions;   ///< \ingroup Sorted BDI structures
		std::vector<Concept> m_sortedDesires;          ///< \ingroup Sorted BDI structures
		std::vector<Concept> m_sortedIntentions;       ///< \ingroup Sorted BDI structures

		static int s_maxRecursionLevel;   ///< The maximum level of recursion for evaluating expressions

	private:
			/**
			 * \brief Evaluate an expression
			 * \param[in] expr The expression to evaluate
			 * \param[in] level The current level of recursion
			 * \param[in] recurseBeliefs True if the agent should recurse beliefs
			 * \returns The weight of the predicate expression
			 * \note
			 * This function is recursive and will throw an exception if the
			 * recursion level exceeds the maximum recursion level.
			*/
		float EvaluateExpression(const Predicate::Expr* expr, int level, bool recurseBeliefs = false) const;
	};
}
