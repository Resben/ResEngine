
#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace AEngine
{
	namespace r3d = reactphysics3d;

	/**
	 * @class ReactBody
	 * @brief ReactBody Class used by the Physics for PvB
	 * @author Geoff Candy (34183006)
	 * @author Lane O'Rafferty (33534304)
	 * @version 01
	 * @date 26/10/2022
	**/
	class ReactBody
	{
	public:
		/**
		 * @brief Used to specify the type of ReactBody
		 */
		enum class BodyType
		{
			NONE,
			STATIC,
			KINEMATIC,
			DYNAMIC
		};

		ReactBody() = default;
		ReactBody(const ReactBody& other) = default;
		~ReactBody();

		/**
		 * @brief Constructor used to construct physics objects
		 * @param r3d::RigidBody*, r3d::Collider*
		 */
		ReactBody(r3d::RigidBody* body);

		/**
		 * @brief Method to apply a local force to the ReactBody using its local coordinates
		 * @param r3d::Vector3
		 */
		void ApplyLocalForce(const glm::vec3& force);

		/**
		 * @brief Method to apply a world force to the ReactBody using world coordinates
		 * @param r3d::Vector3
		 */
		void ApplyWorldForce(const glm::vec3& force);

		/**
		 * @brief Method to apply a linear speed to the ReactBody using world coordinates
		 * @param r3d::Vector3
		 */
		void ApplyLinearVelocity(const glm::vec3& velocity);

		/**
		 * @brief Method to apply a torque to the ReactBody using its local coordinates
		 * @param r3d::Vector3
		 */
		void ApplyLocalTorque(const glm::vec3& torque);

		/**
		 * @brief Method to apply an angular speed to the ReactBody using world coordinates
		 * @param r3d::Vector3
		 */
		void ApplyAngularVelocity(const glm::vec3& velocity);

		/**
		 * @brief Slows down the body's linear velocity over time
		 * @param float
		 */
		void SetLinearDamping(float factor);

		/**
		 * @brief Slows down the body's angular velocity over time
		 * @param float
		 */
		void SetAngularDamping(float factor);

		/**
		 * @brief Enables/Disables gravity on the body
		 */
		void SetGravity(bool state);

		/**
		 * @brief Method to change the Transform of a ReactBody
		 * @param r3d::Transform
		 */
		void SetTransform(const glm::vec3& position, const glm::quat& orientation);

		/**
		 * @brief Method to return the position and euler angles for the ReactBody
		 * @return void
		 */
		void GetTransform(glm::vec3& position, glm::quat& orientation);

		/**
		 * @brief Method to set the optional data in the physics body
		 * @return void
		 */
		void SetUserData(void* data);

		/**
		 * @brief Method to get the optional data in the physics body
		 * @return void
		 */
		void* GetUserData();

		/**
		 * @brief Method to convert a GLM vec3 to a React3D vec3
		 * @return void
		 */
		r3d::Vector3 ToReactVector(const glm::vec3& vec);

		/**
		 * @brief Method to turn on or off the Trigger of a ReactBody
		 */
		void SetTrigger(bool isTrigger, uint32_t index = 0);
		
		/**
		 * @brief Returns a pointer to the bound RigidBody pointer
		 */
		r3d::RigidBody* ReactBody::GetRigidBody();

	private:
		//@brief the pointer to the React3D physics body
		r3d::RigidBody* m_Body;
	};
}