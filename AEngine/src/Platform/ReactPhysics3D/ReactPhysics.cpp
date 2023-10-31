/**
 * \file   ReactPhysics.cpp
 * \author Lane O'Rafferty (33534304)
 * \author Christien Alden (34119981)
*/
#include "AEngine/Core/Logger.h"
#include "AEngine/Core/TimeStep.h"
#include "AEngine/Core/Types.h"
#include "ReactPhysics.h"
#include "ReactCollisionBody.h"
#include "AEngine/Math/Math.h"

namespace {
	// lookup table for the penetration depth multiplier based on the body types
	// passing in the types as an index will give the multipliers for the
	// first and second bodies
	// first body is on the row, and second on the column
	constexpr AEngine::Math::vec2 g_penetrationDepthMultiplier[3][3] = {
		//  static         kinematic      dynamic
		{{ 0.0f, 0.0f}, { 0.0f, 0.0f}, { 0.0f, 1.0f}},   // static
		{{ 0.0f, 0.0f}, { 0.0f, 0.0f}, { 0.0f, 1.0f}},   // kinematic
		{{-1.0f, 0.0f}, {-1.0f, 0.0f}, {-0.5f, 0.5f}}    // dynamic
	};
}

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

			// get the collision body from each contact pair then obtain the user data
			// if at least one of the bodies has no user data (nullptr), then we have collided
			// with a non-rigidbody and we can ignore the collision
			rp3d::CollisionBody* body1 = contactPair.getBody1();
			rp3d::CollisionBody* body2 = contactPair.getBody2();
			void* userData1 = body1->getUserData();
			void* userData2 = body2->getUserData();
			if (!userData1 || !userData2)
			{
				continue;
			}

			//--------------------------------------------------------------------------------
			// Collision Resolution
			//--------------------------------------------------------------------------------

			// retrieve the ReactCollisionBody from the user data
			ReactRigidBody* reactBody1 = static_cast<ReactRigidBody*>(userData1);
			ReactRigidBody* reactBody2 = static_cast<ReactRigidBody*>(userData2);
			ReactRigidBody::Type type1 = reactBody1->GetType();
			ReactRigidBody::Type type2 = reactBody2->GetType();

			// if neither body is dynamic, then we can ignore the collision
			if (type1 != RigidBody::Type::Dynamic && type2 != RigidBody::Type::Dynamic)
			{
				continue;
			}

			// get the averaged collision data from the contact pairs
			CollisionData collisionData;
			AverageCollisionPoints(contactPair, collisionData);


			// numerator
			//float restitution = std::min(reactBody1->GetRestitution(), reactBody2->GetRestitution());
			float restitution = CalculateCombinedRestitution(reactBody1->GetMass(), reactBody2->GetMass(), reactBody1->GetRestitution(), reactBody2->GetRestitution());
			Math::vec3 relativeVelocity = reactBody1->GetLinearVelocity() - reactBody2->GetLinearVelocity();
			Math::vec3 angularVelocity1 = reactBody1->GetAngularVelocity();
			Math::vec3 angularVelocity2 = reactBody2->GetAngularVelocity();
			Math::vec3 radius1 = collisionData.contactPoint1 - reactBody1->GetCentreOfMassWorldSpace();
			Math::vec3 radius2 = collisionData.contactPoint2 - reactBody2->GetCentreOfMassWorldSpace();

			float numerator =
				-(1.0f + restitution) * (
				Math::dot(collisionData.contactNormal, relativeVelocity) +
				Math::dot(angularVelocity1, Math::cross(radius1, collisionData.contactNormal)) -
				Math::dot(angularVelocity2, Math::cross(radius2, collisionData.contactNormal))
				);

			// denominator
			float totalInverseMass = reactBody1->GetInverseMass() + reactBody2->GetInverseMass();
			Math::mat3 invInertia1 = reactBody1->GetWorldInverseInertiaTensor();
			Math::mat3 invInertia2 = reactBody2->GetWorldInverseInertiaTensor();

			Math::vec3 r1xn = Math::cross(radius1, collisionData.contactNormal);
			Math::vec3 r2xn = Math::cross(radius2, collisionData.contactNormal);

			float denominator =
				totalInverseMass +
				Math::dot(r1xn, invInertia1 * r1xn) +
				Math::dot(r2xn, invInertia2 * r2xn);

			// calculate the impulse scalar (lambda) and linear impulse
			float lambda = numerator / denominator;
			Math::vec3 linearImpulse = lambda * collisionData.contactNormal;

			// calculate the adjusted penetration based on the types of the bodies, e.g., if a dynamic body collides with
			// a non-dynamic body then it should only depenetrate itself.
			Math::vec2 adjustedPenetration =
				g_penetrationDepthMultiplier[static_cast<int>(type1)][static_cast<int>(type2)] * collisionData.penetrationDepth;
			DepenetrateBody(reactBody1, adjustedPenetration[0], collisionData.contactNormal);
			DepenetrateBody(reactBody2, adjustedPenetration[1], collisionData.contactNormal);

			// AE_LOG_DEBUG("Lambda = {}", lambda);
			bool shouldApplyForces = std::abs(lambda) >= PhysicsWorld::s_lambdaMinimum;

			// if the lambda is less than the minimum, then we can ignore the collision
			if (!shouldApplyForces)
			{
				continue;
			}

			// apply the forces to dynamic bodies
			if (type1 == RigidBody::Type::Dynamic)
			{
				ApplyLinearImpulse(reactBody1, linearImpulse);
				ApplyAngularImpulse(reactBody1, lambda, invInertia1, radius1, collisionData.contactNormal);
			}

			if (type2 == RigidBody::Type::Dynamic)
			{
				ApplyLinearImpulse(reactBody2, -linearImpulse);
				ApplyAngularImpulse(reactBody2, -lambda, invInertia2, radius2, collisionData.contactNormal);
			}
		}
	}

	void ReactCollisionResolver::ApplyLinearImpulse(
		ReactRigidBody* body,
		const Math::vec3 &impulse)
	{
		Math::vec3 deltaLinearVelocity = impulse * body->GetInverseMass();
		body->SetLinearVelocity(body->GetLinearVelocity() + deltaLinearVelocity);
	}

	void ReactCollisionResolver::ApplyAngularImpulse(
		ReactRigidBody* body,
		float lambda,
		const Math::mat3 &inverseInertiaTensor,
		const Math::vec3 &radius,
		const Math::vec3 &contactNormal
	)
	{
		Math::vec3 deltaAngularVelocity = lambda * inverseInertiaTensor * Math::cross(radius, contactNormal);
		body->SetAngularVelocity(body->GetAngularVelocity() + deltaAngularVelocity);
	}

	void ReactCollisionResolver::DepenetrateBody(ReactRigidBody* body, float penetrationDepth, const Math::vec3& normal)
	{
		// if neither body is dynamic, then we can ignore the collisio
		Math::vec3 position;
		Math::quat rotation;
		body->GetTransform(position, rotation);
		position += normal * (penetrationDepth * 0.5f);
		body->SetTransform(position, rotation);
	}

	void ReactCollisionResolver::AverageCollisionPoints(const CollisionCallback::ContactPair &contactPair, CollisionData &collisionData)
	{
		// store local to world transforms for the colliders
		rp3d::Transform body1Transform = contactPair.getCollider1()->getLocalToWorldTransform();
		rp3d::Transform body2Transform = contactPair.getCollider2()->getLocalToWorldTransform();

		rp3d::Vector3 contactNormal{ 0.0f, 0.0f, 0.0f };
		rp3d::Vector3 contactPoint1{ 0.0f, 0.0f, 0.0f };
		rp3d::Vector3 contactPoint2{ 0.0f, 0.0f, 0.0f };
		rp3d::decimal penetrationDepth = 0.0f;

		// iterate through each contact point in the contact pair
		unsigned int nbContactPoints = contactPair.getNbContactPoints();
		for (unsigned int j = 0; j < nbContactPoints; ++j)
		{
			CollisionCallback::ContactPoint contactPoint = contactPair.getContactPoint(j);

			// add collision data to the totals
			contactNormal += contactPoint.getWorldNormal();
			penetrationDepth += contactPoint.getPenetrationDepth();
			contactPoint1 += contactPoint.getLocalPointOnCollider1();
			contactPoint2 += contactPoint.getLocalPointOnCollider2();
		}

		// average the collision data
		contactNormal /= static_cast<float>(nbContactPoints);
		penetrationDepth /= static_cast<float>(nbContactPoints);
		contactPoint1 /= static_cast<float>(nbContactPoints);
		contactPoint2 /= static_cast<float>(nbContactPoints);

		// convert contact points to world space
		contactPoint1 = body1Transform * contactPoint1;
		contactPoint2 = body2Transform * contactPoint2;

		// convert to AE types and store in the collision data
		collisionData.contactNormal = Math::normalize(RP3DToAEMath(contactNormal));
		collisionData.penetrationDepth = static_cast<float>(penetrationDepth);
		collisionData.contactPoint1 = RP3DToAEMath(contactPoint1);
		collisionData.contactPoint2 = RP3DToAEMath(contactPoint2);
	}

	float ReactCollisionResolver::CalculateCombinedRestitution(float mass1, float mass2, float restitution1, float restitution2)
	{
		return (mass1 * restitution1 + mass2 * restitution2) / (mass1 + mass2);
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
		: m_world(nullptr), m_accumulator(0), m_renderer{ nullptr }, m_collisionResolver{}
	{
		m_world = common->createPhysicsWorld();
		m_world->setIsDebugRenderingEnabled(false);
		m_world->setEventListener(&m_collisionResolver);
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
		Math::vec3 angularVelocity = body->GetAngularVelocity();
		if (body->GetHasGravity())
		{
			linearVelocity +=  m_props.gravity * deltaTime.Seconds();
		}

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

		// apply damping to the velocities
		linearVelocity *= Math::pow(1.0f - body->GetLinearDamping(), deltaTime.Seconds());
		angularVelocity *= Math::pow(1.0f - body->GetAngularDamping(), deltaTime.Seconds());
		body->SetLinearVelocity(linearVelocity);
		body->SetAngularVelocity(angularVelocity);
	}
}
