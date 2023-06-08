/**
 * \file
 * \author Ben Hawkins
 * \author Christien Alden (34119981)
*/
#include "OpenGLRenderCommand.h"
#include "AEngine/Core/Logger.h"
#include <glad/glad.h>

namespace
{
	static constexpr GLenum g_glBlendFunctions[] = {
		GL_ZERO, GL_ONE,
		GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR,
		GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR,
		GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA,
		GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR,
		GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA
	};

	static constexpr GLenum g_glDepthTestFunctions[] = {
		GL_LEQUAL, GL_GEQUAL, GL_LESS, GL_GREATER, GL_EQUAL
	};

	static constexpr GLenum g_glPolygonFaces[] = {
		GL_FRONT, GL_BACK, GL_FRONT_AND_BACK
	};

	static constexpr GLenum g_glPolygonDraws[] = {
		GL_FILL, GL_LINE
	};

	static constexpr GLenum g_glWindingDirections[] = {
		GL_CW, GL_CCW
	};

	static constexpr GLenum g_glPrimitiveDraws[] = {
		GL_POINTS,
		GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP,
		GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN
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

	void OpenGLRenderCommand::SetDepthTestFunction(DepthTestFunction function)
	{
		GLenum func = g_glDepthTestFunctions[static_cast<int>(function)];
		glDepthFunc(func);
	}

	void OpenGLRenderCommand::EnableBlend(bool value)
	{
		value ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
	}

	void OpenGLRenderCommand::SetBlendFunction(BlendFunction source, BlendFunction destination)
	{
		GLenum src = g_glBlendFunctions[static_cast<int>(source)];
		GLenum dst = g_glBlendFunctions[static_cast<int>(destination)];
		glBlendFunc(src, dst);
	}

	void OpenGLRenderCommand::SetBlendConstant(const Math::vec4 &color)
	{
		glBlendColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRenderCommand::EnableFaceCulling(bool set)
	{
		set ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
	}

	void OpenGLRenderCommand::SetCullFace(PolygonFace face)
	{
		GLenum faceEnum = g_glPolygonFaces[static_cast<int>(face)];
		glCullFace(faceEnum);
	}

	void OpenGLRenderCommand::SetFrontFace(Winding direction)
	{
		GLenum directionEnum = g_glWindingDirections[static_cast<int>(direction)];
		glFrontFace(directionEnum);
	}

	void OpenGLRenderCommand::SetViewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRenderCommand::PolygonMode(PolygonFace face, PolygonDraw type)
	{
		GLenum faceEnum = g_glPolygonFaces[static_cast<int>(face)];
		GLenum typeEnum = g_glPolygonDraws[static_cast<int>(type)];
		glPolygonMode(faceEnum, typeEnum);
	}

	void OpenGLRenderCommand::DrawIndexed(PrimitiveDraw type, Intptr_t count, void* offset)
	{
		GLenum mode = g_glPrimitiveDraws[static_cast<int>(type)];
		glDrawElements(mode, static_cast<GLsizei>(count), GL_UNSIGNED_INT, offset);
	}

	void OpenGLRenderCommand::DrawArrays(PrimitiveDraw type, int offset, Intptr_t count)
	{
		GLenum mode = g_glPrimitiveDraws[static_cast<int>(type)];
		glDrawArrays(mode, offset, static_cast<GLsizei>(count));
	}

	RenderLibrary OpenGLRenderCommand::GetLibrary()
	{
		return RenderLibrary::OpenGL;
	}
}