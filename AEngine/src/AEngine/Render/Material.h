#include "AEngine/Core/Types.h"
#include "Shader.h"
#include "Texture.h"

#include <map>

namespace AEngine
{
	enum AE_TEXTURETYPE
	{
		AE_TEXTURETYPE_NONE = 0,
		AE_TEXTURETYPE_DIFFUSE = 1,
		AE_TEXTURETYPE_SPECULAR = 2,
		AE_TEXTURETYPE_AMBIENT = 3,
		AE_TEXTURETYPE_EMISSIVE = 4,
		AE_TEXTURETYPE_HEIGHT = 5,
		AE_TEXTURETYPE_NORMALS = 6,
		AE_TEXTURETYPE_SHININESS = 7,
		AE_TEXTURETYPE_OPACITY = 8,
		AE_TEXTURETYPE_DISPLACEMENT = 9,
		AE_TEXTURETYPE_LIGHTMAP = 10,
		AE_TEXTURETYPE_REFLECTION = 11,
		AE_TEXTURETYPE_BASE_COLOR = 12,
		AE_TEXTURETYPE_NORMAL_CAMERA = 13,
		AE_TEXTURETYPE_EMISSION_COLOR = 14,
		AE_TEXTURETYPE_METALNESS = 15,
		AE_TEXTURETYPE_DIFFUSE_ROUGHNESS = 16,
		AE_TEXTURETYPE_AMBIENT_OCCLUSION = 17,
		AE_TEXTURETYPE_UNKNOWN = -1
	};

	class Material  
	{
	public:
		Material();
		void addTexture(AE_TEXTURETYPE type, SharedPtr<Texture> texture);
		void setShader(SharedPtr<Shader> shader);
			/// @todo Material should hold shader
		void Bind(const Shader& shader) const;
		void Unbind(const Shader& shader) const;

	private:
		std::map<AE_TEXTURETYPE, SharedPtr<Texture>> m_textures;
		SharedPtr<Shader> m_shader;
		Math::vec3 m_color;
	};
}