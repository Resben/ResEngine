/**
 * \file   ReactPhysics.cpp
 * \author Lane O'Rafferty (33534304)
 * \author Christien Alden (34119981)
*/
#include "AEngine/Core/TimeStep.h"
#include "AEngine/Core/Types.h"
#include "ReactPhysics.h"
#include "ReactCollisionBody.h"

namespace AEngine
{
//--------------------------------------------------------------------------------
// Math Conversions
//--------------------------------------------------------------------------------
	rp3d::Vector3 AEMathToRP3D(const Math::vec3& vec)
	{
		return { vec.x, vec.y, vec.z };
	}

	Math::vec3 RP3DToAEMath(const rp3d::Vector3& vec)
	{
		return { vec.x, vec.y, vec.z };
	}

	rp3d::Quaternion AEMathToRP3D(const Math::quat& quat)
	{
		return { quat.x, quat.y, quat.z, quat.w };
	}

	Math::quat RP3DToAEMath(const rp3d::Quaternion quat)
	{
		return { quat.w,  quat.x, quat.y, quat.z };
	}


//--------------------------------------------------------------------------------
// ReactEventListener
//--------------------------------------------------------------------------------
	void ReactCollisionResolver::onContact(const CollisionCallback::CallbackData& callbackData)
	{
		unsigned int nbContactPairs = callbackData.getNbContactPairs();

		for (unsigned int i = 0; i < nbContactPairs; ++i)
		{
			// get each contact pair and ignore it is a contact exit event
			CollisionCallback::ContactPair contactPair = callbackData.getContactPair(i);
			if (contactPair.getEventType() == CollisionCallback::ContactPair::EventType::ContactExit)
			{
				continue;
			}

			// get the collision body from each contact pair
			rp3d::CollisionBody* body1 = contactPair.getBody1();
			rp3d::CollisionBody* body2 = contactPair.getBody2();

			// get the user data from each collision body and check for nullptr
			// if at least one of the bodies has no user data, then we have collided
			// with a non-rigidbody and we can ignore the collision
			void* userData1 = body1->getUserData();
			void* userData2 = body2->getUserData();
			if (!userData1 || !userData2)
			{
				continue;
			}

			// get colliders involved in the collision
			rp3d::Collider* collider1 = contactPair.getCollider1();
			rp3d::Collider* collider2 = contactPair.getCollider2();

			// get local to world transforms
			rp3d::Transform transform1 = collider1->getLocalToWorldTransform();
			rp3d::Transform transform2 = collider2->getLocalToWorldTransform();

			// setup the collision data and average the contact points
			rp3d::Vector3 normal{ 0.0f, 0.0f, 0.0f };
			rp3d::Vector3 contactBody1{ 0.0f, 0.0f, 0.0f };
			rp3d::Vector3 contactBody2{ 0.0f, 0.0f, 0.0f };
			float penetrationDepth = 0.0f;

			unsigned int nbContactPoints = contactPair.getNbContactPoints();
			for (unsigned int j = 0; j < nbContactPoints; ++j)
			{
				// get the contact point from the contact pair
				CollisionCallback::ContactPoint contactPoint = contactPair.getContactPoint(j);

				// get the contact point in world space for each body
				rp3d::Vector3 point1 = contactPoint.getLocalPointOnCollider1();
				contactBody1 += transform1 * point1;
				rp3d::Vector3 point2 = contactPoint.getLocalPointOnCollider2();
				contactBody2 += transform2 * point2;

				// get the normal and penetration depth
				normal += RP3DToAEMath(contactPoint.getWorldNormal());
				penetrationDepth += contactPoint.getPenetrationDepth();
			}

			// calculate the average normal and penetration depth
			normal /= static_cast<float>(nbContactPoints);
			penetrationDepth /= static_cast<float>(nbContactPoints);

			// resolve the collision

			// retrieve the ReactCollisionBody from the user data
			ReactRigidBody* reactBody1 = static_cast<ReactRigidBody*>(userData1);
			ReactRigidBody* reactBody2 = static_cast<ReactRigidBody*>(userData2);

			// depenetrate the bodies
			DepenetrateBody(reactBody1, penetrationDepth, normal);
			DepenetrateBody(reactBody2, penetrationDepth, normal);

		}
	}

	void ReactCollisionResolver::DepenetrateBody(ReactRigidBody* body, float penetrationDepth, const Math::vec3& normal)
	{
		Math::vec3 position;
		Math::quat rotation;
		body->GetTransform(position, rotation);
		position -= normal * (penetrationDepth * 0.5f);
		body->SetTransform(position, rotation);
	}


//--------------------------------------------------------------------------------
// ReactPhysicsAPI
//--------------------------------------------------------------------------------
	ReactPhysicsAPI& ReactPhysicsAPI::Instance()
	{
		static ReactPhysicsAPI s_instance;
		return s_instance;
	}

	ReactPhysicsAPI::ReactPhysicsAPI()
		: m_common()
	{

	}

	UniquePtr<PhysicsWorld> ReactPhysicsAPI::CreateWorld(const PhysicsWorld::Props& props)
	{
		// create the world, set the initial properties, then return it
		UniquePtr<PhysicsWorld> world = MakeUnique<ReactPhysicsWorld>(&m_common);
		world->Init(props);
		return std::move(world);
	}

	rp3d::PhysicsCommon* ReactPhysicsAPI::GetCommon()
	{
		return &m_common;
	}


//--------------------------------------------------------------------------------
// ReactPhysicsWorld
//--------------------------------------------------------------------------------
	ReactPhysicsWorld::ReactPhysicsWorld(rp3d::PhysicsCommon* common)
		: m_world(nullptr), m_accumulator(0), m_renderer{ nullptr }, m_eventListener{}
	{
		m_world = common->createPhysicsWorld();
		m_world->setIsDebugRenderingEnabled(false);
		m_world->setEventListener(&m_eventListener);
		m_renderer = MakeUnique<ReactPhysicsRenderer>(m_world->getDebugRenderer());
	}

	ReactPhysicsWorld::~ReactPhysicsWorld()
	{
		// This ensures that the world is properly destroyed within the PhysicsCommon.
		ReactPhysicsAPI::Instance().GetCommon()->destroyPhysicsWorld(m_world);
	}

	void ReactPhysicsWorld::Init(const Props& settings)
	{
		m_props = settings;
	}

	void ReactPhysicsWorld::SetUpdateStep(TimeStep step)
	{
		m_props.updateStep = step;
	}

	void ReactPhysicsWorld::OnUpdate(TimeStep deltaTime)
	{
		m_accumulator += deltaTime;

		while (m_accumulator >= m_props.updateStep)
		{
			// remove the update step from the accumulator
			m_accumulator -= m_props.updateStep;

			// run the update step on each of the rigidbodies in the world
			// this will update their positions and rotations
			// as well as any other physics calculations
			for (auto it = m_rigidBodies.begin(); it != m_rigidBodies.end();)
			{
				SharedPtr<ReactRigidBody> rb = it->lock();
				if (!rb)
				{
					it = m_rigidBodies.erase(it);
					if (it == m_rigidBodies.end())
					{
						break;
					}

					// don't ++it here otherwise we will skip the element after the erased one
					continue;
				}

				UpdateRigidBody(m_props.updateStep, rb.get());
				++it;
			}

			// update the rp3d physics world to detect collisions
			// inside here the collision callbacks will be called
			// and the properties of the rigidbodies will be updated
			// to reflect the collisions; however, the positions and
			// rotations will not be updated, only the immediate collision
			// resolution will be performed and the positions and rotations
			// will be updated in the next step
			m_world->update(m_props.updateStep.Seconds());

			// update the render data if debug rendering is enabled
			if (m_world->getIsDebugRenderingEnabled())
			{
				m_renderer->GenerateRenderData();
			}
		}
	}

	SharedPtr<CollisionBody> ReactPhysicsWorld::AddCollisionBody(const Math::vec3& position, const Math::quat& orientation)
	{
		SharedPtr<ReactCollisionBody> body = MakeShared<ReactCollisionBody>(this, position, orientation);
		m_collisionBodies.push_back(MakeWeak(body));
		return body;
	}

	SharedPtr<RigidBody> ReactPhysicsWorld::AddRigidBody(const Math::vec3& position, const Math::quat& orientation)
	{
		SharedPtr<ReactRigidBody> body = MakeShared<ReactRigidBody>(this, position, orientation);
		m_rigidBodies.push_back(MakeWeak(body));
		return body;
	}

	void ReactPhysicsWorld::Render(const Math::mat4& projectionView) const
	{
		m_renderer->Render(projectionView);
	}

	bool ReactPhysicsWorld::IsRenderingEnabled() const
	{
		return m_world->getIsDebugRenderingEnabled();
	}

	void ReactPhysicsWorld::SetRenderingEnabled(bool enable)
	{
		m_world->setIsDebugRenderingEnabled(enable);
	}

	const ReactPhysicsRenderer* ReactPhysicsWorld::GetRenderer() const
	{
		return m_renderer.get();
	}

	void ReactPhysicsWorld::ForceRenderingRefresh()
	{
		m_world->forceGenerateRenderingPrimitives();
		if (m_world->getIsDebugRenderingEnabled())
		{
			m_renderer->GenerateRenderData();
		}
	}

	rp3d::PhysicsWorld* ReactPhysicsWorld::GetNative()
	{
		return m_world;
	}


//--------------------------------------------------------------------------------
// Physics Resolution
//--------------------------------------------------------------------------------
	void ReactPhysicsWorld::UpdateRigidBody(TimeStep deltaTime, ReactRigidBody* body)
	{
		// Using Euler integration to update the position and rotation of the rigidbody
		// F = ma with a constant F and mass, therefore acceleration never changes
		if (body->GetType() != RigidBody::Type::Dynamic)
		{
			return;
		}

		// calculate the new linear velocity
		Math::vec3 linearVelocity = body->GetLinearVelocity();
		if (body->GetHasGravity())
		{
			linearVelocity +=  m_props.gravity * deltaTime.Seconds();
		}

		// calculate the new angular velocity
		Math::vec3 angularVelocity = body->GetAngularVelocity();

		// get the current pose of the body
		Math::vec3 position;
		Math::quat rotation;
		body->GetTransform(position, rotation);

		// calculate the new position of the body
		Math::vec3 deltaLinearVelocity = linearVelocity * deltaTime.Seconds();
		Math::vec3 newPosition = position + deltaLinearVelocity;

		// calculate the new orientation of the body
		Math::quat deltaRotation = Math::quat(angularVelocity * deltaTime.Seconds()) * rotation;
		Math::quat newRotation = Math::normalize(deltaRotation);

		// apply the new orientation to the body
		body->SetTransform(newPosition, newRotation);

		// apply the damping to the velocities
		float linearDamping = body->GetLinearDamping();
		float angularDamping = body->GetAngularDamping();
		linearVelocity *= Math::pow(1.0f - linearDamping, deltaTime.Seconds());
		angularVelocity *= Math::pow(1.0f - angularDamping, deltaTime.Seconds());

		// update the rigidbody with the new velocities
		body->SetLinearVelocity(linearVelocity);
		body->SetAngularVelocity(angularVelocity);
	}
}
