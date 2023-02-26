#pragma once

#include "reactphysics3d/reactphysics3d.h"

namespace React
{
	namespace r3d = reactphysics3d;

	/**
	* @brief	Used to construct a mesh collision shape
	*/
	
	/**
	 * @class ReactShapeFactory
	 * @brief Used to create collision shapes for the physics world
	 * @author Lane O'Rafferty (33534304)
	 * @version 1.00
	 * @date 27/10/2022
	**/
	class ReactShapeFactory
	{
	public:

		/**
		* @brief	Default constructor
		*/
		ReactShapeFactory();

		/**
		* @brief	Sets a pointer to the React Physics Common
		* @param	commonPtr: a pointer to the React Physics Common
		* @return	void
		*/
		void SetCommon(r3d::PhysicsCommon* commonPtr);

		/**
		* @brief	Adds a box shape to the React Physics Common and returns a pointer to it
		* @param	halfExtents: half the x, y, z dimensions of the box
		* @return	CollisionShape*
		*/
		r3d::CollisionShape* AddBoxShape(r3d::Vector3 halfExtents);

		/**
		* @brief	Adds a sphere shape to the React Physics Common and returns a pointer to it
		* @param	radius: radius of the sphere
		* @return	CollisionShape*
		*/
		r3d::CollisionShape* AddSphereShape(float radius);

	private:

		// a pointer to the React Physics Common object
		r3d::PhysicsCommon* m_Common;
	};
}