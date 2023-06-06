/**
 * \file
 * \author Christien Alden (34119981)
*/
#include "RenderCommand.h"
#include "GraphicsAPI.h"

namespace AEngine
{
	UniquePtr<GraphicsAPI> RenderCommand::s_impl{ nullptr };

	void RenderCommand::Initialise(RenderLibrary graphics)
	{
		if (!s_impl)
		{
			s_impl = GraphicsAPI::Create(graphics);
		}
	}

	void RenderCommand::Clear()
	{
		s_impl->Clear();
	}

	void RenderCommand::SetClearColor(const Math::vec4& color)
	{
		s_impl->SetClearColor(color);
	}

	void RenderCommand::EnableDepthTest(bool set)
	{
		s_impl->EnableDepthTest(set);
	}

	void RenderCommand::SetDepthTestFunction(GraphicsEnum function)
	{
		s_impl->SetDepthTestFunction(function);
	}

	void RenderCommand::EnableBlend(bool value)
	{
		s_impl->EnableBlend(value);
	}

	void RenderCommand::SetBlendFunction(GraphicsEnum source, GraphicsEnum destination)
	{
		s_impl->SetBlendFunction(source, destination);
	}

	void RenderCommand::SetViewport(int x, int y, int width, int height)
	{
		s_impl->SetViewport(x, y, width, height);
	}

	void RenderCommand::PolygonMode(GraphicsEnum face, GraphicsEnum type)
	{
		s_impl->PolygonMode(face, type);
	}

	void RenderCommand::DrawIndexed(GraphicsEnum type, Size_t count, void* offset)
	{
		s_impl->DrawIndexed(type, count, offset);
	}

	void RenderCommand::DrawArrays(GraphicsEnum type, int offset, Size_t count)
	{
		s_impl->DrawArrays(type, offset, count);
	}

	RenderLibrary RenderCommand::GetLibrary()
	{
		return s_impl->GetLibrary();
	}
}
