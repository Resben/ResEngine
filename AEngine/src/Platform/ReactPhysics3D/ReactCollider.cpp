#include <reactphysics3d/reactphysics3d.h>
#include "ReactCollider.h"

namespace AEngine
{
    ReactCollider::ReactCollider(rp3d::CollisionBody* body, rp3d::CollisionShape* shape)
        : m_collider(nullptr)
    {
        m_collider = body->addCollider(shape, rp3d::Transform::identity());
    }

    void ReactCollider::SetIsTrigger(bool isTrigger)
    {
        m_collider->setIsTrigger(isTrigger);
    }

    bool ReactCollider::GetIsTrigger() const
    {
        return m_collider->getIsTrigger();
    }

    rp3d::Collider* ReactCollider::GetNative()
    {
        return m_collider;
    }

    rp3d::Collider* m_collider;
}