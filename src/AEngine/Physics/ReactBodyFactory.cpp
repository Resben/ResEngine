#include "AEngine/Physics/ReactBodyFactory.h"

namespace React
{
	void ReactBodyFactory::SetWorld(r3d::PhysicsWorld* worldPtr)
	{
		m_World = worldPtr;
	}

	r3d::RigidBody* ReactBodyFactory::AddBody(r3d::BodyType type, r3d::Transform& transform)
	{
		r3d::RigidBody* body = m_World->createRigidBody(transform);
		body->setType(type);

		return body;
	}

	r3d::RigidBody* ReactBodyFactory::AddBody(
		r3d::BodyType type,
		r3d::Transform& transform,
		r3d::CollisionShape* shape)
	{
		r3d::RigidBody* body = m_World->createRigidBody(transform);
		body->setType(type);
		body->addCollider(shape, r3d::Transform::identity());

		return body;
	}
}
