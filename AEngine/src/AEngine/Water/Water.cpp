#include "Water.h"
#include "AEngine/Render/RenderCommand.h"
#include <vector>

namespace AEngine
{
	Water::Water()
		: Water(nullptr, nullptr)
	{

	}

	Water::Water(SharedPtr<Texture> dudv, SharedPtr<Texture> normal)
		: m_dudv(dudv), m_normal(normal)
	{
		std::vector<float> quadVerts = {
			-1.0f, 0.0f,  1.0f,
			-1.0f, 0.0f, -1.0f,
			 1.0f, 0.0f, -1.0f,
			
			-1.0f, 0.0f,  1.0f,
			 1.0f, 0.0f, -1.0f,
			 1.0f, 0.0f,  1.0f
		};

		m_vertexArray = VertexArray::Create();
		SharedPtr<VertexBuffer> vbo = VertexBuffer::Create();
		vbo->SetData(quadVerts.data(), quadVerts.size() * sizeof(float), BufferUsage::StaticDraw);
		vbo->SetLayout({ { BufferElementType::Float3, false } });
		m_vertexArray->AddVertexBuffer(vbo);
	}

	void Water::Render(const Math::mat4& transform, const Shader& shader, const Math::mat4& projectionView) const
	{
		shader.Bind();
		shader.SetUniformMat4("u_projectionView", projectionView);
		shader.SetUniformMat4("u_model", transform);

		m_vertexArray->Bind();
		const VertexBuffer* vbo = m_vertexArray->GetVertexBuffers().at(0).get();
		RenderCommand::DrawArrays(PrimitiveDraw::Triangles, 0, vbo->GetCount());
		m_vertexArray->Unbind();

		shader.Unbind();
	}

	void Water::SetDUDV(SharedPtr<Texture> dudv)
	{
		m_dudv = dudv;
	}

	void Water::SetNormal(SharedPtr<Texture> normal)
	{
		m_normal = normal;
	}

	Texture *Water::GetDUDV()
	{
		return m_dudv.get();
	}

	Texture *Water::GetNormal()
	{
		return m_normal.get();
	}
}

