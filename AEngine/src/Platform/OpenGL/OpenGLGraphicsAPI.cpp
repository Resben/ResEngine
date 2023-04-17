#include "OpenGLGraphicsAPI.h"
#include <glad/glad.h>

namespace AEngine
{
	void OpenGLGraphicsAPI::SetClearColor(Math::vec4 color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLGraphicsAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLGraphicsAPI::DrawIndexed(unsigned int size)
	{
		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	}

	void OpenGLGraphicsAPI::PolygonMode(enum AE_TYPES type)
	{
		if (type == AE_TYPES::AE_FILL)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else if (type == AE_TYPES::AE_LINE)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void OpenGLGraphicsAPI::EnableDepth(bool set)
	{
		if (set)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}

	void OpenGLGraphicsAPI::SetViewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}

	GraphicsType OpenGLGraphicsAPI::GetType()
	{
		return GraphicsType::OPENGL;
	}
}