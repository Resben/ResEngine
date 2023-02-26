#include "AEngine/Physics/ReactBody.h"
#include <glm/gtc/type_ptr.hpp>

namespace AEngine
{
	ReactBody::ReactBody(r3d::RigidBody* body)
		: m_Body(body)
	{

	}

	ReactBody::~ReactBody()
	{
		m_Body = nullptr;
	}

	void ReactBody::ApplyLocalForce(const glm::vec3& force)
	{
		m_Body->applyLocalForceAtCenterOfMass(ToReactVector(force));
	}

	void ReactBody::ApplyWorldForce(const glm::vec3& force)
	{
		m_Body->applyWorldForceAtCenterOfMass(ToReactVector(force));
	}

	void ReactBody::ApplyLinearVelocity(const glm::vec3& velocity)
	{
		m_Body->setLinearVelocity(ToReactVector(velocity));
	}

	void ReactBody::ApplyAngularVelocity(const glm::vec3& velocity)
	{
		m_Body->setAngularVelocity(ToReactVector(velocity));
	}

	void ReactBody::SetLinearDamping(float factor)
	{
		m_Body->setLinearDamping(factor);
	}

	void ReactBody::SetAngularDamping(float factor)
	{
		m_Body->setAngularDamping(factor);
	}

	void ReactBody::SetTransform(const glm::vec3& position, const glm::quat& orientation)
	{
		m_Body->setTransform(
			{ {position.x, position.y, position.z}, {orientation.x, orientation.y, orientation.z, orientation.w } }
		);
	}

	void ReactBody::GetTransform(glm::vec3& position, glm::quat& orientation)
	{
		const r3d::Vector3& bodyPos = m_Body->getTransform().getPosition();
		const r3d::Quaternion& bodyRot = m_Body->getTransform().getOrientation();

		position.x = bodyPos.x;
		position.y = bodyPos.y;
		position.z = bodyPos.z;

		orientation.x = bodyRot.x;
		orientation.y = bodyRot.y;
		orientation.z = bodyRot.z;
		orientation.w = bodyRot.w;
	}

	void ReactBody::ApplyLocalTorque(const glm::vec3& torque)
	{
		r3d::Vector3 vec{ torque.x, torque.y, torque.z };
		m_Body->applyLocalTorque(vec);
	}

	void ReactBody::SetTrigger(bool isTrigger, uint32_t index)
	{
		r3d::Collider* collider = m_Body->getCollider(index);

		if (collider)
			collider->setIsTrigger(isTrigger);
	}

	r3d::Vector3 ReactBody::ToReactVector(const glm::vec3& vec)
	{
		return {vec.x, vec.y, vec.z};
	}

	void ReactBody::SetUserData(void* data)
	{
		m_Body->setUserData(data);
	}

	void* ReactBody::GetUserData()
	{
		return m_Body->getUserData();
	}

	r3d::RigidBody* ReactBody::GetRigidBody()
	{
		return m_Body;
	}

	void ReactBody::SetGravity(bool state)
	{
		m_Body->enableGravity(state);
	}
}
