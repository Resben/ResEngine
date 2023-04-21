#pragma once

#include <reactphysics3d/reactphysics3d.h>
#include <AEngine/Physics/Collider.h>
#include "ReactCollisionBody.h"

namespace AEngine
{
    class ReactCollider : public Collider
    {
    public:
        ReactCollider(rp3d::CollisionBody* body, rp3d::CollisionShape* shape);
        virtual ~ReactCollider() = default;

        virtual void SetIsTrigger(bool isTrigger) override;
        virtual bool GetIsTrigger() const override;

		rp3d::Collider* GetNative();

    private:
        rp3d::Collider* m_collider;
    };
}
