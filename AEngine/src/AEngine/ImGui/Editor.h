#pragma once
#include <AEngine/Core/Window.h>

namespace AEngine
{
	struct EditorProperties
	{
		bool TitleBarMove = true;
		bool IsDockingEnabled = true;
		bool IsViewportEnabled = false;
	};
	class Editor
	{
	public:
		void Init(Window* window, const EditorProperties& props);

		void CreateNewFrame();

		void Update();

		void Render();

		void Shutdown();

		bool WantCaptureMouse();

		bool WantCaptureKeyboard();
	};
}


