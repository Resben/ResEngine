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

    Math::vec3 ReactCollider::GetOffset() const
    {
        rp3d::Transform transform = m_collider->getLocalToBodyTransform();
        return RP3DToAEMath(transform.getPosition());
    }

	void ReactCollider::SetOffset(const Math::vec3& offset)
    {
        // get transform, set position, then set transform in collider
        rp3d::Transform transform = m_collider->getLocalToBodyTransform();
        transform.setPosition(AEMathToRP3D(offset));
        m_collider->setLocalToBodyTransform(transform);
    }

	Math::quat ReactCollider::GetOrientation() const
    {
        rp3d::Transform transform = m_collider->getLocalToBodyTransform();
        return RP3DToAEMath(transform.getOrientation());
    }

	void ReactCollider::SetOrientation(const Math::quat& orientation)
    {
        // get transform, set orientation, then set transform in collider
        rp3d::Transform transform = m_collider->getLocalToBodyTransform();
        transform.setOrientation(AEMathToRP3D(orientation));
        m_collider->setLocalToBodyTransform(transform);
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

    Math::vec3 ReactBoxCollider::GetOffset() const
    {
        return m_collider->GetOffset();
    }

    void ReactBoxCollider::SetOffset(const Math::vec3& offset)
    {
        m_collider->SetOffset(offset);
    }

    Math::quat ReactBoxCollider::GetOrientation() const
    {
        return m_collider->GetOrientation();
    }

    void ReactBoxCollider::SetOrientation(const Math::quat& orientation)
    {
        m_collider->SetOrientation(orientation);
    }

    Math::vec3 ReactBoxCollider::GetSize() const
    {
        rp3d::BoxShape* box = dynamic_cast<rp3d::BoxShape*>(m_collider->GetNativeShape());
        return RP3DToAEMath(box->getHalfExtents());
    }
}
