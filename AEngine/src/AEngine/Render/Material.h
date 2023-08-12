#include "AEngine/Core/Types.h"
#include "Shader.h"
#include "Texture.h"

#include <map>

namespace AEngine
{
	enum AE_TEXTURETYPE
	{
		NONE = 0,					// Dummy value 'texture semantic'

		// "LEGACY" //
		DIFFUSE = 1,				// Diffuse lighting equation (PBR Specular/Glossiness)
		SPECULAR = 2,				// Specular lighting equation (PBR Specular/Glossiness)
		AMBIENT = 3,				// Ambient lighting equation
		EMISSIVE = 4,				// Lighting equation + not influenced by light
		HEIGHT = 5,					// Texture is a height map
		NORMALS = 6,				// Tangent space normal map
		SHININESS = 7,				// Defines glossiness of the material
		OPACITY = 8,				// Defines per pixel opacity
		DISPLACEMENT = 9,			// Displacement texture
		LIGHTMAP = 10,				// Lightmap texture (ambient occulusion)
		REFLECTION = 11,			// Reflection texture

		// PBR //
		BASE_COLOR = 12,			// Diffuse
		NORMAL_CAMERA = 13,			// Normals
		EMISSION_COLOR = 14, 		// Emissions
		METALNESS = 15,				// Metal
		DIFFUSE_ROUGHNESS = 16,		// Roughness
		AMBIENT_OCCLUSION = 17, 	// Ambient occlusion

		UNKNOWN = -1				// Texture unknown
	};

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

	class Material  
	{
	public:
		Material();
		void addTexture(AE_TEXTURETYPE type, SharedPtr<Texture> texture);
		void setShader(SharedPtr<Shader> shader);
		void setMaterialProperties(MaterialProperties &properties);
			/// @todo Material should hold shader
		void Bind(const Shader& shader) const;
		void Unbind(const Shader& shader) const;

	private:
		std::map<AE_TEXTURETYPE, SharedPtr<Texture>> m_textures;
		SharedPtr<Shader> m_shader;
		MaterialProperties m_properties;
		bool m_isPBR;
	};
}