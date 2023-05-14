#pragma once
#include "AEngine/Core/Types.h"
#include "AEngine/Math/Math.hpp"

namespace AEngine
{
	enum class GraphicsType
	{
		NONE = 0, OPENGL
	};

	enum class AE_TYPES
	{
		AE_FILL, AE_LINE
	};

	class GraphicsAPI
	{
	public:
		GraphicsAPI() = default;
		virtual void SetClearColor(Math::vec4 color) = 0;

		virtual void Clear() = 0;

		virtual void DrawIndexed(unsigned int size) = 0;

		virtual void PolygonMode(enum AE_TYPES type) = 0;

		virtual void EnableDepth(bool set) = 0;

		virtual void SetViewport(int x, int y, int width, int height) = 0;

		virtual GraphicsType GetType() = 0;

		static UniquePtr<GraphicsAPI> Create(GraphicsType graphics);
	};
}
