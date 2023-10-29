#pragma once

#include <vector>
#include <string>
#include <unordered_map>

namespace AEngine
{
	class Node
	{
	public:
		std::string name;
		float value;

		Node(const std::string& name, float initialValue = 0.0f)
			: name(name), value(value)
		{}
	};

	class Edge
	{
	public:
		Node* from;
		Node* to;
		float weight;

		Edge(Node* from, Node* to, float weight)
			: from(from), to(to), weight(weight)
		{}
	};

	class FCM
	{
	public:
		void addNode(const std::string& name, float initialValue = 0.0f);
		
		void addEdge(const std::string& from, const std::string& to, float weight);

		void update();

		float getNodeValue(const std::string& name) const;

		void setNodeValue(const std::string& name, float value);
	private:
		std::vector<Node> m_nodes;
		std::vector<Edge> m_edges;
		std::unordered_map<std::string, Node*> m_nodeMap;
	};
}