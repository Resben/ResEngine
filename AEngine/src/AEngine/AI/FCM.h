#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

namespace AEngine
{
	class Node

	class Concept
	{
	public:
		std::string name;
		float value;

		Concept(const std::string& name, float initialValue = 0.0f)
			: name(name), value(value)
		{}
	};

	class Arc
	{
	public:
		Concept* from;
		Concept* to;
		float weight;

		Arc(Concept* from, Concept* to, float weight)
			: from(from), to(to), weight(weight)
		{}
	};

	class FCM
	{
	public:
		void AddNode(
			const std::string& name,
			float initialValue = 0.0f,
			std::function<void(float)> onActivate = nullptr,
			std::function<void(float)> onDeactivate = nullptr
		);

		void AddEdge(
			const std::string& from,
			const std::string& to,
			float weight
		);

		void OnUpdate();

		float GetConceptValue(const std::string& name) const;
		void SetConceptValue(const std::string& name, float value);

	private:
		std::vector<Concept> m_concepts;
		std::vector<Arc> m_arcs;
		std::unordered_map<std::string, Concept*> m_nodeMap;
	};
}
