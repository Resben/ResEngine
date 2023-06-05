#include "OpenGLGraphicsAPI.h"
#include "AEngine/Core/Logger.h"
#include <glad/glad.h>

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
		GLenum func = GL_INVALID_ENUM;

		switch (function)
		{
		case GraphicsEnum::DepthLessEqual:
			func = GL_LEQUAL;
			break;
		case GraphicsEnum::DepthGreaterEqual:
			func = GL_GEQUAL;
			break;
		case GraphicsEnum::DepthLess:
			func = GL_LESS;
			break;
		case GraphicsEnum::DepthGreater:
			func = GL_GREATER;
			break;
		case GraphicsEnum::DepthEqual:
			func = GL_EQUAL;
			break;
		default:
			AE_LOG_ERROR("OpenGLGraphicsAPI::SetDepthTestFunction: Invalid function {}", static_cast<int>(function));
			return;
		}

		glDepthFunc(func);
	}

	void OpenGLGraphicsAPI::DrawIndexed(GraphicsEnum type, Size_t count, void* offset)
	{
		GLenum mode = GL_INVALID_ENUM;
		switch (type)
		{
		case GraphicsEnum::DrawTriangles:
			mode = GL_TRIANGLES;
			break;
		case GraphicsEnum::DrawTriangleStrip:
			mode = GL_TRIANGLE_STRIP;
			break;
		case GraphicsEnum::DrawTriangleFan:
			mode = GL_TRIANGLE_FAN;
			break;
		default:
			AE_LOG_ERROR("OpenGLGraphicsAPI::DrawIndexed: Invalid type {}", static_cast<int>(type));
			return;
		}

		glDrawElements(mode, count, GL_UNSIGNED_INT, offset);
	}

	void OpenGLGraphicsAPI::DrawArrays(GraphicsEnum type, int offset, Size_t count)
	{
		GLenum mode = GL_INVALID_ENUM;

		switch (type)
		{
		case GraphicsEnum::DrawTriangles:
			mode = GL_TRIANGLES;
			break;
		case GraphicsEnum::DrawTriangleStrip:
			mode = GL_TRIANGLE_STRIP;
			break;
		case GraphicsEnum::DrawTriangleFan:
			mode = GL_TRIANGLE_FAN;
			break;
		default:
			AE_LOG_ERROR("OpenGLGraphicsAPI::DrawArrays: Invalid type {}", static_cast<int>(type));
			return;
		}

		glDrawArrays(mode, offset, count);
	}

	void OpenGLGraphicsAPI::PolygonMode(GraphicsEnum face, GraphicsEnum type)
	{
		// placeholder values
		GLenum faceEnum = GL_INVALID_ENUM;
		GLenum typeEnum = GL_INVALID_ENUM;

		switch (face)
		{
		case GraphicsEnum::FaceFront:
			faceEnum = GL_FRONT;
			break;
		case GraphicsEnum::FaceBack:
			faceEnum = GL_BACK;
			break;
		case GraphicsEnum::FaceFrontAndBack:
			faceEnum = GL_FRONT_AND_BACK;
			break;
		default:
			AE_LOG_ERROR("OpenGLGraphicsAPI::PolygonMode: Invalid face type {}", static_cast<int>(face));
			return;
		}

		switch (type)
		{
		case GraphicsEnum::PolygonFill:
			typeEnum = GL_FILL;
			break;
		case GraphicsEnum::PolygonLine:
			typeEnum = GL_LINE;
			break;
		default:
			AE_LOG_ERROR("OpenGLGraphicsAPI::PolygonMode: Invalid type {}", static_cast<int>(type));
			return;
		}

		// make OpenGL call
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