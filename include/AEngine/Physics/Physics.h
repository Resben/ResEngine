#pragma once
#include <glm/glm.hpp>
#include <map>
#include <vector>

#include "../Render/Mesh.h"
#include "PhysicsEventListener.h"
#include "ReactBody.h"
#include "ReactBodyFactory.h"
#include "ReactManager.h"
#include "ReactShapeFactory.h"

namespace AEngine
{
	/**
	 * @class Physics
	 * @brief Facade used to implement React Physics functions for AEngine
	 * @author Lane O'Rafferty (33534304)
	 * @version 01.00
	 * @date 27/10/2022
	**/
	class Physics
	{
	public:

		/**
		 * @brief Used to contain the settings for initializing a physics world
		**/
		struct PhysicsSettings
		{
			float timeStep = 1.0f / 60.0f;
		};

		/**
		 * @brief Default constructor
		**/
		Physics();

		/**
		 * @brief Initializes the physics world
		 * @param settings: the initial world settings
		**/
		void Init(const PhysicsSettings& settings = PhysicsSettings{});

		/**
		 * @brief Updates the physics world
		 * @param frametime: the amount of seconds passed between calls to this function
		**/
		void Update(float frametime);

		/**
		 * @brief Returns the event listener that is bound to this physics world
		**/
		React::PhysicsEventListener& GetPhysicsEventListener() const;

		/**
		 * @brief Adds a physics body to the physics world
		 * @param type: the type of ReactBody
		 * @param index: the id of the collision shape
		 * @param position: the initial position of the body in the physics world
		 * @param orientation: the initial orientation of the body in the physics world 
		 * @pre the body needs an existing shape to be created
		**/
		ReactBody AddBody(ReactBody::BodyType type, size_t index, glm::vec3 position, glm::quat orientation);

		/**
		 * @brief Adds a box collision shape to the physics world
		**/
		size_t AddBoxShape(const float* halfExtents);

		/**
		 * @brief Adds a sphere collision shape to the physics world
		**/
		size_t AddSphereShape(float radius);

	private:
		React::ReactManager& m_Manager;
		React::ReactShapeFactory m_ShapeFactory;
		React::ReactBodyFactory m_BodyFactory;
		React::PhysicsEventListener& m_Listener;
		std::vector<r3d::CollisionShape*> m_Shapes;
	};
}

