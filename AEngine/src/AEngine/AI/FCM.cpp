#include "FCM.h"
#include <algorithm>

namespace AEngine
{
	void FCM::AddNode(
		const std::string &name,
		float initialValue,
		std::function<void(float)> onActivate,
		std::function<void(float)> onDeactivate
	)
	{
		m_concepts.emplace_back(name, initialValue);
		m_nodeMap[name] = &m_concepts.back();
	}

	void FCM::AddEdge(const std::string &from, const std::string &to, float weight)
	{
		Concept* fromNode = m_nodeMap[from];
		Concept* toNode = m_nodeMap[to];
		m_arcs.emplace_back(fromNode, toNode, weight);
	}

	void FCM::OnUpdate()
	{
		// apply weights to concepts
		for(Arc& edge : m_arcs)
		{
			edge.to->value += edge.from->value * edge.weight;
		}

		// clamp concepts to [0, 1]
		for(Concept& node : m_concepts)
		{
			node.value = std::clamp(node.value, 0.0f, 1.0f);
		}
	}

	float FCM::GetConceptValue(const std::string &name) const
	{
		return m_nodeMap.at(name)->value;
	}

	void FCM::SetConceptValue(const std::string &name, float value)
	{
		m_nodeMap.at(name)->value = value;
	}
}
