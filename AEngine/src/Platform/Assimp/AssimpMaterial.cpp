#include "AssimpMaterial.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Resource/AssetManager.h"

namespace
{
	static constexpr aiTextureType ai_TextureList[] = {
		aiTextureType_NONE, aiTextureType_DIFFUSE, aiTextureType_SPECULAR, aiTextureType_AMBIENT,
		aiTextureType_EMISSIVE, aiTextureType_HEIGHT, aiTextureType_NORMALS, aiTextureType_SHININESS,
		aiTextureType_OPACITY, aiTextureType_DISPLACEMENT, aiTextureType_LIGHTMAP, aiTextureType_REFLECTION,
		aiTextureType_BASE_COLOR, aiTextureType_NORMAL_CAMERA, aiTextureType_EMISSION_COLOR, 
		aiTextureType_METALNESS, aiTextureType_DIFFUSE_ROUGHNESS, aiTextureType_AMBIENT_OCCLUSION
	};
}

namespace AEngine
{
    AssimpMaterial::AssimpMaterial(const std::string& ident, const std::string& path, const aiMaterial* material, const std::string& directory)
    : Material(ident, path), m_directory(directory)
    {
		GenerateMaterial(material);
    }

	TextureType GetAETextureType(aiTextureType type)
	{
		switch(type)
		{
			case aiTextureType_NONE:                return TextureType::None;
			case aiTextureType_DIFFUSE:             return TextureType::Diffuse;
			case aiTextureType_SPECULAR:            return TextureType::Specular;
			case aiTextureType_AMBIENT:             return TextureType::Ambient;
			case aiTextureType_EMISSIVE:            return TextureType::Emissive; 
			case aiTextureType_HEIGHT:              return TextureType::Height;
			case aiTextureType_NORMALS:             return TextureType::Normals;
			case aiTextureType_SHININESS:           return TextureType::Shininess;
			case aiTextureType_OPACITY:             return TextureType::Opacity;
			case aiTextureType_DISPLACEMENT:        return TextureType::Displacement;
			case aiTextureType_LIGHTMAP:            return TextureType::Lightmap;
			case aiTextureType_REFLECTION:          return TextureType::Reflection;
			case aiTextureType_BASE_COLOR:          return TextureType::BaseColor;
			case aiTextureType_NORMAL_CAMERA:       return TextureType::NormalCamera;
			case aiTextureType_EMISSION_COLOR:      return TextureType::EmissionColor;
			case aiTextureType_METALNESS:           return TextureType::Metalness;
			case aiTextureType_DIFFUSE_ROUGHNESS:   return TextureType::DiffuseRoughness;
			case aiTextureType_AMBIENT_OCCLUSION:   return TextureType::AmbientOcclusion;
			default:                                return TextureType::Unknown;
		}
	}

	void AssimpMaterial::LoadTextures(const aiMaterial* ai_material, const aiTextureType ai_type)
	{
		aiString str;
		if(ai_material->GetTexture(ai_type, 0, &str) == AI_SUCCESS)
		{
			std::string filename = str.C_Str();
			Size_t last = filename.find_last_of("\\");

			if (last != std::string::npos)
				filename = filename.substr(last + 1);

			TextureType ae_type = GetAETextureType(ai_type);

			//AE_LOG_ERROR("Type: {} -> {}", ae_type, filename);

			std::string path = m_directory + "/" + filename;
			this->AddTexture(ae_type, AssetManager<Texture>::Instance().Load(path));
		}
	}

	void AssimpMaterial::GenerateMaterial(const aiMaterial* ai_mat)
	{
		for(auto ai_type : ai_TextureList)
		{
			if(ai_mat->GetTextureCount(ai_type) > 0)
			{
				LoadTextures(ai_mat, ai_type);
			}
		}

		MaterialProperties props;
		aiColor4D color4;
		ai_mat->Get(AI_MATKEY_COLOR_DIFFUSE, color4);
		props.baseColor = Math::vec4(color4.r, color4.g, color4.b, color4.a); // Remove alpha replace with transparencyFactor

		aiColor3D color3;
		aiColor3D reset;
		ai_mat->Get(AI_MATKEY_COLOR_EMISSIVE, color3);
		props.emissionColor = Math::vec3(color3.r, color3.g, color3.b);
		color3 = reset;
		ai_mat->Get(AI_MATKEY_COLOR_AMBIENT, color3);
		props.ambientColor = Math::vec3(color3.r, color3.g, color3.b);
		color3 = reset;
		ai_mat->Get(AI_MATKEY_COLOR_TRANSPARENT, color3);
		props.transparencyColor = Math::vec3(color3.r, color3.g, color3.b);
		color3 = reset;

		ai_real floatval = 0;
		ai_real reset2 = 0;
		ai_mat->Get(AI_MATKEY_SHININESS_STRENGTH, floatval);
		props.shininessStrength = floatval;
		floatval = reset2;
		ai_mat->Get(AI_MATKEY_OPACITY, floatval); // Seems to just be alpha
		props.transparencyFactor = floatval;
		floatval = reset2;
		ai_mat->Get(AI_MATKEY_REFRACTI, floatval);
		props.ior = floatval;
		floatval = reset2;
		ai_mat->Get(AI_MATKEY_SHININESS, floatval);
		props.shininess = floatval;
		floatval = reset2;

		AE_LOG_DEBUG("base: {} {} {} {}", props.baseColor.a, props.baseColor.x, props.baseColor.y, props.baseColor.z);
		AE_LOG_DEBUG("emission: {} {} {}", props.emissionColor.x, props.emissionColor.y, props.emissionColor.z);
		AE_LOG_DEBUG("ambient: {} {} {}", props.ambientColor.x, props.ambientColor.y, props.ambientColor.z);
		AE_LOG_DEBUG("transcolor: {} {} {}", props.transparencyColor.x, props.transparencyColor.y, props.transparencyColor.z);
		AE_LOG_DEBUG("sstrength: {}", props.shininessStrength);
		AE_LOG_DEBUG("transfactor: {}", props.transparencyFactor);
		AE_LOG_DEBUG("ior: {}", props.ior);
		AE_LOG_DEBUG("s: {}", props.shininess);

		if (m_directory == "assets/test/TestItems")
		{
			AE_LOG_DEBUG("NAME: {} ", ai_mat->GetName().C_Str());

			for (int i = 0; i < ai_mat->mNumProperties; i++)
			{
				AE_LOG_DEBUG("{}", ai_mat->mProperties[i]->mKey.C_Str());
			}
		}

		this->SetMaterialProperties(props);
	}
}