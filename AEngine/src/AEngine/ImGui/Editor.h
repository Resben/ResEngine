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

		void CreateTagComponent();

		void CreateTransformComponent();

		void CreateRenderableComponent();

		Uint16 m_inspectorId;
	};
}


