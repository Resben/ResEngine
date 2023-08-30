#pragma once
#include <AEngine/Core/Window.h>

namespace AEngine
{
	class Editor
	{
	public:
		void Init(Window* window);

		void CreateNewFrame();

		void Update();

		void Render();

		void Shutdown();
	};
}


