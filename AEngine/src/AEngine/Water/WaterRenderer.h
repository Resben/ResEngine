#pragma once

#include <set>

#include "AEngine/Core/PerspectiveCamera.h"
#include "AEngine/Water/Water.h"
#include "AEngine/Water/WaterFrameBuffer.h"

namespace AEngine
{
	class WaterRenderer
	{
	public:
		WaterRenderer();

		~WaterRenderer();

		void Register(Water* water);
		void UnRegister(Water* water);

		void Render(WaterFrameBuffer* waterFrameBuffer);
	private:

		PerspectiveCamera* camera;
		std::set<Water*> waters;

	};
}