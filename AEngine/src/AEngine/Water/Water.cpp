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
			 1.0f, 0.0f, -1.0f,
			-1.0f, 0.0f, -1.0f,
			-1.0f, 0.0f,  1.0f,

			 1.0f, 0.0f,  1.0f,
			 1.0f, 0.0f, -1.0f,
			-1.0f, 0.0f,  1.0f
		};

		m_vertexArray = VertexArray::Create();
		SharedPtr<VertexBuffer> vbo = VertexBuffer::Create();
		vbo->SetData(quadVerts.data(), quadVerts.size() * sizeof(float), BufferUsage::StaticDraw);
		vbo->SetLayout({ { BufferElementType::Float3, false } });
		m_vertexArray->AddVertexBuffer(vbo);
	}

	void Water::Render(const Math::mat4& transform, const Shader& shader, const Math::mat4& projectionView, TimeStep dt) const
	{
		shader.Bind();
		shader.SetUniformMat4("u_projectionView", projectionView);
		shader.SetUniformMat4("u_model", transform);
		shader.SetUniformFloat("u_tilingFactor", 25.0f);

		m_dudv->Bind();
		shader.SetUniformInteger("dudvMap", 0);
		shader.SetUniformFloat("moveFactor", GetMoveFactor(dt));
		m_normal->Bind();
		shader.SetUniformInteger("normalMap", 1);

		m_vertexArray->Bind();
		const VertexBuffer* vbo = m_vertexArray->GetVertexBuffers().at(0).get();
		RenderCommand::DrawArrays(Primitive::Triangles, 0, vbo->GetCount());
		m_vertexArray->Unbind();
		m_normal->Unbind();
		m_dudv->Unbind();
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
	float Water::GetMoveFactor(TimeStep dt) const
	{
		constexpr float WAVE_SPEED = 0.03f;
		static float moveFactor = 0;
		moveFactor += WAVE_SPEED * dt;
		if(moveFactor >= 1.0)
		{
			moveFactor -= 1.0;
		}
		return moveFactor;
	}
}
