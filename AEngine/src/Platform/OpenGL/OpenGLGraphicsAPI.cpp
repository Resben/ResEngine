#include "OpenGLGraphicsAPI.h"
#include "AEngine/Core/Logger.h"
#include <glad/glad.h>
#include <array>

namespace
{
		/// \todo Rework this to be less prone to errors
	static constexpr std::array<GLenum, 33> g_glEnums = {
		GL_INVALID_ENUM,

		// blend functions
		GL_ZERO, GL_ONE,
		GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR,
		GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR,
		GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA,
		GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR,
		GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA,

		// depth test functions
		GL_LEQUAL, GL_GEQUAL, GL_LESS, GL_GREATER, GL_EQUAL,

		// polygon modes
		GL_FRONT, GL_BACK, GL_FRONT_AND_BACK,

		// polygon draw modes
		GL_FILL, GL_LINE,

		// draw primitives
		GL_POINTS,
		GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP,
		GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_LINES
	};
}

namespace AEngine
{
	void OpenGLGraphicsAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLGraphicsAPI::SetClearColor(const Math::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLGraphicsAPI::EnableDepthTest(bool set)
	{
		set ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	}

	void OpenGLGraphicsAPI::SetDepthTestFunction(GraphicsEnum function)
	{
		GLenum func = g_glEnums[static_cast<int>(function)];
		if (func == GL_INVALID_ENUM)
		{
			AE_LOG_FATAL("OpenGLGraphicsAPI::SetDepthTestFunction: Invalid function {}", static_cast<int>(function));
		}

		glDepthFunc(func);
	}

	void OpenGLGraphicsAPI::EnableBlend(bool value)
	{
		value ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
	}

	void OpenGLGraphicsAPI::SetBlendFunction(GraphicsEnum source, GraphicsEnum destination)
	{
		GLenum src = g_glEnums[static_cast<int>(source)];
		GLenum dst = g_glEnums[static_cast<int>(destination)];
		if (src == GL_INVALID_ENUM)
		{
			AE_LOG_FATAL("OpenGLGraphicsAPI::SetBlendFunction: Invalid source {}", static_cast<int>(source));
		}

		if (dst == GL_INVALID_ENUM)
		{
			AE_LOG_FATAL("OpenGLGraphicsAPI::SetBlendFunction: Invalid destination {}", static_cast<int>(destination));
		}

		glBlendFunc(src, dst);
	}

	void OpenGLGraphicsAPI::SetViewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLGraphicsAPI::PolygonMode(GraphicsEnum face, GraphicsEnum type)
	{
		GLenum faceEnum = g_glEnums[static_cast<int>(face)];
		GLenum typeEnum = g_glEnums[static_cast<int>(type)];

		if (faceEnum == GL_INVALID_ENUM)
		{
			AE_LOG_FATAL("OpenGLGraphicsAPI::PolygonMode: Invalid face type {}", static_cast<int>(face));
		}

		if (typeEnum == GL_INVALID_ENUM)
		{
			AE_LOG_FATAL("OpenGLGraphicsAPI::PolygonMode: Invalid type {}", static_cast<int>(type));
		}

		glPolygonMode(faceEnum, typeEnum);
	}

	void OpenGLGraphicsAPI::DrawIndexed(GraphicsEnum type, Size_t count, void* offset)
	{
		GLenum mode = g_glEnums[static_cast<int>(type)];
		if (mode == GL_INVALID_ENUM)
		{
			AE_LOG_FATAL("OpenGLGraphicsAPI::DrawIndexed: Invalid type {}", static_cast<int>(type));
		}

		glDrawElements(mode, count, GL_UNSIGNED_INT, offset);
	}

	void OpenGLGraphicsAPI::DrawArrays(GraphicsEnum type, int offset, Size_t count)
	{
		GLenum mode = g_glEnums[static_cast<int>(type)];
		if (mode == GL_INVALID_ENUM)
		{
			AE_LOG_FATAL("OpenGLGraphicsAPI::DrawArrays: Invalid type {}", static_cast<int>(type));
		}

		glDrawArrays(mode, offset, count);
	}

	GraphicsLibrary OpenGLGraphicsAPI::GetLibrary()
	{
		return GraphicsLibrary::OpenGL;
	}
}