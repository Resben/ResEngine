/**
 * \file
 * \author Christien Alden (34119981)
*/
#include "RenderCommand.h"
#include "RenderCommandImpl.h"

namespace AEngine
{
	UniquePtr<RenderCommandImpl> RenderCommand::s_impl{ nullptr };

	void RenderCommand::Initialise(RenderLibrary graphics)
	{
		if (!s_impl)
		{
			s_impl = RenderCommandImpl::Create(graphics);
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

	void RenderCommand::SetDepthTestFunction(RenderEnum function)
	{
		s_impl->SetDepthTestFunction(function);
	}

	void RenderCommand::EnableBlend(bool value)
	{
		s_impl->EnableBlend(value);
	}

	void RenderCommand::SetBlendFunction(RenderEnum source, RenderEnum destination)
	{
		s_impl->SetBlendFunction(source, destination);
	}

	void RenderCommand::SetViewport(int x, int y, int width, int height)
	{
		s_impl->SetViewport(x, y, width, height);
	}

	void RenderCommand::PolygonMode(RenderEnum face, RenderEnum type)
	{
		s_impl->PolygonMode(face, type);
	}

	void RenderCommand::DrawIndexed(RenderEnum type, Size_t count, void* offset)
	{
		s_impl->DrawIndexed(type, count, offset);
	}

	void RenderCommand::DrawArrays(RenderEnum type, int offset, Size_t count)
	{
		s_impl->DrawArrays(type, offset, count);
	}

	RenderLibrary RenderCommand::GetLibrary()
	{
		return s_impl->GetLibrary();
	}
}
