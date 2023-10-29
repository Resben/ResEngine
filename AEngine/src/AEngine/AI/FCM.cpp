#include "FCM.h"


namespace AEngine
{
	void FCM::addNode(const std::string &name, float initialValue)
	{
		m_nodes.emplace_back(name, initialValue);
		m_nodeMap[name] = &m_nodes.back();
	}

	void FCM::addEdge(const std::string &from, const std::string &to, float weight)
	{
		Node* fromNode = m_nodeMap[from];
		Node* toNode = m_nodeMap[to];
		m_edges.emplace_back(fromNode, toNode, weight);
	}

	void FCM::update()
	{
		for(Edge& edge : m_edges)
		{
			edge.to->value += edge.from->value * edge.weight;
		}

		for(Node& node : m_nodes)
		{
			node.value = std::max(0.0f, std::min(1.0f, node.value));
		}
	}

	float FCM::getNodeValue(const std::string &name) const
	{
		return m_nodeMap.at(name)->value;
	}

	void FCM::setNodeValue(const std::string &name, float value)
	{
		m_nodeMap.at(name)->value = value;
	}
}
