#pragma once
#include <vector>
#include <string>
#include <map>
#include <functional>

namespace AEngine
{
	/**
	 * \struct Concept
	 * @brief A concept / node in the FCM
	 */
	struct Concept
	{
		const std::string name;
		float initialValue;
		float activationThreshold;
		float decayRate;
		std::function<void(float)> onActivate;
		std::function<void(float)> onDeactivate;
		bool active{ false };

		Concept(
			const std::string& name,
			float initialValue,
			float activationThreshold,
			float decayRate,
			std::function<void(float)> onActivate = nullptr,
			std::function<void(float)> onDeactivate = nullptr
		) : name(name),
		    initialValue(initialValue),
		    activationThreshold(activationThreshold),
		    decayRate(decayRate),
		    onActivate(onActivate),
		    onDeactivate(onDeactivate) {}
	};

	/**
	 * \class Arc
	 * @brief An arc / edge in the FCM
	 */
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

	/**
	 * \class FCM
	 * @brief Fuzzy Cognitive Map class for emotion system
	 */
	class FCM
	{
	public:
			/**
			 * \brief Add a concept to the FCM
			 * \param name[in] Name of the concept
			 * \param initialValue[in] Initial value of the concept; clamped between 0 and 1
			 * \param activationThreshold[in] Activation threshold of the concept; clamped between 0 and 1
			 * \param decayRate[in] Decay rate of the concept, negative for growth; clamped between -1 and 1
			 * \param onActivate[in] Callback function when the concept is activated
			 * \param onDeactivate[in] Callback function when the concept is deactivated
			*/
		void AddConcept(
			const std::string& name,
			float initialValue,
			float activationThreshold,
			float decayRate,
			std::function<void(float)> onActivate = nullptr,
			std::function<void(float)> onDeactivate = nullptr
		);

			/**
			 * \brief Add an edge to the FCM
			 * \param from[in] Index of the concept the edge is coming from
			 * \param to[in] Index of the concept the edge is going to (effecting)
			 * \param weight[in] Weight of the edge; clamped between -1 and 1
			*/
		void AddEdge(
			unsigned int from,
			unsigned int to,
			float weight
		);

			/**
			 * \brief Update the FCM
			 * \param dt[in] Delta time
			*/
		void OnUpdate(float dt = 0.0f);
			/**
			 * \brief Initialize the FCM
			 * \note Once this has been called no more nodes or edges can be added
			*/
		void Init();

			/**
			 * \brief Get the value of a concept
			 * \param name[in] Name of the concept
			 * \return Value of the concept
			*/
		float GetConceptValue(const std::string& name) const;
			/**
			 * \brief Get the value of a concept
			 * \param index[in] Index of the concept
			 * \return Value of the concept
			*/
		float GetConceptValue(unsigned int index) const;
			/**
			 * \brief Set the value of a concept
			 * \param name[in] Name of the concept
			 * \param value[in] Value of the concept; clamped between 0 and 1
			 * \return True if the concept was found
			*/
		bool SetConceptValue(const std::string& name, float value);
			/**
			 * \brief Set the value of a concept
			 * \param index[in] Index of the concept
			 * \param value[in] Value of the concept; clamped between 0 and 1
			 * \return True if the concept was found
			*/
		bool SetConceptValue(unsigned int index, float value);

			/**
			 * \brief Get the number of concepts in the FCM
			 * \return Number of concepts
			*/
		unsigned int GetConceptCount() const;
			/**
			 * \brief Get the concepts in the FCM
			 * \return Concepts
			*/
		const std::vector<Concept>& GetConcepts() const;
			/**
			 * \brief Get the arcs in the FCM
			 * \return Arcs
			*/
		const std::vector<float>& GetWeights() const;
			/**
			 * \brief Get the activation levels of the concepts in the FCM
			 * \return Activation levels
			*/
		const std::vector<float>& GetActivationLevels() const;

	private:
		bool m_isInit{ false };      ///< Has the FCM been initialized
		unsigned int m_count{ 0 };   ///< Number of concepts in the FCM

		std::vector<Concept> m_concepts;   ///< Concepts in the FCM
		std::vector<Arc> m_arcs;           ///< Arcs in the FCM

		std::vector<float> m_activationLevels;       ///< Activation levels of the concepts
		std::vector<float> m_activationLevelsLast;   ///< Activation levels of the concepts last frame
		std::vector<float> m_weights;                ///< Weights of the arcs
	};
}
