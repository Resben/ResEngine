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
		void CreateHierarchy();

		void CreateInspector();

		//Methods for the Components in ECS
		void CreateTagComponent();
		void CreateTransformComponent();
		void CreateRenderableComponent();
		void CreateSkinnedRenderableComponent();
		void CreateTextComponent();
		void CreateTerrainComponent();
		void CreateSkyboxComponent();
		void CreateWaterComponent();
		void CreateCameraComponent();
		void CreateScriptableComponent();
		void CreatePhysicsHandle();
		void CreateRigidBodyComponent();
		void CreateBoxColliderComponent();
		void CreateHeightMapColliderComponent();
		void CreatePlayerControllerComponent();

		//Do we need ImGui stuff for Assets like textures, shaders?

		//other editor features play and pause?

		//add entities and add components?

		Uint16 m_inspectorId;
	};
}


