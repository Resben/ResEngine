#include "OpenGLGraphicsAPI.h"
#include "AEngine/Core/Logger.h"
#include <glad/glad.h>
#include <array>

namespace
{
		/// \todo Rework this to be less prone to errors
	static constexpr std::array<GLenum, 15> g_glEnums = {
		GL_INVALID_ENUM,
		GL_LEQUAL, GL_GEQUAL, GL_LESS, GL_GREATER, GL_EQUAL,
		GL_FRONT, GL_BACK, GL_FRONT_AND_BACK,
		GL_FILL, GL_LINE,
		GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_LINES
	};
}

namespace AEngine
{
	void OpenGLGraphicsAPI::SetClearColor(const Math::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLGraphicsAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

	void OpenGLGraphicsAPI::EnableDepthTest(bool set)
	{
		set ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	}

	void OpenGLGraphicsAPI::SetViewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}

	GraphicsLibrary OpenGLGraphicsAPI::GetLibrary()
	{
		return GraphicsLibrary::OpenGL;
	}
}