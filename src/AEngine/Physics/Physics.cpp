#include "AEngine/Physics/Physics.h"

namespace AEngine
{
	using namespace React;

	Physics::Physics()
		:
		m_Manager(ReactManager::GetInstance()),
		m_Listener(m_Manager.GetPhysicsEventListener())
	{

	}

	void Physics::Init(const PhysicsSettings& settings)
	{
		m_Manager.Init();
		m_ShapeFactory.SetCommon(&m_Manager.common);
		m_BodyFactory.SetWorld(m_Manager.worldPtr);
	}

	void Physics::Update(float frametime)
	{
		m_Manager.Update(frametime);
	}

	React::PhysicsEventListener& Physics::GetPhysicsEventListener() const
	{
		return m_Listener;
	}

	ReactBody Physics::AddBody(ReactBody::BodyType type, size_t index, glm::vec3 position, glm::quat orientation)
	{
		r3d::BodyType reactType = r3d::BodyType::STATIC;

		switch (type)
		{
		case ReactBody::BodyType::STATIC:
			reactType = r3d::BodyType::STATIC;
			break;
		case ReactBody::BodyType::KINEMATIC:
			reactType = r3d::BodyType::KINEMATIC;
			break;
		case ReactBody::BodyType::DYNAMIC:
			reactType = r3d::BodyType::DYNAMIC;
			break;
		}

		r3d::Transform reactTransform{
			{position.x, position.y, position.z}, {orientation.x, orientation.y, orientation.z, orientation.w }
		};

		return {
			m_BodyFactory.AddBody(reactType, reactTransform, m_Shapes[index])
		};
	}

	size_t Physics::AddBoxShape(const float* halfExtents)
	{
		m_Shapes.push_back(m_ShapeFactory.AddBoxShape(
			r3d::Vector3(halfExtents[0], halfExtents[1], halfExtents[2])
		));
		return m_Shapes.size() - 1;
	}

	size_t Physics::AddSphereShape(float radius)
	{
		m_Shapes.push_back(m_ShapeFactory.AddSphereShape(radius));
		return m_Shapes.size() - 1;
	}
}
