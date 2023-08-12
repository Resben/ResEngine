#include "AssimpModel.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Math/Math.h"
#include "AEngine/Resource/AssetManager.h"
#include "AEngine/Render/RenderCommand.h"
#include "AEngine/Render/Texture.h"
#include "AssimpAnimation.h"

namespace AEngine
{

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

	AssimpModel::AssimpModel(const std::string& ident, const std::string& path)
		: Model(ident, path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			return;
		}
		AE_LOG_DEBUG("AssimpModel::Constructor::Loading -> {}", path);

		m_directory = path.substr(0, path.find_last_of('/'));

		ProcessNode(scene->mRootNode, scene);

		if(scene->HasAnimations())
		{
			for(int i = 0; i < scene->mNumAnimations; i++)
			{
				SharedPtr<AssimpAnimation> animation = MakeShared<AssimpAnimation>(scene, m_BoneInfoMap, i);
				AssetManager<Animation>::Instance().LoadSubAsset(path, scene->mAnimations[i]->mName.C_Str(), animation);
			}
		}

		AE_LOG_TRACE("Model::Constructor::Success -> {}", path);
	}

	AssimpModel::~AssimpModel()
	{
		Clear();
	}

	void AssimpModel::ProcessNode(const aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			m_meshes.push_back(
				CreateMesh(scene, scene->mMeshes[node->mMeshes[i]])
			);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	/*
	 * NOTE:
	 * Is it necessary to copy data
	 * Possibly needed for physics
	*/
	AssimpModel::mesh_material AssimpModel::CreateMesh(const aiScene* scene, const aiMesh* mesh)
	{
		// generate structures for mesh data
		std::vector<float> positionAndTextureData;
		std::vector<float> normalData;
		std::vector<unsigned int> indices;

		// get index data
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (unsigned int y = 0; y < face.mNumIndices; y++)
			{
				indices.push_back(face.mIndices[y]);
			}
		}

		// get position data and texture data
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			positionAndTextureData.push_back(mesh->mVertices[i].x);
			positionAndTextureData.push_back(mesh->mVertices[i].y);
			positionAndTextureData.push_back(mesh->mVertices[i].z);

			if (mesh->HasTextureCoords(0))
			{
				positionAndTextureData.push_back(mesh->mTextureCoords[0][i].x);
				positionAndTextureData.push_back(mesh->mTextureCoords[0][i].y);
			}
			else
			{
				positionAndTextureData.push_back(0.0f);
				positionAndTextureData.push_back(0.0f);
			}
		}

		// get normal data
		if (mesh->HasNormals())
		{
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				glm::vec3 normal = glm::normalize(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
				normalData.push_back(normal.x);
				normalData.push_back(normal.y);
				normalData.push_back(normal.z);
			}
		}

		// setup vertex array
		SharedPtr<VertexArray> vertexArray = VertexArray::Create();

		// setup index buffer
		SharedPtr<IndexBuffer> indexBuffer = IndexBuffer::Create();
		indexBuffer->SetData(indices.data(), static_cast<Uint32>(indices.size()), BufferUsage::StaticDraw);
		vertexArray->SetIndexBuffer(indexBuffer);

		// setup position and texture-coordinate vertex buffer
		SharedPtr<VertexBuffer> positionAndTextureBuffer = VertexBuffer::Create();
		positionAndTextureBuffer->SetData(positionAndTextureData.data(), static_cast<Intptr_t>(positionAndTextureData.size() * sizeof(float)), BufferUsage::StaticDraw);
		positionAndTextureBuffer->SetLayout({ { BufferElementType::Float3, false }, { BufferElementType::Float2, false } });
		vertexArray->AddVertexBuffer(positionAndTextureBuffer);

		// setup normal vertex buffer
		if (mesh->HasNormals())
		{
			SharedPtr<VertexBuffer> normalBuffer = VertexBuffer::Create();
			normalBuffer->SetData(normalData.data(), static_cast<Intptr_t>(normalData.size() * sizeof(float)), BufferUsage::StaticDraw);
			normalBuffer->SetLayout({ { BufferElementType::Float3, false } });
			vertexArray->AddVertexBuffer(normalBuffer);
		}

		if(mesh->HasBones())
		{
			std::vector<int> boneIDs;
			std::vector<float> boneWeights;
			boneIDs.resize(positionAndTextureData.size() * MAX_BONE_INFLUENCE);
			boneWeights.resize(positionAndTextureData.size() * MAX_BONE_INFLUENCE);

			for (unsigned int t = 0; t < boneIDs.size(); t++)
			{
				boneIDs[t] = -1;
				boneWeights[t] = 0.0f;
			}

			LoadMeshBones(mesh, boneWeights, boneIDs);

			// setup buffers for bone data
			SharedPtr<VertexBuffer> boneIdBuffer = VertexBuffer::Create();
			boneIdBuffer->SetData(boneIDs.data(), boneIDs.size() * sizeof(int), BufferUsage::StaticDraw);
			boneIdBuffer->SetLayout({ { BufferElementType::Int4, false } });
			vertexArray->AddVertexBuffer(boneIdBuffer);

			SharedPtr<VertexBuffer> boneWeightBuffer = VertexBuffer::Create();
			boneWeightBuffer->SetData(boneWeights.data(), boneWeights.size() * sizeof(float), BufferUsage::StaticDraw);
			boneWeightBuffer->SetLayout({ { BufferElementType::Float4, false } });
			vertexArray->AddVertexBuffer(boneWeightBuffer);
		}

		return std::make_pair(vertexArray, GenerateMaterials(scene, mesh->mMaterialIndex));
	}

	int AssimpModel::NameToID(std::string& name, const aiBone* bone)
	{
		auto it = m_BoneInfoMap.find(name);
		if (it != m_BoneInfoMap.end())
		{
			return it->second.id;
		}

		BoneInfo info;
		info.id = static_cast<int>(m_BoneInfoMap.size());
		info.offset = Math::transpose(Math::make_mat4(&bone->mOffsetMatrix.a1));
		m_BoneInfoMap.emplace(name, info);
		return info.id;
	}

	void AssimpModel::LoadMeshBones(const aiMesh* mesh, std::vector<float>& BoneWeights, std::vector<int>& BoneIDs)
	{
		for (unsigned int i = 0; i < mesh->mNumBones; i++)
		{
			for (unsigned int y = 0; y < mesh->mBones[i]->mNumWeights; y++)
			{
				const int index = mesh->mBones[i]->mWeights[y].mVertexId * MAX_BONE_INFLUENCE;
				// Loop through max number of bones per vertex
				int replacedIndex = 0;
				float lowestWeight = std::numeric_limits<float>::max();
				for (unsigned int z = 0; z < MAX_BONE_INFLUENCE; z++)
				{
					if (BoneWeights[index + z] < lowestWeight)
					{
						lowestWeight = BoneWeights[index + z];
						replacedIndex = z;
					}
				}

				BoneIDs[index + replacedIndex] = NameToID(std::string(mesh->mBones[i]->mName.C_Str()), mesh->mBones[i]);
				BoneWeights[index + replacedIndex] = static_cast<float>(mesh->mBones[i]->mWeights[y].mWeight);
			}
		}
	}

	void AssimpModel::Clear()
	{
		m_meshes.clear();
		AE_LOG_DEBUG("Model::Clear");
	}

	void AssimpModel::Render(const Math::mat4& transform, const Shader &shader, const Math::mat4 &projectionView) const
	{
		shader.Bind();
		shader.SetUniformInteger("u_texture1", 0);
		shader.SetUniformMat4("u_transform", transform);
		shader.SetUniformMat4("u_projectionView", projectionView);

		for (auto it = m_meshes.begin(); it != m_meshes.end(); ++it)
		{
			const Material* mat = (it->second).get();
			const VertexArray* va = (it->first).get();

			mat->Bind(shader);
			va->Bind();

			// draw
			RenderCommand::DrawIndexed(Primitive::Triangles, va->GetIndexBuffer()->GetCount(), 0);

			mat->Unbind(shader);
			va->Unbind();
		}

		shader.Unbind();
	}

	void AssimpModel::Render(const Math::mat4& transform, const Shader& shader, const Math::mat4 & projectionView, Animator& animation, const TimeStep dt)
	{
		shader.Bind();
		//shader.SetUniformInteger("u_texture1", 0);
		shader.SetUniformMat4("u_transform", transform);
		shader.SetUniformMat4("u_projectionView", projectionView);

		animation.UpdateAnimation(dt);

		auto transforms = animation.GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i)
			shader.SetUniformMat4("u_finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

		for (auto it = m_meshes.begin(); it != m_meshes.end(); ++it)
		{
			const Material* mat = (it->second).get();
			const VertexArray* va = it->first.get();

			mat->Bind(shader);
			va->Bind();

			// draw
			RenderCommand::DrawIndexed(Primitive::Triangles, va->GetIndexBuffer()->GetCount(), 0);

			mat->Unbind(shader);
			va->Unbind();
		}

		shader.Unbind();
	}

	void AssimpModel::LoadTextures(SharedPtr<Material> ae_material, const aiMaterial* ai_material, const aiTextureType ai_type)
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
			ae_material->addTexture(ae_type, AssetManager<Texture>::Instance().Load(path));
		}
	}

	SharedPtr<Material> AssimpModel::GenerateMaterials(const aiScene* scene, int index)
	{
		if (index >= 0)
		{
			aiMaterial* ai_mat = scene->mMaterials[index];
			SharedPtr<Material> ae_mat = MakeShared<Material>();

			for(auto ai_type : ai_TextureList)
			{
				if(ai_mat->GetTextureCount(ai_type) > 0)
				{
					LoadTextures(ae_mat, ai_mat, ai_type);
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

			AE_LOG_DEBUG("colour: {} {} {} {}", props.baseColor.a, props.baseColor.x, props.baseColor.y, props.baseColor.z);

			if(m_directory == "assets/test/TestItems")
			{
				AE_LOG_DEBUG("NAME: {} ", ai_mat->GetName().C_Str());

				for(int i = 0; i < ai_mat->mNumProperties; i++)
				{
					AE_LOG_DEBUG("{}", ai_mat->mProperties[i]->mKey.C_Str());
				}
			}

			ae_mat->setMaterialProperties(props);

			return ae_mat;
		}
	}

	const VertexArray* AssimpModel::GetMesh(int index) const
	{
		if (index > m_meshes.size())
		{
			AE_LOG_FATAL("AssimpModel::GetMesh::Out of Bounds");
		}

		return (m_meshes[index].first).get();
	}

	const Material* AssimpModel::GetMaterial(int index) const
	{
		if (index > m_meshes.size())
		{
			AE_LOG_FATAL("AssimpModel::GetMesh::Out of Bounds");
		}

		return (m_meshes[index].second).get();
	}

	Animation* AssimpModel::GetAnimation(std::string id)
	{
			// Caused when you try serialize a model that does not exist
		if(this == nullptr)
		{
			AE_LOG_FATAL("AssimpModel::GetAnimation::Model is Null -> This may be because you have a null model in your component");
		}

		std::map<std::string, SharedPtr<Animation>>::const_iterator it;
		it = m_animations.find(id);
		if (it != m_animations.end())
			return it->second.get();
		else
			return nullptr;
	}
}