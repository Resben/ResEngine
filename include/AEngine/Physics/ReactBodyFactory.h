#pragma once
#include "reactphysics3d/reactphysics3d.h"

namespace React
{
	namespace r3d = reactphysics3d;

	/**
	 * @class ReactBodyFactory
	 * @brief Used to create physics bodies for the physics world
	 * @author Lane O'Rafferty (33534304)
	 * @version 1.00
	 * @date 27/10/2022
	**/
	class ReactBodyFactory
	{
	public:

		/**
		* @brief	Sets a pointer to the React Physics World
		* @param	worldPtr: a pointer to the React Physics World
		* @return	void
		*/
		void SetWorld(r3d::PhysicsWorld* worldPtr);

		/**
		* @brief	Method to add a Rigid Body to Physics World
		* @param	type: the Rigid Body type
		* @param	transform: the body's initial transform
		* @return	RigidBody*
		*/
		r3d::RigidBody* AddBody(r3d::BodyType type, r3d::Transform& transform);

		/**
		* @brief	Method to add a Rigid Body to Physics World with a given shape
		* @param	type: the Rigid Body type
		* @param	transform: the body's initial transform
		* @param	shape: the shape of the collider
		* @return	RigidBody*
		*/
		r3d::RigidBody* AddBody(r3d::BodyType type, r3d::Transform& transform, r3d::CollisionShape* shape);

	private:

		// pointer to the React Physics World
		r3d::PhysicsWorld* m_World;
	};
}
