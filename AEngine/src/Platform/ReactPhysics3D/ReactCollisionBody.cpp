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
    ReactCollisionBody::ReactCollisionBody(ReactPhysicsWorld* world, const Math::vec3& position,
        const Math::quat& orientation, bool isRigid):
        m_world(world)
    {
        if (isRigid)
        {
            m_body = world->GetNative()->createRigidBody({ AEMathToRP3D(position), AEMathToRP3D(orientation) });
        }
        else
        {
            m_body = world->GetNative()->createCollisionBody({ AEMathToRP3D(position), AEMathToRP3D(orientation) });
        }

        m_lastTransform = m_body->getTransform();
    }

    rp3d::CollisionBody* ReactCollisionBody::GetNative() const
    {
        return m_body;
    }

    void ReactCollisionBody::SetTransform(const Math::vec3& position, const Math::quat& orientation)
    {
        rp3d::Transform transform;
        transform.setPosition(AEMathToRP3D(position));
        transform.setOrientation(AEMathToRP3D(orientation));
        m_body->setTransform(transform);
    }

    void ReactCollisionBody::GetTransform(Math::vec3& position, Math::quat& orientation) const
    {
        const rp3d::Transform& transform = m_body->getTransform();
        position = RP3DToAEMath(transform.getPosition());
        orientation = RP3DToAEMath(transform.getOrientation());
    }

    Collider* ReactCollisionBody::AddBoxCollider(const Math::vec3& size)
    {
        rp3d::PhysicsCommon* common = dynamic_cast<ReactPhysicsAPI&>(PhysicsAPI::Instance()).GetCommon();
        rp3d::BoxShape* box = common->createBoxShape(AEMathToRP3D(size));
        return new ReactCollider(m_body, box);
    }

    Collider* ReactCollisionBody::AddSphereCollider(float radius)
    {
        rp3d::PhysicsCommon* common = dynamic_cast<ReactPhysicsAPI&>(PhysicsAPI::Instance()).GetCommon();
        rp3d::SphereShape* sphere = common->createSphereShape(radius);
        return new ReactCollider(m_body, sphere);
    }

    Collider* ReactCollisionBody::AddCapsuleCollider(float radius, float height)
    {
        rp3d::PhysicsCommon* common = dynamic_cast<ReactPhysicsAPI&>(PhysicsAPI::Instance()).GetCommon();
        rp3d::CapsuleShape* capsule = common->createCapsuleShape(radius, height);
        return new ReactCollider(m_body, capsule);
    }

    Collider* ReactCollisionBody::AddHeightMapCollider(int sideLength, float minHeight, float maxHeight, float* data, const Math::vec3& scale)
    {
        rp3d::PhysicsCommon* common = dynamic_cast<ReactPhysicsAPI&>(PhysicsAPI::Instance()).GetCommon();
        rp3d::HeightFieldShape* heightField = common->createHeightFieldShape(
            sideLength,
            sideLength,
            minHeight,
            maxHeight,
            data,
            rp3d::HeightFieldShape::HeightDataType::HEIGHT_FLOAT_TYPE,
            1,
            1.f,
            AEMathToRP3D(scale)
        );
        return new ReactCollider(m_body, heightField);
    }

    void ReactCollisionBody::RemoveCollider(Collider* collider)
    {
        m_body->removeCollider(dynamic_cast<ReactCollider*>(collider)->GetNative());
    }

    void ReactCollisionBody::GetInterpolatedTransform(Math::vec3& position, Math::quat& orientation)
    {
        rp3d::Transform current = m_body->getTransform();
        rp3d::Transform interpolated = rp3d::Transform::interpolateTransforms(
            m_lastTransform,
            current,
            m_world->GetAccumulatorVal() / m_world->GetUpdateStep()
        );

        m_lastTransform = current;
        position = RP3DToAEMath(interpolated.getPosition());
        orientation = RP3DToAEMath(interpolated.getOrientation());
    }

//--------------------------------------------------------------------------------
// ReactRigidBody
//--------------------------------------------------------------------------------
    ReactRigidBody::ReactRigidBody(ReactPhysicsWorld* world, const Math::vec3& position, const Math::quat& orientation)
    {
        m_body = new ReactCollisionBody(world, position, orientation, true);
    }

    ReactRigidBody::~ReactRigidBody()
    {
        delete m_body;
    }

    rp3d::RigidBody* ReactRigidBody::GetNative() const
    {
        return dynamic_cast<rp3d::RigidBody*>(m_body->GetNative());
    }

    // rigidbody stuff

    void ReactRigidBody::SetHasGravity(bool hasGravity)
    {
    	GetNative()->enableGravity(hasGravity);
    }

    bool ReactRigidBody::GetHasGravity() const
    {
        return GetNative()->isGravityEnabled();
    }

    void ReactRigidBody::SetVelocity(const Math::vec3& velocity)
    {
        GetNative()->setLinearVelocity(AEMathToRP3D(velocity));
    }

    const Math::vec3& ReactRigidBody::GetVelocity() const
    {
        const rp3d::Vector3& velocity = GetNative()->getLinearVelocity();
        return RP3DToAEMath(velocity);
    }

    void ReactRigidBody::SetDrag(float factor)
    {
        GetNative()->setLinearDamping(factor);
    }

    void ReactRigidBody::SetMass(float massKg)
    {
        GetNative()->setMass(massKg);
    }

    float ReactRigidBody::GetMass() const
    {
        return GetNative()->getMass();
    }

    void ReactRigidBody::SetType(AE_RigidBodyType type)
    {
        switch (type)
        {
        case AE_RigidBodyType::STATIC:
            GetNative()->setType(rp3d::BodyType::STATIC);
            break;
        case AE_RigidBodyType::KINEMATIC:
            GetNative()->setType(rp3d::BodyType::KINEMATIC);
            break;
        case AE_RigidBodyType::DYNAMIC:
            GetNative()->setType(rp3d::BodyType::DYNAMIC);
            break;
        default:
        {
            AE_LOG_FATAL("ReactRigidBody::SetType::Invalid_type");
        }
        }
    }

    // collision body stuff

    void ReactRigidBody::SetTransform(const Math::vec3& position, const Math::quat& orientation)
    {
        m_body->SetTransform(position, orientation);
    }

    void ReactRigidBody::GetTransform(Math::vec3& position, Math::quat& orientation) const
    {
        m_body->GetTransform(position, orientation);
    }

    Collider* ReactRigidBody::AddBoxCollider(const Math::vec3& size)
    {
        return m_body->AddBoxCollider(size);
    }

    Collider* ReactRigidBody::AddSphereCollider(float radius)
    {
        return m_body->AddSphereCollider(radius);
    }

    Collider* ReactRigidBody::AddCapsuleCollider(float radius, float height)
    {
        return m_body->AddCapsuleCollider(radius, height);
    }

    Collider* ReactRigidBody::AddHeightMapCollider(int sideLength, float minHeight, float maxHeight, float* data, const Math::vec3& scale)
    {
        return m_body->AddHeightMapCollider(sideLength, minHeight, maxHeight, data, scale);
    }

    void ReactRigidBody::RemoveCollider(Collider* collider)
    {
        m_body->RemoveCollider(collider);
    }
    void ReactRigidBody::GetInterpolatedTransform(Math::vec3& position, Math::quat& orientation)
    {
        m_body->GetInterpolatedTransform(position, orientation);
    }
}
