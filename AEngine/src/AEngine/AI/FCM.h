#pragma once
#include <vector>
#include <string>
#include <map>
#include <functional>

namespace AEngine
{
	struct Concept
	{
		const std::string name;
		float initialValue;
		float activationThreshold;
		std::function<void(float)> onActivate;
		std::function<void(float)> onDeactivate;
		bool active{ false };

		Concept(
			const std::string& name,
			float initialValue,
			float activationThreshold,
			std::function<void(float)> onActivate = nullptr,
			std::function<void(float)> onDeactivate = nullptr
		) : name(name),
		    initialValue(initialValue),
		    activationThreshold(activationThreshold),
		    onActivate(onActivate),
		    onDeactivate(onDeactivate) {}
	};

	class Arc
	{
	public:
		unsigned int from;
		unsigned int to;
		float weight;

		Arc(unsigned int from, unsigned int to, float weight)
			: from(from), to(to), weight(weight)
		{}
	};

	class FCM
	{
	public:
		void AddConcept(
			const std::string& name,
			float initialValue,
			float activationThreshold,
			std::function<void(float)> onActivate = nullptr,
			std::function<void(float)> onDeactivate = nullptr
		);

		void AddEdge(
			unsigned int from,
			unsigned int to,
			float weight
		);

		void OnUpdate();
		void Init();

		float GetConceptValue(const std::string& name) const;
		float GetConceptValue(unsigned int index) const;
		bool SetConceptValue(const std::string& name, float value);
		bool SetConceptValue(unsigned int index, float value);

		unsigned int GetConceptCount() const;
		const std::vector<Concept>& GetConcepts() const;
		const std::vector<float>& GetWeights() const;
		const std::vector<float>& GetActivationLevels() const;

	private:
		bool m_isInit{ false };
		unsigned int m_count{ 0 };

		std::vector<Concept> m_concepts;
		std::vector<Arc> m_arcs;

		std::vector<float> m_activationLevels;
		std::vector<float> m_activationLevelsLast;
		std::vector<float> m_weights;
	};
}
