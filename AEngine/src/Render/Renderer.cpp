/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Interface to setup and submit jobs to renderer
**/
#include <glad/glad.h>
#include "Renderer.h"

namespace AEngine
{
	Renderer* Renderer::s_instance{ nullptr };

	Renderer* Renderer::Instance()
	{
		if (!s_instance)
		{
			s_instance = new Renderer;
		}

		return s_instance;
	}

	void Renderer::SetProjection(const glm::mat4& projectionMat, const struct Light& light)
	{
		m_projectionView = projectionMat;
		m_light = light;
	}

	void Renderer::Submit(const Mesh& mesh, const Texture& texture, Shader& shader, const glm::mat4& transform)
	{
		shader.Bind();
		texture.Bind();
		mesh.Bind();

		shader.SetUniformInteger("u_texture1", 0);
		shader.SetUniformMat4("u_transform", transform);
		shader.SetUniformMat4("u_projectionView", m_projectionView);
		shader.SetUniformFloat3("u_lightPos", m_light.pos);
		shader.SetUniformFloat3("u_lightColour", m_light.colour);
		shader.SetUniformFloat("u_ambient", 0.20f);
		shader.SetUniformMat3("u_normalMatrix", glm::mat3(glm::transpose(glm::inverse(transform))));

		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mesh.GetVertices().size()));

		texture.Unbind();
		mesh.Unbind();
		shader.Unbind();
	}
}
