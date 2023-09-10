#pragma once
#include <AEngine/Core/Window.h>


namespace AEngine
{
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

		void ShowGameViewPort();

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

		//other editor features play and pause?

		//add entities and add components?

		Uint16 m_inspectorId;
	};
}


