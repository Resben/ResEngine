#pragma once
#include <AEngine/Core/Window.h>
#include <AEngine/Scene/Entity.h>

namespace AEngine
{
	class Scene;
	struct EditorProperties
	{
		bool TitleBarMove = true;
		bool IsDockingEnabled = true;
		bool IsViewportEnabled = true;
	};
	class Editor
	{
	public:
		void Init(Window* window, const EditorProperties& props);

		void CreateNewFrame();

		void Update();

		void Render();

		void Shutdown();

	private:
		Scene* m_scene;
		Entity m_selectedEntity;


		void ShowDebugCameraConfig();
		void ShowDebugWindow();
		void ShowHierarchy();
		void ShowInspector();

		//Methods for the Components in ECS
		void ShowTagComponent();
		void ShowTransformComponent();
		void ShowRenderableComponent();
		void ShowSkinnedRenderableComponent();
		void ShowTextComponent();
		void ShowTerrainComponent();
		void ShowSkyboxComponent();
		void ShowWaterComponent();
		void ShowCameraComponent();
		void ShowScriptableComponent();
		void ShowPhysicsHandle();
		void ShowRigidBodyComponent();
		void ShowBoxColliderComponent();
		void ShowHeightMapColliderComponent();
		void ShowPlayerControllerComponent();

		//Do we need ImGui stuff for Assets like textures, shaders?
		void ShowAddComponentButton();

		template <typename T>
		void ShowAddComponentPrompt(const char* label);

		template <typename T>
		void ShowRemoveButton();

		template <typename T>
		SharedPtr<T> AssetPopupMenu(const std::string& label);

		//other editor features play and pause?

		//add entities and add components?

	};
}
