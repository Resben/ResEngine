#include "AEngine/Core/Logger.h"
#include "ReactCollider.h"
#include "ReactCollisionBody.h"
#include "ReactPhysics.h"

namespace AEngine
{
//--------------------------------------------------------------------------------
// ReactCollisionBody
//--------------------------------------------------------------------------------
    ReactCollisionBody::ReactCollisionBody(rp3d::PhysicsWorld* world, const Math::vec3& position, const Math::quat& orientation, bool isRigid)
    {
        if (isRigid)
        {
            m_body = world->createRigidBody({ AEMathToRP3D(position), AEMathToRP3D(orientation) });
        }
        else
        {
            m_body = world->createCollisionBody({ AEMathToRP3D(position), AEMathToRP3D(orientation) });
        }
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

    void ReactCollisionBody::RemoveCollider(Collider* collider)
    {
        m_body->removeCollider(dynamic_cast<ReactCollider*>(collider)->GetNative());
    }

//--------------------------------------------------------------------------------
// ReactRigidBody
//--------------------------------------------------------------------------------
    ReactRigidBody::ReactRigidBody(rp3d::PhysicsWorld* world, const Math::vec3& position, const Math::quat& orientation)
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

    void ReactRigidBody::RemoveCollider(Collider* collider)
    {
        m_body->RemoveCollider(collider);
    }
}
