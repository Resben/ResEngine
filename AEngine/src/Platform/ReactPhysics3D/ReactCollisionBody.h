#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include "AEngine/Math/Math.hpp"
#include "AEngine/Physics/Collider.h"
#include "AEngine/Physics/CollisionBody.h"

namespace AEngine
{
    class ReactCollisionBody : public CollisionBody
    {
    public:
        ReactCollisionBody(rp3d::PhysicsWorld* world, const Math::vec3& position, const Math::quat& orientation, bool isRigid = false);
        virtual ~ReactCollisionBody() = default;
       
        virtual void SetTransform(const Math::vec3&, const Math::quat&) override;
        virtual void GetTransform(Math::vec3&, Math::quat&) const override;

        virtual Collider* AddBoxCollider(const Math::vec3& size) override;
        virtual Collider* AddSphereCollider(float radius) override;

        virtual void RemoveCollider(Collider* collider) override;

		rp3d::CollisionBody* GetNative() const;

    protected:
        rp3d::CollisionBody* m_body;
    };

    class ReactRigidBody : public RigidBody
    {
    public:
        ReactRigidBody(rp3d::PhysicsWorld* world, const Math::vec3& position, const Math::quat& orientation);
        virtual ~ReactRigidBody();

        virtual void SetHasGravity(bool hasGravity) override;
        virtual bool GetHasGravity() const override;

        virtual void SetMass(float massKg) override;
        virtual float GetMass() const override;

//--------------------------------------------------------------------------------
// From CollisionBody
//--------------------------------------------------------------------------------
        virtual void SetTransform(const Math::vec3&, const Math::quat&) override;
        virtual void GetTransform(Math::vec3&, Math::quat&) const override;

        virtual Collider* AddBoxCollider(const Math::vec3& size) override;
        virtual Collider* AddSphereCollider(float radius) override;

        virtual void RemoveCollider(Collider* collider) override;

        virtual void SetType(AE_RigidBodyType type) override;
		rp3d::RigidBody* GetNative() const;      
    
    private:
        ReactCollisionBody* m_body;
    };
}
