#pragma once
#include <string>
#include <EnTT/entt.hpp>
#include "../Core/IntervalTimer.h"
#include "../Core/TimeStep.h"
#include "Components.h"
#include "DebugCamera.h"

namespace AEngine
{
		// forward declaration
	class Entity;

		/**
		 * @class Scene
		 * @brief Scene Class used by the ECS
		 * @author Geoff Candy (34183006)
		 * @author Christien Alden (34119981)
		**/
	class Scene
	{
	public:
			/**
			 * @brief Method to create Entities within the Scene
			 * @param[in] name of entity; default is "DefaultEntity"
			 * @return Entity
			**/
		Entity CreateEntity(const std::string& name = std::string());

		Entity GetEntity(const std::string& tag);
		Entity GetEntity(uint32_t id);

			/**
			 * @brief Updates scene cameras to reflect the new aspect ratio
			 * @param[in] width of scene
			 * @param[in] height of scene
			 * @return void
			**/
		void OnViewportResize(unsigned int width, unsigned int height);

			/**
			 * @brief Sets the active camera for the scene
			 * @param[in] entityTag of entity with CameraComponent
			 * @retval true if active camera was updated
			 * @retval false if active camera was **not** updated
			 *
			 * Searches for an entity with a camera that matches \p entityTag,
			 * the active camera is only changed if a matching entity if found.
			**/
		bool SetActiveCamera(const std::string& entityTag);

			/**
			 * @brief Sets whether the scene uses the debug camera or an entity camera
			 * @param[in] value true to use debug cam; false to revert to entity cam
			 * @return void
			 * @bug Currently controllable entities are still updated
			**/
		void SetDebugCamera(bool value);

			/**
			 * @brief Returns status of Scene::SetDebugCamera
			 * @retval true if using debug camera
			 * @retval false if **not** using debug camera
			**/
		bool IsUsingDebugCamera() const;

			/**
			 * @brief Returns the debug camera reference to update properties
			 * @return DebugCamera&
			**/
		DebugCamera& GetDebugCamera();

			/**
			 * @brief Updates the scene during runtime
			 * @return void
			 *
			 * This should be called each frame just before the window is refreshed.
			**/
		void OnUpdate();

			/**
			 * @brief Initialises scene
			 * @retval void
			**/
		void Init(unsigned int updatesPerSecond = 60);

			/**
			 * @brief Pauses scene simulation
			 * @retval void
			**/
		void Pause();

			/**
			 * @brief Returns running state of simulation
			 * @retval true if simulation is running
			 * @retval false if simulation is **not** running
			**/
		bool IsRunning();

			/**
			 * @brief Resumes scene simulation
			 * @retval void
			**/
		void Resume();

	private:
		friend class Entity;
		entt::registry m_Registry;			///< EnTT registry for this scene
		uint32_t nextId{ 0 };

		DebugCamera m_debugCam;				///< Scene debug camera
		bool m_useDebugCamera{ false };		///< Scene debug camera active state
		
		IntervalTimer sceneClock;

		float m_width, m_height;

			/**
			 * @brief Calls modern render system with the given camera
			 * @param[in] activeCam to render scene from
			 * @return void
			**/
		void RenderOnUpdate(const PerspectiveCamera& activeCam);

			/**
			 * @brief Updates cameras in scene
			 * @retval PerspectiveCamera* if there is an active camera
			 * @retval nullptr is there was no active camera
			 * @warning If function returns nullptr, the program will terminate
			 * @note For optimisation reasons, only the active camera will have
			 * its view matrix updated.
			**/
		PerspectiveCamera* CamerasOnUpdate();
	};
}
