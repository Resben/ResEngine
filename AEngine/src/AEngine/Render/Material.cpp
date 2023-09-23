#include "Material.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Core/Application.h"
#include "AEngine/Render/RenderCommand.h"
#include "Platform/Assimp/AssimpMaterial.h"
#include "AEngine/Render/Model.h"
#include "AEngine/Scene/SceneManager.h"
#include "AEngine/Scene/Entity.h"
#include "AEngine/Skybox/Skybox.h"
#include "AEngine/Render/RenderCommand.h"

namespace AEngine
{

	Material::Material(const std::string& ident, const std::string& path)
	: Asset(ident, path) {}

	void Material::SetMaterialProperties(MaterialProperties &properties)
	{
		m_properties = properties;
	}

	void Material::AddTexture(TextureType type, SharedPtr<Texture> texture)
	{
		m_textures[type] = texture;

		if(type > 11)
			m_isPBR = true;
	}

	void Material::SetShader(SharedPtr<Shader> shader)
	{
		m_shader = shader;
	}

	bool Material::IsTransparent() const
	{
		if(m_textures.empty() && m_properties.baseColor.a != 1)
			return true;

		return false;
	}

	void Material::Bind(const Shader& shader) const
	{
		shader.SetUniformFloat4("u_baseColor", m_properties.baseColor);
		//shader.SetUniformFloat3("u_specularColor", m_properties.specularColor);
		//shader.SetUniformFloat3("u_ambientColor", m_properties.ambientColor);
		//shader.SetUniformFloat3("u_emissionColor", m_properties.emissionColor);
		//shader.SetUniformFloat("u_shininess", m_properties.shininess);
		//shader.SetUniformFloat("u_reflectivity", m_properties.reflectivity);
		//shader.SetUniformFloat("u_ior", m_properties.ior);

		if (m_textures.size() > 0)
			shader.SetUniformInteger("u_hasTextures", 1);
		else
			shader.SetUniformInteger("u_hasTextures", 0);

		unsigned int i = 0;
		for(const auto& pair : m_textures)
		{
			if(m_isPBR && pair.first < 11)
				continue;

			pair.second->Bind(i);
			shader.SetUniformInteger("u_texture" + pair.first, i);
			i++;
		}

		SharedPtr<CubeMapTexture> hdr = SceneManager::GetActiveScene()->GetEntity("Skybox").GetComponent<SkyboxComponent>()->skybox->GetCubeMap();
		if (hdr)
		{
			Math::vec3 camPos = SceneManager::GetActiveScene()->GetDebugCamera().GetPosition();

			shader.SetUniformFloat3("u_camPos", camPos);
			shader.SetUniformInteger("u_hdr", i);
			hdr->Bind(i);
		}
	}

	void Material::Unbind(const Shader& shader) const
	{
		unsigned int i = 0;
		for (const auto& pair : m_textures)
		{
			if (m_isPBR && pair.first < 11)
				continue;

			pair.second->Unbind(i);
			i++;
		}
	}
}
