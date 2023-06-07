/**
 * \file
 * \author Ben Hawkins
 * \author Christien Alden (34119981)
*/
#include "OpenGLRenderCommand.h"
#include "AEngine/Core/Logger.h"
#include <array>
#include <glad/glad.h>

namespace
{
		/// \todo Rework this to be less prone to errors
	static constexpr std::array<GLenum, 32> g_glEnums = {
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
	void OpenGLRenderCommand::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRenderCommand::SetClearColor(const Math::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRenderCommand::EnableDepthTest(bool set)
	{
		set ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	}

	void OpenGLRenderCommand::SetDepthTestFunction(RenderEnum function)
	{
		GLenum func = g_glEnums[static_cast<int>(function)];
		glDepthFunc(func);
	}

	void OpenGLRenderCommand::EnableBlend(bool value)
	{
		value ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
	}

	void OpenGLRenderCommand::SetBlendFunction(RenderEnum source, RenderEnum destination)
	{
		GLenum src = g_glEnums[static_cast<int>(source)];
		GLenum dst = g_glEnums[static_cast<int>(destination)];
		glBlendFunc(src, dst);
	}

	void OpenGLRenderCommand::SetViewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRenderCommand::PolygonMode(RenderEnum face, RenderEnum type)
	{
		GLenum faceEnum = g_glEnums[static_cast<int>(face)];
		GLenum typeEnum = g_glEnums[static_cast<int>(type)];
		glPolygonMode(faceEnum, typeEnum);
	}

	void OpenGLRenderCommand::DrawIndexed(RenderEnum type, Size_t count, void* offset)
	{
		GLenum mode = g_glEnums[static_cast<int>(type)];
		glDrawElements(mode, count, GL_UNSIGNED_INT, offset);
	}

	void OpenGLRenderCommand::DrawArrays(RenderEnum type, int offset, Size_t count)
	{
		GLenum mode = g_glEnums[static_cast<int>(type)];
		glDrawArrays(mode, offset, count);
	}

	RenderLibrary OpenGLRenderCommand::GetLibrary()
	{
		return RenderLibrary::OpenGL;
	}
}