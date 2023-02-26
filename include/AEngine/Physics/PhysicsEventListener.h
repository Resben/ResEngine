
#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <reactphysics3d/engine/EventListener.h>

namespace r3d = reactphysics3d;

namespace React
{
	/**
	 * @class PhysicsEventListener
	 * @brief PhysicsEventListener Class used by the Physics for PvB
	 * @author Geoff Candy (34183006)
	 * @version 01
	 * @date 26/10/2022
	**/
	class PhysicsEventListener : public r3d::EventListener
	{
		/**
		 * @brief function pointer used inside overridden methods
		 * @param[out] body1,body2 userDataPointer of each collided body
		 */
		typedef void (*funcptr)(void* body1, void* body2);

	public:
		/**
		 * @brief Method to register the function pointer used within the onContact Method
		 * @param funcptr
		 */
		void registerOnContact(funcptr f1);

		/**
		 * @brief Method to register the function pointer used within the onTrigger Method
		 * @param funcptr
		 */
		void registerOnTrigger(funcptr f2);

	private:

		/**
		 * @brief Overridden Method from React EventListener used to determine Collision events
		 * @param CollisionCallback::CallbackData&
		 */
		void onContact(const r3d::CollisionCallback::CallbackData& callbackData) override;

		/**
		 * @brief Overridden Method from React EventListener used to determine Trigger events
		 * @param OverlapCallback::CallbackData&
		 */
		void onTrigger(const r3d::OverlapCallback::CallbackData& callbackData) override;

		funcptr m_contact{ nullptr };
		funcptr m_trigger{ nullptr };
	};
}