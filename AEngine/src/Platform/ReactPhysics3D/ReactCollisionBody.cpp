/**
 * \file   ReactCollisionBody.cpp
 * \author Lane O'Rafferty (33534304)
 * \author Christien Alden (34119981)
*/

#include "AEngine/Core/Logger.h"
#include "ReactCollider.h"
#include "ReactCollisionBody.h"
#include "ReactPhysics.h"

namespace AEngine
{
//--------------------------------------------------------------------------------
// ReactCollisionBody
//--------------------------------------------------------------------------------
	ReactCollisionBody::ReactCollisionBody(
		ReactPhysicsWorld* world,
		const Math::vec3& position,
		const Math::quat& orientation)
		: m_world{ world }, m_position{ position }, m_orientation{ orientation }
	{
		m_body = world->GetNative()->createCollisionBody({ AEMathToRP3D(position), AEMathToRP3D(orientation) });
	}

	ReactCollisionBody::~ReactCollisionBody()
	{
		m_world->GetNative()->destroyCollisionBody(m_body);
	}

	rp3d::CollisionBody* ReactCollisionBody::GetNative() const
	{
		return m_body;
	}

	void ReactCollisionBody::SetTransform(const Math::vec3& position, const Math::quat& orientation)
	{
		// update member variables
		m_position = position;
		m_orientation = orientation;

		// update rp3d world
		m_body->setTransform({
			AEMathToRP3D(position),
			AEMathToRP3D(orientation)
		});
	}

	void ReactCollisionBody::GetTransform(Math::vec3& position, Math::quat& orientation) const
	{
		// set output parameters based on member variables
		position = m_position;
		orientation = m_orientation;
	}

	SharedPtr<BoxCollider> ReactCollisionBody::AddBoxCollider(const Math::vec3& size, const Math::vec3& offset, const Math::quat& orientation)
	{
		rp3d::PhysicsCommon* common = dynamic_cast<ReactPhysicsAPI&>(PhysicsAPI::Instance()).GetCommon();
		rp3d::BoxShape* box = common->createBoxShape(AEMathToRP3D(size));
		rp3d::Transform transform(AEMathToRP3D(offset), AEMathToRP3D(orientation));
		rp3d::Collider* collider = m_body->addCollider(box, transform);
		SharedPtr<ReactBoxCollider> boxCollider = MakeShared<ReactBoxCollider>(collider);
		m_colliders.push_back(boxCollider);
		return boxCollider;
	}

	SharedPtr<CapsuleCollider> ReactCollisionBody::AddCapsuleCollider(float radius, float height, const Math::vec3& offset, const Math::quat& orientation)
	{
		rp3d::PhysicsCommon* common = dynamic_cast<ReactPhysicsAPI&>(PhysicsAPI::Instance()).GetCommon();
		rp3d::CapsuleShape* capsule = common->createCapsuleShape(radius, height);
		rp3d::Transform transform(AEMathToRP3D(offset), AEMathToRP3D(orientation));
		rp3d::Collider* collider = m_body->addCollider(capsule, transform);
		SharedPtr<ReactCapsuleCollider> capsuleCollider = MakeShared<ReactCapsuleCollider>(collider);
		m_colliders.push_back(capsuleCollider);
		return capsuleCollider;
	}

	SharedPtr<SphereCollider> ReactCollisionBody::AddSphereCollider(float radius, const Math::vec3& offset, const Math::quat& orientation)
	{
		rp3d::PhysicsCommon* common = dynamic_cast<ReactPhysicsAPI&>(PhysicsAPI::Instance()).GetCommon();
		rp3d::SphereShape* sphere = common->createSphereShape(radius);
		rp3d::Transform transform(AEMathToRP3D(offset), AEMathToRP3D(orientation));
		rp3d::Collider* collider = m_body->addCollider(sphere, transform);
		SharedPtr<ReactSphereCollider> sphereCollider = MakeShared<ReactSphereCollider>(collider);
		m_colliders.push_back(sphereCollider);
		return sphereCollider;
	}

	const std::list<SharedPtr<Collider>>& ReactCollisionBody::GetColliders()
	{
		return m_colliders;
	}

	void ReactCollisionBody::RemoveCollider(Collider* collider)
	{
		for (auto it = m_colliders.begin(); it != m_colliders.end(); ++it)
		{
			if (it->get() == collider)
			{
				/// \todo Fix the hell out of this, its awful...
				switch (collider->GetType())
				{
				case Collider::Type::Box:
					{
						ReactBoxCollider* box = dynamic_cast<ReactBoxCollider*>(collider);
						m_body->removeCollider(box->GetNativeCollider()->GetNativeCollider());
					}
					break;
				case Collider::Type::Capsule:
					{
						ReactCapsuleCollider* capsule = dynamic_cast<ReactCapsuleCollider*>(collider);
						m_body->removeCollider(capsule->GetNativeCollider()->GetNativeCollider());
					}
					break;
				case Collider::Type::Sphere:
					{
						ReactSphereCollider* sphere = dynamic_cast<ReactSphereCollider*>(collider);
						m_body->removeCollider(sphere->GetNativeCollider()->GetNativeCollider());
					}
				}

				// cast to ReactCollider and get the native collider
				m_colliders.erase(it);
				break;
			}
		}
	}

//--------------------------------------------------------------------------------
// ReactRigidBody
//--------------------------------------------------------------------------------
	ReactRigidBody::ReactRigidBody(ReactPhysicsWorld* world, const Math::vec3& position, const Math::quat& orientation)
	{
		m_body = MakeUnique<ReactCollisionBody>(world, position, orientation);
		m_body->GetNative()->setUserData(static_cast<void*>(this));
	}

	void ReactRigidBody::SetType(Type type)
	{
		m_type = type;
	}

	RigidBody::Type ReactRigidBody::GetType() const
	{
		return m_type;
	}

	void ReactRigidBody::SetMass(float massKg)
	{
		if (massKg <= 0.0f)
		{
			AE_LOG_ERROR("ReactRigidBody::SetMass::Mass_must_be_greater_than_zero");
			return;
		}

		// don't update the mass if it's the same, prevents unnecessary calculations of the inertia tensor
		if (abs(massKg - m_mass) < std::numeric_limits<float>::epsilon())
		{
			return;
		}

		m_mass = massKg;
		m_inverseMass = 1.0f / massKg;
		CalculateInertiaTensor();
	}

	float ReactRigidBody::GetMass() const
	{
		return m_mass;
	}

	float ReactRigidBody::GetInverseMass() const
	{
		return m_inverseMass;
	}

	void ReactRigidBody::SetRestitution(float restitution)
	{
		m_restitution = std::clamp(restitution, 0.0f, 1.0f);
	}

	float ReactRigidBody::GetRestitution() const
	{
		return m_restitution;
	}

	Math::mat3 ReactRigidBody::GetLocalInertiaTensor() const
	{
		return m_inertiaTensor;
	}

	Math::mat3 ReactRigidBody::GetWorldInertiaTensor() const
	{
		Math::vec3 position;
		Math::quat orientation;
		m_body->GetTransform(position, orientation);
		Math::mat3 rotation = Math::mat3_cast(orientation);
		return rotation * m_inertiaTensor * Math::transpose(rotation);
	}

	Math::mat3 ReactRigidBody::GetLocalInverseInertiaTensor() const
	{
		return m_inverseInertiaTensor;
	}

	Math::mat3 ReactRigidBody::GetWorldInverseInertiaTensor() const
	{
		Math::vec3 position;
		Math::quat orientation;
		m_body->GetTransform(position, orientation);
		Math::mat3 rotation = Math::mat3_cast(orientation);
		return rotation * m_inverseInertiaTensor * Math::transpose(rotation);
	}

	void ReactRigidBody::SetCentreOfMass(const Math::vec3 &centreOfMass)
	{
		m_centreOfMass = centreOfMass;
	}

	Math::vec3 ReactRigidBody::GetCentreOfMass() const
	{
		return m_centreOfMass;
	}

	Math::vec3 ReactRigidBody::GetCentreOfMassWorldSpace() const
	{
		Math::vec3 position;
		Math::quat orientation;
		m_body->GetTransform(position, orientation);
		return position + (orientation * m_centreOfMass);
	}

	void ReactRigidBody::SetHasGravity(bool hasGravity)
	{
		m_hasGravity = hasGravity;
	}

	bool ReactRigidBody::GetHasGravity() const
	{
		return m_hasGravity;
	}

	void ReactRigidBody::SetLinearDamping(float damping)
	{
		m_linearDamping = std::clamp(damping, 0.0f, 1.0f);
	}

	float ReactRigidBody::GetLinearDamping() const
	{
		return m_linearDamping;
	}

	void ReactRigidBody::SetAngularDamping(float damping)
	{
		m_angularDamping = std::clamp(damping, 0.0f, 1.0f);
	}

	float ReactRigidBody::GetAngularDamping() const
	{
		return m_angularDamping;
	}

	void ReactRigidBody::SetLinearMomentum(const Math::vec3 &momentum)
	{
		m_linearMomentum = momentum;
	}

	const Math::vec3 ReactRigidBody::GetLinearMomentum() const
	{
		return m_linearMomentum;
	}

	void ReactRigidBody::SetAngularMomentum(const Math::vec3 &momentum)
	{
		m_angularMomentum = momentum;
	}

	const Math::vec3 ReactRigidBody::GetAngularMomentum() const
	{
		return m_angularMomentum;
	}

	void ReactRigidBody::SetLinearVelocity(const Math::vec3& velocity)
	{
		m_linearMomentum = m_mass * velocity;
	}

	const Math::vec3 ReactRigidBody::GetLinearVelocity() const
	{
		return m_linearMomentum * m_inverseMass;
	}

	void ReactRigidBody::SetAngularVelocity(const Math::vec3& velocity)
	{
		m_angularMomentum = m_inertiaTensor * velocity;
	}

	const Math::vec3 ReactRigidBody::GetAngularVelocity() const
	{
		return m_angularMomentum * m_inverseInertiaTensor;
	}


//--------------------------------------------------------------------------------
// From ReactCollisionBody
//--------------------------------------------------------------------------------
	void ReactRigidBody::SetTransform(const Math::vec3& position, const Math::quat& orientation)
	{
		m_body->SetTransform(position, orientation);
	}

	void ReactRigidBody::GetTransform(Math::vec3& position, Math::quat& orientation) const
	{
		m_body->GetTransform(position, orientation);
	}

	SharedPtr<BoxCollider> ReactRigidBody::AddBoxCollider(const Math::vec3& size, const Math::vec3& offset, const Math::quat& orientation)
	{
		// attach the collider and update the inertia tensor
		SharedPtr<BoxCollider> collider = m_body->AddBoxCollider(size, offset, orientation);
		CalculateInertiaTensor();
		return collider;
	}

	SharedPtr<CapsuleCollider> ReactRigidBody::AddCapsuleCollider(float radius, float height, const Math::vec3& offset, const Math::quat& orientation)
	{
		SharedPtr<CapsuleCollider> collider = m_body->AddCapsuleCollider(radius, height, offset, orientation);
		CalculateInertiaTensor();
		return collider;
	}

	SharedPtr<SphereCollider> ReactRigidBody::AddSphereCollider(float radius, const Math::vec3& offset, const Math::quat& orientation)
	{
		SharedPtr<SphereCollider> collider = m_body->AddSphereCollider(radius, offset, orientation);
		CalculateInertiaTensor();
		return collider;
	}

	const std::list<SharedPtr<Collider>>& ReactRigidBody::GetColliders()
	{
		return m_body->GetColliders();
	}

	void ReactRigidBody::RemoveCollider(Collider* collider)
	{
		m_body->RemoveCollider(collider);
	}

	void ReactRigidBody::CalculateInertiaTensor()
	{
		auto colliders = m_body->GetColliders();
		if (colliders.empty())
		{
			return;
		}

		SharedPtr<Collider> collider = colliders.front();

		switch (collider->GetType())
		{
		case Collider::Type::Box:
			{
				ReactBoxCollider* box = dynamic_cast<ReactBoxCollider*>(collider.get());
				Math::vec3 halfExtents = box->GetSize();

				// calculate inertia tensor assuming uniform density and no offset
				m_inertiaTensor[0][0] = (1.0f / 3.0f) * m_mass * (pow(halfExtents.y, 2.0f) + pow(halfExtents.z, 2.0f));
				m_inertiaTensor[1][1] = (1.0f / 3.0f) * m_mass * (pow(halfExtents.x, 2.0f) + pow(halfExtents.z, 2.0f));
				m_inertiaTensor[2][2] = (1.0f / 3.0f) * m_mass * (pow(halfExtents.x, 2.0f) + pow(halfExtents.y, 2.0f));
				m_inverseInertiaTensor = Math::inverse(m_inertiaTensor);
			}
			break;

		case Collider::Type::Capsule:
			{
				ReactCapsuleCollider* capsule = dynamic_cast<ReactCapsuleCollider*>(collider.get());
				float radius = capsule->GetRadius();
				float height = capsule->GetHeight();

				// calculate inertia tensor assuming uniform density and no offset
				float hemisphereI = (2.0f / 5.0f) * m_mass * radius * radius;
				float cylinderI = (1.0f / 12.0f) * m_mass * (3.0f * radius * radius + height * height);
				m_inertiaTensor[0][0] = 2 * hemisphereI + cylinderI;
				m_inertiaTensor[1][1] = 2 * hemisphereI + cylinderI;
				m_inertiaTensor[2][2] = hemisphereI + cylinderI;
				m_inverseInertiaTensor = Math::inverse(m_inertiaTensor);
			}
			break;
		case Collider::Type::Sphere:
			{
				ReactSphereCollider* sphere = dynamic_cast<ReactSphereCollider*>(collider.get());
				float radius = sphere->GetRadius();

				// calculate inertia tensor assuming uniform density and no offset
				m_inertiaTensor[0][0] = (2.0f / 5.0f) * m_mass * radius * radius;
				m_inertiaTensor[1][1] = (2.0f / 5.0f) * m_mass * radius * radius;
				m_inertiaTensor[2][2] = (2.0f / 5.0f) * m_mass * radius * radius;
				m_inverseInertiaTensor = Math::inverse(m_inertiaTensor);
			}
			break;
		default:
			{
				AE_LOG_FATAL("ReactRigidBody::CalculateInertiaTensor::Invalid_collider_type");
			}
		}
	}
}
