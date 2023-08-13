#include "AssimpModel.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Math/Math.h"
#include "AEngine/Resource/AssetManager.h"
#include "AEngine/Render/RenderCommand.h"
#include "AEngine/Render/Texture.h"
#include "AssimpAnimation.h"
#include "AssimpMaterial.h"

namespace AEngine
{

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
				Size_t last = path.find_last_of("/");
				const std::string ident = path.substr(last + 1) + "/" + scene->mAnimations[i]->mName.C_Str();

				SharedPtr<Animation> animation = MakeShared<AssimpAnimation>(ident, path, scene, m_BoneInfoMap, i);
				AssetManager<Animation>::Instance().LoadSubAsset(ident, animation);
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
			/// @todo define a null material
		std::string matid = "null";

		if(mesh->mMaterialIndex >= 0)
		{
			aiMaterial* ai_mat = scene->mMaterials[mesh->mMaterialIndex];
			matid = this->GetPath() + "/" + ai_mat->GetName().C_Str();
			SharedPtr<Material> material = MakeShared<AssimpMaterial>(matid, this->GetPath(), ai_mat, m_directory);
			SharedPtr<Material> resultMaterial = AssetManager<Material>::Instance().LoadSubAsset(matid, material);
		}

		return std::make_pair(vertexArray, matid);
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
}