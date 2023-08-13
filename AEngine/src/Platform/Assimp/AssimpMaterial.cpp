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
		ai_mat->Get(AI_MATKEY_COLOR_EMISSIVE, props.emissionColor);
		ai_mat->Get(AI_MATKEY_COLOR_AMBIENT, props.ambientColor);
		ai_mat->Get(AI_MATKEY_SHININESS_STRENGTH, props.shininessStrength);
		ai_mat->Get(AI_MATKEY_COLOR_TRANSPARENT, props.transparencyColor);
		ai_mat->Get(AI_MATKEY_OPACITY, props.transparencyFactor);
		ai_mat->Get(AI_MATKEY_REFRACTI, props.ior);
		ai_mat->Get(AI_MATKEY_SHININESS, props.shininess);
		ai_mat->Get(AI_MATKEY_COLOR_DIFFUSE, props.baseColor);

		// AE_LOG_DEBUG("colour: {} {} {} {}", props.baseColor.a, props.baseColor.x, props.baseColor.y, props.baseColor.z);

		// if(m_directory == "assets/test/TestItems")
		// {
		// 	AE_LOG_DEBUG("NAME: {} ", ai_mat->GetName().C_Str());

		// 	for(int i = 0; i < ai_mat->mNumProperties; i++)
		// 	{
		// 		AE_LOG_DEBUG("{}", ai_mat->mProperties[i]->mKey.C_Str());
		// 	}
		// }

		this->SetMaterialProperties(props);
	}
}