/**
 * \file   ReactCollider.cpp
 * \author Lane O'Rafferty (33534304)
 * \author Christien Alden (34119981)
*/

#include <reactphysics3d/reactphysics3d.h>
#include "ReactCollider.h"

namespace AEngine
{
    ReactCollider::ReactCollider(rp3d::Collider* collider)
        : m_collider{ collider }
    {
        // no body
    }

    void ReactCollider::SetIsTrigger(bool isTrigger)
    {
        m_collider->setIsTrigger(isTrigger);
    }

    bool ReactCollider::GetIsTrigger() const
    {
        return m_collider->getIsTrigger();
    }

    rp3d::CollisionShape* ReactCollider::GetNativeShape() const
    {
        return m_collider->getCollisionShape();
    }

    ReactBoxCollider::ReactBoxCollider(rp3d::Collider* collider)
    {
        m_collider = MakeUnique<ReactCollider>(collider);
    }

    void ReactBoxCollider::SetIsTrigger(bool isTrigger)
    {
        m_collider->SetIsTrigger(isTrigger);
    }

    bool ReactBoxCollider::GetIsTrigger() const
    {
        return m_collider->GetIsTrigger();
    }

    void ReactBoxCollider::Resize(const Math::vec3& size)
    {
        rp3d::BoxShape* box = dynamic_cast<rp3d::BoxShape*>(m_collider->GetNativeShape());
        box->setHalfExtents(AEMathToRP3D(size));
    }

    Math::vec3 ReactBoxCollider::GetSize() const
    {
        rp3d::BoxShape* box = dynamic_cast<rp3d::BoxShape*>(m_collider->GetNativeShape());
        return RP3DToAEMath(box->getHalfExtents());
    }
}
