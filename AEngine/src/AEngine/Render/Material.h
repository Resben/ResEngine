/**
 * \file
 * \author Ben Hawkins (34112619)
**/

#pragma once
#include "AEngine/Core/Types.h"
#include "Shader.h"
#include "Texture.h"
#include "AEngine/Resource/Asset.h"

#include <map>

namespace AEngine
{
		/**
		 * \enum TextureType
		 * \brief Define the type of texture
		**/
	enum TextureType
	{
		None = 0,                ///< Dummy value 'texture semantic'

		// "LEGACY" //
		Diffuse = 1,             ///< Diffuse lighting equation (PBR Specular/Glossiness)
		Specular = 2,            ///< Specular lighting equation (PBR Specular/Glossiness)
		Ambient = 3,             ///< Ambient lighting equation
		Emissive = 4,            ///< Lighting equation + not influenced by light
		Height = 5,              ///< Texture is a height map
		Normals = 6,             ///< Tangent space normal map
		Shininess = 7,           ///< Defines glossiness of the material
		Opacity = 8,             ///< Defines per pixel opacity
		Displacement = 9,        ///< Displacement texture
		Lightmap = 10,           ///< Lightmap texture (ambient occulusion)
		Reflection = 11,         ///< Reflection texture

		// PBR //
		BaseColor = 12,          ///< Diffuse
		NormalCamera = 13,       ///< Normals
		EmissionColor = 14,      ///< Emissions
		Metalness = 15,          ///< Metal
		DiffuseRoughness = 16,   ///< Roughness
		AmbientOcclusion = 17,   ///< Ambient occlusion

		Unknown = -1             ///< Texture unknown
	};

		/**
		 * \struct MaterialProperties
		 * \brief Stores non-texture related properties
		**/
	struct MaterialProperties
	{
		Math::vec4 baseColor;
		Math::vec3 specularColor;
		Math::vec3 emissionColor;
		Math::vec3 ambientColor;
		Math::vec3 transparencyColor;
		float shininess;
		float shininessStrength;
		float reflectivity;
		float transparencyFactor;
		float ior;

		MaterialProperties()
		: baseColor(1.0f, 1.0f, 1.0f, 1.0f),
		  specularColor(0.5f),
		  transparencyColor(0.5f),
		  emissionColor(0.0f),
		  ambientColor(0.0f),
		  shininess(32.0f),
		  shininessStrength(1.0f),
		  reflectivity(0.1f),
		  transparencyFactor(1.0f),
		  ior(1.0f) {}
	};

		/**
		 * \class Material
		 * \brief A asset to store material data
		**/
	class Material : public Asset
	{
	public:
		virtual ~Material() = default;
			/**
			 * \brief Add a texture of a type to material
			 * \param[in] type enum type of material
			 * \param[in] texture shared ptr to texture
			 * \note only allows 1 type of each texture
			 * \note types will be replaced by duplicates
			**/
		void AddTexture(TextureType type, SharedPtr<Texture> texture);
			/**
			 * \brief Set the materials shader
			 * \param[in] shader shared ptr to shader
			**/
		void SetShader(SharedPtr<Shader> shader);
			/**
			 * \brief Set the materials properties
			 * \param[in] properties struct of properties
			**/
		void SetMaterialProperties(MaterialProperties &properties);
			/**
			 * \brief Bind a materials textures and properties
			 * \param[in] shader shared ptr to shader
			 * \todo Remove shader --> Material should hold shader
			**/
		void Bind(const Shader& shader) const;
			/**
			 * \brief Unbind a materials textures
			 * \param[in] shader shared ptr to shader
			 * \todo Remove shader --> Material should hold shader
			**/
		void Unbind(const Shader& shader) const;

		void SetColor(const Math::vec4& color);

		bool IsTransparent() const;

	protected:
			/**
			 * \brief Protected constructor for model loading
			 * \param[in] ident Unique asset identifier
			 * \param[in] path Path of asset
			 * \note path relates to parent asset
			 * \todo define a constructor for user creation
			**/
		Material(const std::string& ident, const std::string& path);

		std::map<TextureType, SharedPtr<Texture>> m_textures;
		SharedPtr<Shader> m_shader;
		MaterialProperties m_properties;
		bool m_isPBR = false;
	};
}