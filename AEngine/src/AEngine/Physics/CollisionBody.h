#pragma once
#include "AEngine/Math/Math.h"
#include "Collider.h"

namespace AEngine
{
    class CollisionBody
    {
    public:
        virtual ~CollisionBody() = default;

        virtual void SetTransform(const Math::vec3& position, const Math::quat& orientation) = 0;
        virtual void GetTransform(Math::vec3& position, Math::quat& orientation) const = 0;

        virtual Collider* AddBoxCollider(const Math::vec3& size) = 0;
        virtual Collider* AddSphereCollider(float radius) = 0;
        virtual Collider* AddCapsuleCollider(float radius, float height) = 0;
        virtual Collider* AddHeightMapCollider(int squareSize, float minHeight, float maxHeight, float* data, Math::vec3 scale) = 0;

        virtual void RemoveCollider(Collider* collider) = 0;
        
        virtual void GetInterpolatedTransform(Math::vec3& position, Math::quat& orientation) = 0;
    };

    class RigidBody : public CollisionBody
    {
    public:
        enum class AE_RigidBodyType {
            STATIC, KINEMATIC, DYNAMIC
        };

    public:
        virtual ~RigidBody() = default;

        virtual void SetHasGravity(bool hasGravity) = 0;
        virtual bool GetHasGravity() const = 0;

        virtual void SetMass(float massKg) = 0;
        virtual float GetMass() const = 0;

        virtual void SetVelocity(Math::vec3 velocity) = 0;
        virtual const Math::vec3 GetVelocity() const = 0;
        virtual void SetDrag(float factor) = 0;

        virtual void SetType(AE_RigidBodyType type) = 0;
    };
}
