#include "AEngine/Physics/ReactShapeFactory.h"

using namespace React;

ReactShapeFactory::ReactShapeFactory()
	:
	m_Common(nullptr)
{

}

void ReactShapeFactory::SetCommon(r3d::PhysicsCommon* commonPtr)
{
	m_Common = commonPtr;
}

r3d::CollisionShape* ReactShapeFactory::AddBoxShape(r3d::Vector3 halfExtents)
{
	return m_Common->createBoxShape(halfExtents);
}

r3d::CollisionShape* ReactShapeFactory::AddSphereShape(float radius)
{
	return m_Common->createSphereShape(radius);
}