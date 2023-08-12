#include "Material.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Core/Application.h"
#include "AEngine/Render/RenderCommand.h"
#include "Platform/Assimp/AssimpMaterial.h"
#include "AEngine/Render/Model.h"

namespace AEngine
{

	Material::Material(const std::string& ident, const std::string& parent)
	: Asset(ident, parent) {}

	Material::Material(const std::string& ident, const std::string& parent, const SharedPtr<Material> material)
	: Asset(ident, parent)
	{
		m_textures = material->m_textures;
		m_shader = material->m_shader;
		m_properties = material->m_properties;
		m_isPBR = m_isPBR;
	}

	SharedPtr<Material> AEngine::Material::Create(const std::string& ident, const std::string& parent, const SharedPtr<Material> material)
	{
		switch (RenderCommand::GetLibrary())
		{
		case RenderLibrary::OpenGL:
			return MakeShared<AssimpMaterial>(ident, parent, material);
		default:
			AE_LOG_FATAL("Texture::Create::RenderLibrary::Error -> None selected");
		}
	}

	void Material::setMaterialProperties(MaterialProperties &properties)
	{
		m_properties = properties;
	}

	void Material::addTexture(AE_TEXTURETYPE type, SharedPtr<Texture> texture)
	{
		m_textures[type] = texture;

		if(type > 11)
			m_isPBR = true;
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
			//AE_LOG_DEBUG("colour: {} {} {} {}", m_properties.baseColor.a, m_properties.baseColor.x, m_properties.baseColor.y, m_properties.baseColor.z);

			if(m_isPBR && pair.first < 11)
				continue;

			pair.second->Bind(i);
			shader.SetUniformInteger("u_texture" + pair.first, i);
			i++;
		}
	}

	void Material::Unbind(const Shader& shader) const
	{
		for(const auto& pair : m_textures)
		{
			if(m_isPBR && pair.first < 11)
				continue;

			pair.second->Unbind();
		}
	}
}
