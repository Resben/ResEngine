#include "AEngine/Core/Logger.h"
#include "AEngine/Physics/PhysicsEventListener.h"

namespace React
{
	void PhysicsEventListener::onContact(const r3d::CollisionCallback::CallbackData& callbackData)
	{
		for (r3d::uint p = 0; p < callbackData.getNbContactPairs(); p++)
		{
			r3d::CollisionCallback::ContactPair contactPair = callbackData.getContactPair(p);
			if (contactPair.getEventType() != r3d::CollisionCallback::ContactPair::EventType::ContactStart)
				continue;

			if (m_contact)
			{
				m_contact(
					contactPair.getBody1()->getUserData(),
					contactPair.getBody2()->getUserData()
				);
			}
		}
	}

	void PhysicsEventListener::onTrigger(const r3d::OverlapCallback::CallbackData& callbackData)
	{
		for (r3d::uint p = 0; p < callbackData.getNbOverlappingPairs(); p++)
		{
			r3d::OverlapCallback::OverlapPair overlapPair = callbackData.getOverlappingPair(p);
			if (overlapPair.getEventType() != r3d::OverlapCallback::OverlapPair::EventType::OverlapStart)
				continue;

			if (m_trigger)
			{
				m_trigger(
					overlapPair.getBody1()->getUserData(),
					overlapPair.getBody2()->getUserData()
				);
			}
		}
	}

	void PhysicsEventListener::registerOnContact(funcptr f1)
	{
		m_contact = f1;
	}

	void PhysicsEventListener::registerOnTrigger(funcptr f2)
	{
		m_trigger = f2;
	}
}
