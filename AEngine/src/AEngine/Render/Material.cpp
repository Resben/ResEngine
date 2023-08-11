#include "Material.h"

namespace AEngine
{

	Material::Material() {}

	void Material::setMaterialProperties(MaterialProperties properties)
	{
		m_properties = properties;
	}

	void Material::addTexture(AE_TEXTURETYPE type, SharedPtr<Texture> texture)
	{
		m_textures[type] = texture;
	}

	void Material::setShader(SharedPtr<Shader> shader)
	{
		m_shader = shader;
	}

	void Material::Bind(const Shader& shader) const
	{
		shader.SetUniformFloat4("u_baseColor", m_properties.baseColor);
		shader.SetUniformFloat3("u_specularColor", m_properties.specularColor);
		shader.SetUniformFloat3("u_ambientColor", m_properties.ambientColor);
		shader.SetUniformFloat3("u_emissionColor", m_properties.emissionColor);
		shader.SetUniformFloat("u_shininess", m_properties.shininess);
		shader.SetUniformFloat("u_reflectivity", m_properties.reflectivity);
		shader.SetUniformFloat("u_ior", m_properties.ior);

		unsigned int i = 0;
		for(const auto& pair : m_textures)
		{
			pair.second->Bind(i);
			shader.SetUniformInteger("u_texture" + pair.first, i);
		}
	}

	void Material::Unbind(const Shader& shader) const
	{
		for(const auto& pair : m_textures)
		{
			pair.second->Unbind();
		}
	}
}
