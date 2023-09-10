/**
 * \file   ReactCollider.cpp
 * \author Lane O'Rafferty (33534304)
 * \author Christien Alden (34119981)
*/

#include <reactphysics3d/reactphysics3d.h>
#include "ReactCollider.h"

namespace AEngine
{
    ReactCollider::ReactCollider(rp3d::CollisionBody* body, rp3d::CollisionShape* shape)
        : m_body{ body }, m_collider{ nullptr }
    {
        m_collider = body->addCollider(shape, rp3d::Transform::identity());
    }

    ReactCollider::~ReactCollider()
    {
        m_body->removeCollider(m_collider);
    }

    void ReactCollider::SetIsTrigger(bool isTrigger)
    {
        m_collider->setIsTrigger(isTrigger);
    }

    bool ReactCollider::GetIsTrigger() const
    {
        return m_collider->getIsTrigger();
    }
}
