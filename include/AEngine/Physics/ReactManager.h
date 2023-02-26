#pragma once

#include <reactphysics3d/reactphysics3d.h>
#include "AEngine/Physics/PhysicsEventListener.h"

namespace React
{
	namespace r3d = reactphysics3d;

	/**
	 * @class ReactManager
	 * @brief ReactManager Class Singleton used by the Physics for PvB
	 * @author Geoff Candy (34183006)
	 * @author Lane O'Rafferty (33534304)
	 * @version 01.01
	 * @date 27/10/2022
	 * @todo:
	 * - add set timestep method
	 * - add set world settings method
	**/
	class ReactManager
	{
	public:
		/**
		 * @brief Deleted Copy Constructor for the ReactManager Class
		 */
		ReactManager(const ReactManager& reManRef) = delete;

		/**
		 * @brief Deleted Assignment Operator for the ReactManager Class
		 */
		void operator=(const ReactManager& reManRef) = delete;

		/**
		 * @brief Method to generate the instance for the ReactManager Class
		 * @return ReactManager&
		 */
		static ReactManager& GetInstance();

		/**
		 * @brief Method to get the Physics Event Listener for the ReactManager Class
		 * @return PhysicsEventListener&
		 */
		PhysicsEventListener& GetPhysicsEventListener();

		/**
		 * @brief Method that initialises the ReactManager creating the React PhysicsWorld
		 * @param float
		 */
		void Init(float timestep = 1 / 60.f);

		/**
		 * @brief Method that Updates all the react attributes
		 * @param float
		 */
		void Update(float dtSeconds);

		/**
		 * @brief Method that returns the Interpolation Factor
		 * @return float
		 */
		float GetInterpolationFactor();

		// the React Physics Common
		r3d::PhysicsCommon common;

		// pointer to the React Physics World
		r3d::PhysicsWorld* worldPtr;

	private:
		// the Physics Event Listener
		PhysicsEventListener m_listener;

		// the Physics Time Step (physics update frequency)
		float m_timestep;

		// Accumulator for enforcing Time Step and computing interpolation factor
		float m_accumulator;

		// Default Constructor
		ReactManager();
	};
}
