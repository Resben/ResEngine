/**
 * \file Editor.h
 * \author Geoff Candy (34183006)
*/
#pragma once
#include <AEngine/Core/Window.h>
#include <AEngine/Scene/Entity.h>

namespace AEngine
{
	class Scene;

	/**
	 * \struct EditorProperties
	 * @brief properties for the editor
	 */
	struct EditorProperties
	{
		bool TitleBarMove = true;
		bool IsDockingEnabled = true;
		bool IsViewportEnabled = true;
	};

	/**
	 * @class Editor
	 * @brief Class to create the editor overlay
	 */
	class Editor
	{
	public:
			/**
			* @brief Initialisation of ImGui for the Editor
			* @param window
			* @param props
			*/
		void Init(Window* window, const EditorProperties& props);
			/**
			* @brief Create a New ImGui Frame
			*/
		void CreateNewFrame();
			/**
			* @brief Update for the Editor
			*/
		void Update();
		void ShowGuizmos();
		/**
		 * @brief Render call for the ImGui elements in the Editor
		 */
		void Render();
			/**
			* @brief Shutdown Method for ImGui
			*/
		void Shutdown();

		void CameraPanel(PerspectiveCamera *camera);

			/**
			 * \bug These don't do exatly what they say, really just showing
			 * the editor or not when the cursor isn't visible...
			*/
		void ShowEditor(bool show);
		void ShowEditorInSimulation(bool show);

	private:
		// properties
		Scene* m_scene;
		Entity m_selectedEntity;
		InputBuffer m_input;

		// editor
		bool m_useDebugCamera{ true };
		bool m_handleInput{ true };
		bool m_showEditor{ true };

		// imguizmo
		bool m_showGuizmos{ true };
		int m_guizmoOperation{ 0 };
		int m_guizmoMode{ 0 };
		float m_guizmoTranslateSnapInterval{ 1.0f };
		float m_guizmoRotateSnapInterval{ 45.0f };
		float m_guizmoScaleSnapInterval{ 0.05f };

		void HandleGeneralInput();
		void ControlDebugCamera();

			/**
			* @brief Method to create an ImGui Frame for the Debug Camera
			*/
		void ShowDebugCameraConfig();
			/**
			* @brief Method to create an ImGui Frame for Scene Debug Information
			*/
		void ShowDebugWindow();
		/**
		 * @brief Method to create an ImGui Frame for the Entity Heirarchy
		 */
		void ShowHierarchy();
			/**
			* @brief Method to create an ImGui Frame for the Component Inspector
			*/
		void ShowInspector();

//------------------------------------------------------------------------------
// Methods for the Components in ECS
//------------------------------------------------------------------------------
			/**
			* @brief Method to create an ImGui Frame for the Tag Component
			*/
		void ShowTagComponent();
			/**
			* @brief Method to create an ImGui Frame for the Transform Component
			*/
		void ShowTransformComponent();
			/**
			* @brief Method to create an ImGui Frame for the Renderable Component
			*/
		void ShowRenderableComponent();
			/**
		 	* @brief Method to create an ImGui Frame for the Skinned Renderable Component
		 	*/
		void ShowSkinnedRenderableComponent();
			/**
			 * @brief Method to create an ImGui Frame for the Sky Box Component
			 */
		void ShowSkyboxComponent();
			/**
			 * @brief Method to create an ImGui Frame for the Camera Component
			 */
		void ShowCameraComponent();
			/**
			 * @brief Method to create an ImGui Frame for the Scriptable Component
			 */
		void ShowScriptableComponent();
			/**
			 * @brief Method to create an ImGui Frame for the Player Controller Component
			 */
		void ShowPlayerControllerComponent();

		void ShowRectTransformComponent();
		void ShowCanvasRendererComponent();
		void ShowPanelComponent();
		void ShowTextComponent();

		void ShowBDIComponent();

		void ShowNavigationComponent();

		void ShowAddComponentButton();

		template <typename T>
		void ShowAddComponentPrompt(const char* label);

//------------------------------------------------------------------------------
// Physics
//------------------------------------------------------------------------------
		void PhysicsPanel();
		void ColliderPanel(CollisionBody* body, Collider* collider, const char* label);
		void CollisionBodyPanel(CollisionBody* body);
		void RigidBodyPanel(RigidBody* body);


		void HelpMarker(const char* label);
	};
}
