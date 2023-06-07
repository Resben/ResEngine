/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Interface to setup and submit jobs to renderer
**/
#include <glad/glad.h>
#include "AEngine/Core/Application.h"
#include "Renderer.h"
#include "AEngine/Resource/AssetManager.h"
#include "AEngine/Core/Types.h"

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

		std::vector<std::pair<SharedPtr<Mesh>, int>>::const_iterator it;
		for (it = model.begin(); it != model.end(); ++it)
		{
			/// @todo Make this work with other material types...
			SharedPtr<Texture> tex = AssetManager<Texture>::Instance().Get(model.GetMaterial(it->second)->DiffuseTexture);
			Mesh& mesh = *(it->first);

			tex->Bind();
			mesh.Bind();

			// draw
			Application::Instance().Graphics().DrawIndexed(mesh.GetIndexCount());

			tex->Unbind();
			mesh.Unbind();
		}

		shader.Unbind();
	}

	void Renderer::SubmitAnimation(Model& model, const Shader& shader, Timer& time, const Math::mat4& transform)
	{
		shader.Bind();
		shader.SetUniformInteger("u_texture1", 0);
		shader.SetUniformMat4("u_transform", transform);
		shader.SetUniformMat4("u_projectionView", m_projectionView);

		Animation& animation = model.GetAnimation();
		animation.UpdateAnimation(time.GetDelta());

		auto transforms = animation.GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i)
			shader.SetUniformMat4("u_finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

		std::vector<std::pair<SharedPtr<Mesh>, int>>::const_iterator it;
		for (it = model.begin(); it != model.end(); ++it)
		{
			/// @todo Make this work with other material types...
			SharedPtr<Texture> tex = AssetManager<Texture>::Instance().Get(model.GetMaterial(it->second)->DiffuseTexture);
			Mesh& mesh = *(it->first);			

			tex->Bind();
			mesh.Bind();

			// draw
			Application::Instance().Graphics().DrawIndexed(mesh.GetIndexCount());

			tex->Unbind();
			mesh.Unbind();
		}

		shader.Unbind();
	}

		// Temporary
	void Renderer::SubmitTerrain(const std::vector<std::string> textures, const std::vector<Math::vec2> yRange, const HeightMap& map, const Shader& shader, const Math::mat4& transform)
	{
		Size_t tsize = textures.size();

		shader.Bind();
		shader.SetUniformMat4("u_transform", transform);
		shader.SetUniformMat4("u_projectionView", m_projectionView);

			//probably merge later
		for (Size_t y = 0; y < tsize; y++)
		{
			std::string textureUniform = "u_textures[" + std::to_string(y) + "]";
			std::string rangeUniform = "u_yRanges[" + std::to_string(y) + "]";
			shader.SetUniformInteger(textureUniform, y);
			shader.SetUniformFloat2(rangeUniform, yRange[y]);
		}

		shader.SetUniformInteger("u_numTextures", tsize);

		for (Size_t i = 0; i < tsize; i++)
		{
			SharedPtr<Texture> tex = AssetManager<Texture>::Instance().Get(textures[i]);
			tex->Bind(i);
		}

		Mesh& mesh = *map.GetMesh();

		mesh.Bind();

		unsigned int size = mesh.GetIndexCount();
		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

		for (Size_t i = 0; i < tsize; i++)
		{
			SharedPtr<Texture> tex = AssetManager<Texture>::Instance().Get(textures[i]);
			tex->Unbind();
		}

		mesh.Unbind();
		shader.Unbind();
	}
}
