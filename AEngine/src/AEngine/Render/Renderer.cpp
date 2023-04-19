/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Interface to setup and submit jobs to renderer
**/
#include <glad/glad.h>
#include "Renderer.h"
#include "AEngine/Resource/AssetManager.h"

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

	void Renderer::SetProjection(const Math::mat4& projectionMat)
	{
		m_projectionView = projectionMat;
	}

	void Renderer::Submit(const Model& model, const Shader& shader, const Math::mat4& transform)
	{
		shader.Bind();
		shader.SetUniformInteger("u_texture1", 0);
		shader.SetUniformMat4("u_transform", transform);
		shader.SetUniformMat4("u_projectionView", m_projectionView);

		std::vector<std::pair<std::shared_ptr<Mesh>, int>>::const_iterator it;
		for (it = model.begin(); it != model.end(); ++it)
		{
			/// @todo Make this work with other material types...
			std::shared_ptr<Texture> tex = AssetManager<Texture>::Instance().Get(model.GetMaterial(it->second)->DiffuseTexture);
			Mesh& mesh = *(it->first);

			tex->Bind();
			mesh.Bind();

			// draw
			unsigned int size = mesh.GetIndexCount();
			glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

			tex->Unbind();
			mesh.Unbind();
		}

		shader.Unbind();
	}
}
