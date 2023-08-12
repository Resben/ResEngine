#include "AssimpMaterial.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Resource/AssetManager.h"

namespace AEngine
{
	AssimpMaterial::~AssimpMaterial() {}

    AssimpMaterial::AssimpMaterial(const aiMaterial* material, const std::string& directory)
    : Material("invalid", "invalid"), m_directory(directory)
    {
		GenerateMaterial(material);
    }

	AssimpMaterial::AssimpMaterial(const std::string& ident, const std::string& parent, const SharedPtr<Material> material)
	: Material(ident, parent, material) {}

    static constexpr aiTextureType ai_TextureList[] = {
		aiTextureType_NONE, aiTextureType_DIFFUSE, aiTextureType_SPECULAR, aiTextureType_AMBIENT,
		aiTextureType_EMISSIVE, aiTextureType_HEIGHT, aiTextureType_NORMALS, aiTextureType_SHININESS,
		aiTextureType_OPACITY, aiTextureType_DISPLACEMENT, aiTextureType_LIGHTMAP, aiTextureType_REFLECTION,
		aiTextureType_BASE_COLOR, aiTextureType_NORMAL_CAMERA, aiTextureType_EMISSION_COLOR, 
		aiTextureType_METALNESS, aiTextureType_DIFFUSE_ROUGHNESS, aiTextureType_AMBIENT_OCCLUSION
	};



	AEngine::AE_TEXTURETYPE GetAETextureType(aiTextureType type)
	{
		switch(type)
		{
			case aiTextureType_NONE:							return AEngine::AE_TEXTURETYPE::NONE;
			case aiTextureType_DIFFUSE:							return AEngine::AE_TEXTURETYPE::DIFFUSE;
			case aiTextureType_SPECULAR:						return AEngine::AE_TEXTURETYPE::SPECULAR;
			case aiTextureType_AMBIENT:							return AEngine::AE_TEXTURETYPE::AMBIENT;
			case aiTextureType_EMISSIVE:						return AEngine::AE_TEXTURETYPE::EMISSIVE; 
			case aiTextureType_HEIGHT:							return AEngine::AE_TEXTURETYPE::HEIGHT;
			case aiTextureType_NORMALS:							return AEngine::AE_TEXTURETYPE::NORMALS;
			case aiTextureType_SHININESS:						return AEngine::AE_TEXTURETYPE::SHININESS;
			case aiTextureType_OPACITY:							return AEngine::AE_TEXTURETYPE::OPACITY;
			case aiTextureType_DISPLACEMENT:					return AEngine::AE_TEXTURETYPE::DISPLACEMENT;
			case aiTextureType_LIGHTMAP:						return AEngine::AE_TEXTURETYPE::LIGHTMAP;
			case aiTextureType_REFLECTION:						return AEngine::AE_TEXTURETYPE::REFLECTION;
			case aiTextureType_BASE_COLOR:						return AEngine::AE_TEXTURETYPE::BASE_COLOR;
			case aiTextureType_NORMAL_CAMERA:					return AEngine::AE_TEXTURETYPE::NORMAL_CAMERA;
			case aiTextureType_EMISSION_COLOR:					return AEngine::AE_TEXTURETYPE::EMISSION_COLOR;
			case aiTextureType_METALNESS:						return AEngine::AE_TEXTURETYPE::METALNESS;
			case aiTextureType_DIFFUSE_ROUGHNESS:				return AEngine::AE_TEXTURETYPE::DIFFUSE_ROUGHNESS;
			case aiTextureType_AMBIENT_OCCLUSION:				return AEngine::AE_TEXTURETYPE::AMBIENT_OCCLUSION;
			default:											return AEngine::AE_TEXTURETYPE::UNKNOWN;
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

			AE_TEXTURETYPE ae_type = GetAETextureType(ai_type);

			//AE_LOG_ERROR("Type: {} -> {}", ae_type, filename);

			std::string path = m_directory + "/" + filename;
			this->addTexture(ae_type, AssetManager<Texture>::Instance().Load(path));
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

		this->setMaterialProperties(props);
	}
}