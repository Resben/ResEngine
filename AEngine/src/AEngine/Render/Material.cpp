#include "Material.h"

namespace AEngine
{

	Material::Material() {}

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
		shader.SetUniformFloat3("colour", m_color);

		unsigned int i = 0;
		for(const auto& pair : m_textures)
		{
			pair.second->Bind();
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
