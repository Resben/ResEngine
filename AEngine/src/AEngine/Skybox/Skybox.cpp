/**
 * \file
 * \author Geoff Candy
*/
#include "Skybox.h"
#include "AEngine/Core/Application.h"
#include "AEngine/Core/Types.h"
#include "AEngine/Render/RenderCommand.h"

namespace {
	constexpr AEngine::Size_t g_numVerts = 108;
	constexpr AEngine::Size_t g_numIndices = 36;
	const float g_vertices[g_numVerts] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
}

namespace AEngine
{
	Skybox::Skybox(const std::vector<std::string>& texturePaths)
		: m_mesh{ g_vertices, g_numVerts, g_numIndices }, m_texture{ texturePaths }
	{

	}

	void Skybox::Render(Shader& shader, const Math::mat4& projection, const Math::mat4& view)
	{
		RenderCommand::SetDepthTestFunction(GraphicsEnum::DepthLessEqual);

		shader.Bind();
		m_mesh.Bind();
		m_texture.Bind();

		// cast away translation
		shader.SetUniformMat4("u_projectionView", projection * Math::mat4(Math::mat3(view)));
		RenderCommand::DrawArrays(GraphicsEnum::DrawTriangles, 0, m_mesh.GetNumIndices());

		m_mesh.Unbind();
		m_texture.Unbind();
		shader.Unbind();

		RenderCommand::SetDepthTestFunction(GraphicsEnum::DepthLess);
	}
}
