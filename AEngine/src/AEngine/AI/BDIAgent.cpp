#include "BDIAgent.h"
#include "Predicate.h"
#include <algorithm>
#include <stdexcept>

namespace AEngine
{
	int BDIAgent::s_maxRecursionLevel = 100;

	BDIAgent::BDIAgent(const std::string& debugName)
		: m_debugName{ debugName },
		  m_activationLevel{ 0.0f },
		  m_intentionThreshold{ 1.20f },
		  m_shouldReevaluate{ true }
	{

	}

	const std::string &BDIAgent::GetName() const
	{
		return m_debugName;
	}

	void BDIAgent::OnUpdate()
	{
		// only reevaluate if necessary
		if (!m_shouldReevaluate)
		{
			return;
		}

		// re-evaluate the desires
		m_activeDesires.clear();
		for (auto [name, desire] : m_potentialDesires)
		{
			// we need to recurse the desires, because a desire may rely on other desires
			// could muliply this value by the priority of the desire
			if (EvaluateExpression(desire.predicate.GetExprTree(), 0, true) > 0.0f)
			{
				m_activeDesires[name] = desire.priority;
			}
		}

		// re-evaluate the intentions
		m_activeIntentions.clear();
		for (auto [name, intention] : m_potentialIntentions)
		{
			float value = EvaluateExpression(intention.predicate.GetExprTree(), 0);
			if (value > 0.0f)
			{
				m_activeIntentions[name] = value;
			}
		}

		// sort the active desires by priority
		m_sortedDesires.clear();
		for (auto [name, priority] : m_activeDesires)
		{
			m_sortedDesires.push_back({ name, priority });
		}
		std::sort(m_sortedDesires.begin(), m_sortedDesires.end(),
			[](const auto& left, const auto& right) {
				return left.second > right.second;
		});

		// sort the active intentions by weight
		m_sortedIntentions.clear();
		for (auto [name, weight] : m_activeIntentions)
		{
			m_sortedIntentions.push_back({ name, weight });
		}
		std::sort(m_sortedIntentions.begin(), m_sortedIntentions.end(),
			[](const auto& left, const auto& right) {
				return left.second > right.second;
		});

		// reset the reevaluate flag
		m_shouldReevaluate = false;

		// get the first intention, this is the one with the highest activation level
		auto it = m_sortedIntentions.begin();
		if (it != m_sortedIntentions.end())
		{
			// if the activation level of the first intention is greater than the
			// current activation level, then we need to call the action of the first
			// intention, otherwise we do nothing and wait for the next update
			if (it->second > m_activationLevel * m_intentionThreshold)
			{
				// set the activation level to the weight of the first intention
				m_activationLevel = it->second;

				// call the action of the first intention
				m_potentialIntentions[it->first].action(m_debugName);
			}
		}
	}

	void BDIAgent::SetActivationLevel(float level)
	{
		m_activationLevel = level;
		m_shouldReevaluate = true;
	}

	float BDIAgent::GetActivationLevel() const
	{
		return m_activationLevel;
	}

	void BDIAgent::SetIntentionThreshold(float threshold)
	{
		m_intentionThreshold = threshold;
	}

	float BDIAgent::GetIntentionThreshold() const
	{
		return m_intentionThreshold;
	}

	bool BDIAgent::AddBelief(const std::string& belief)
	{
		// Don't add empty beliefs
		if (belief.empty())
		{
			return false;
		}

		auto [_, inserted] = m_beliefs.emplace(belief);
		if (inserted)
		{
			m_shouldReevaluate = true;
			return true;
		}

		return false;
	}

	bool BDIAgent::RemoveBelief(const std::string& belief)
	{
		if (belief.empty())
		{
			return false;
		}

		for (auto it = m_beliefs.begin(); it != m_beliefs.end(); ++it)
		{
			if (*it == belief)
			{
				m_beliefs.erase(it);
				m_shouldReevaluate = true;
				return true;
			}
		}

		return false;
	}

	bool BDIAgent::AddDesire(
		const std::string& desire,
		const std::string& predicate,
		float priority)
	{
		if (desire.empty())
		{
			return false;
		}


		auto [_, inserted] = m_potentialDesires.emplace(desire, Desire{ predicate, priority });
		if (inserted)
		{
			m_shouldReevaluate = true;
			return true;
		}

		return false;
	}

	bool BDIAgent::RemoveDesire(const std::string& desire)
	{
		if (desire.empty())
		{
			return false;
		}

		for (auto it = m_potentialDesires.begin(); it != m_potentialDesires.end(); ++it)
		{
			if (it->first == desire)
			{
				m_potentialDesires.erase(it);
				m_shouldReevaluate = true;
				return true;
			}
		}

		return false;
	}

	bool BDIAgent::AddIntention(
		const std::string& intention,
		const std::string& predicate,
		const std::function<void(const std::string&)>& action)
	{
		if (intention.empty())
		{
			return false;
		}

		auto [_, inserted] = m_potentialIntentions.emplace(intention, Intention{ predicate, action });
		if (inserted)
		{
			m_shouldReevaluate = true;
			return true;
		}

		return false;
	}

	bool BDIAgent::RemoveIntention(const std::string& intention)
	{
		if (intention.empty())
		{
			return false;
		}

		for (auto it = m_potentialIntentions.begin(); it != m_potentialIntentions.end(); ++it)
		{
			if (it->first == intention)
			{
				m_potentialIntentions.erase(it);
				m_shouldReevaluate = true;
				return true;
			}
		}

		return false;
	}

	const std::set<std::string> &BDIAgent::GetBeliefs() const
	{
		return m_beliefs;
	}

	const std::vector<BDIAgent::Concept>& BDIAgent::GetActiveDesires() const
	{
		return m_sortedDesires;
	}

	const std::vector<BDIAgent::Concept>& BDIAgent::GetActiveIntentions() const
	{
		return m_sortedIntentions;
	}

	std::vector<BDIAgent::Concept> BDIAgent::GetPotentialDesires() const
	{
		std::vector<Concept> desires;
		for (auto [name, desire] : m_potentialDesires)
		{
			desires.push_back({ name, desire.priority });
		}

		return desires;
	}

	std::vector<std::string> BDIAgent::GetPotentialIntentions() const
	{
		std::vector<std::string> intentions;
		for (auto [name, intention] : m_potentialIntentions)
		{
			intentions.push_back(name);
		}

		return intentions;
	}

	float BDIAgent::EvaluateExpression(const Predicate::Expr* expr, int level, bool recurseDesires) const
	{
		if (level > s_maxRecursionLevel)
		{
			throw std::runtime_error("Predicate expression is too deep");
		}

		// base-case, terms represent the beliefs and desires of the agent
		// beliefs are a simple lookup in the beliefs set
		// desires are either a simple lookup in the activeDesires set, or a recursive
		// evaluation of the desire's predicate
		if (auto term = dynamic_cast<const Predicate::Term*>(expr))
		{
			switch (term->type)
			{
			case Predicate::TermType::Belief:
				// base-case for beliefs
				// should return 1.0f if the belief exists, 0.0f otherwise
				return {
					m_beliefs.find(term->value) != m_beliefs.end() ?
					1.0f : 0.0f
				};
			case Predicate::TermType::Desire:
				// base-case for desires
				if (!recurseDesires)
				{
					// find the desire in the activeDesires set
					auto it = std::find_if(m_activeDesires.begin(), m_activeDesires.end(),
						[term](const auto& pair) {
							return pair.first == term->value;
					});
					if (it != m_activeDesires.end())
					{
						// if the desire exists, return its weight
						return it->second;
					}
					else
					{
						// otherwise, return 0.0f
						return 0.0f;
					}
				}
				else
				{
					// recursively evaluate the desire, this is because the desire could rely
					// on other desires, which have not yet been evaluated, but may have
					// evaluated to true
					return EvaluateExpression(term, level + 1, recurseDesires);
				}
			default:
				throw std::runtime_error("Unknown term type");
			}
		}

		// recursive case, evaluate binary expressions
		// binary expressions are evaluated by evaluating the left and right operands
		// if the operator is an AND, then both operands must evaluate to true
		// if the operator is an OR, then either operand must evaluate to true
		else if (auto binary = dynamic_cast<const Predicate::BinaryExpression*>(expr))
		{
			switch (binary->op)
			{
			case Predicate::Operator::And:
				{
					float left = EvaluateExpression(binary->leftOperand.get(), level + 1, recurseDesires);
					float right = EvaluateExpression(binary->rightOperand.get(), level + 1, recurseDesires);
					if (left * right > 0.0f)
					{
						// if both operands evaluated to true, then return their average
						return (left + right) / 2.0f;
					}
					else
					{
						return 0.0f;
					}
				}
			case Predicate::Operator::Or:
				{
					float left = EvaluateExpression(binary->leftOperand.get(), level + 1, recurseDesires);
					float right = EvaluateExpression(binary->rightOperand.get(), level + 1, recurseDesires);
					if (left + right <= 0.0f)
					{
						return 0.0f;
					}
					else
					{
						if (left > right)
						{
							return left;
						}
						else
						{
							return right;
						}
					}
				}
			default:
				throw std::runtime_error("Unknown operator");
			}
		}

		// recursive case, evaluate unary expressions
		else if (auto unary = dynamic_cast<const Predicate::UnaryExpression*>(expr))
		{
			switch (unary->op)
			{
			case Predicate::Operator::Not:
				{
					// we check for a truthy value and if it is truthy, we return 0.0f
					// otherwise we return 1.0f
					// therefore this operator has no effect on the weight of the predicate
					float value = EvaluateExpression(unary->operand.get(), level + 1, recurseDesires);
					return value > 0.0f ? 0.0f : 1.0f;
				}
			default:
				throw std::runtime_error("Unknown operator");
			}
		}

		// unknown expression type
		else
		{
			throw std::runtime_error("Unknown expression type");
		}
	}
}