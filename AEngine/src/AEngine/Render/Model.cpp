#include "Model.h"
#include "../Resource/TextureManager.h"
#include "../Core/Logger.h"
#include <iostream>

namespace AEngine
{
	Math::vec3 operator>>(const aiVector3D& aiVec, Math::vec3& glmvec)
	{
		glmvec.x = aiVec.x;
		glmvec.y = aiVec.y;
		glmvec.z = aiVec.z;

		return glmvec;
	}

	Math::vec2 operator>>(const aiVector3D& aiVec, Math::vec2& glmvec)
	{
		glmvec.x = aiVec.x;
		glmvec.y = aiVec.y;

		return glmvec;
	}

	Math::vec3 operator>>(const aiColor4D& aiVec, Math::vec3& glmvec)
	{
		glmvec.x = aiVec.r;
		glmvec.y = aiVec.g;
		glmvec.z = aiVec.b;

		return glmvec;
	}


	Model::Model(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			return;
		}

		m_directory = path.substr(0, path.find_last_of('/'));

		ProcessNode(scene->mRootNode, scene);

		GenerateMaterials(scene);

		AE_LOG_DEBUG("Model::Loaded::{}", path);
	}

	Model::~Model()
	{
		Clear();
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			m_meshes.push_back(
				CreateMesh(scene->mMeshes[node->mMeshes[i]])
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
	std::shared_ptr<Mesh> Model::CreateMesh(aiMesh* mesh)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex{};

			mesh->mVertices[i] >> vertex.Position;

			if (mesh->HasNormals())
			{
				//mesh->mNormals[i] >> vertex.normal;
				//vertex.normal = vector;
			}

			if (mesh->HasTextureCoords(0))
			{
				mesh->mTextureCoords[0][i] >> vertex.TexCoords;
				// Look into Tangents and Bitangents
			}
			else
			{
				vertex.TexCoords = Math::vec2(0.0f, 0.0f);
			}

				// Colour
			//if (mesh->HasVertexColors(0))
			//{
			//	mesh->mColors[0][i] >> vertex.Colour;
			//}
			//else
			//{
			//	vertex.Colour = glm::vec3(1.0f, 1.0f, 1.0f);
			//}

			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (unsigned int y = 0; y < face.mNumIndices; y++)
			{
				indices.push_back(face.mIndices[y]);
			}
		}

		m_indexes.push_back(mesh->mMaterialIndex);

		return std::make_shared<Mesh>(vertices, indices);
	}

	void Model::Clear()
	{
		m_materials.clear();
		m_meshes.clear();
		AE_LOG_DEBUG("Model::Clear");
	}

	std::string& Model::LoadTextures(aiMaterial* mat, aiTextureType type)
	{
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			std::string filename = str.C_Str();
			std::size_t last = filename.find_last_of("\\");

			if (last != std::string::npos)
				filename = filename.substr(last + 1);

			std::string path = m_directory + "/" + filename;

				// Should this be checked here of texturemanager?
			if(TextureManager::Instance()->GetTexture(filename) == nullptr)
				TextureManager::Instance()->LoadTexture(path);

			return filename;
		}
	}

	void Model::GenerateMaterials(const aiScene* scene)
	{
		for (unsigned int i = 0; i < m_indexes.size(); i++)
		{
			if (m_indexes[i] >= 0)
			{
				Material material;
				aiMaterial* mat = scene->mMaterials[m_indexes[i]];
				material.DiffuseTexture = LoadTextures(mat, aiTextureType_DIFFUSE);
				material.SpecularTexture = LoadTextures(mat, aiTextureType_SPECULAR);
				m_materials.emplace(std::make_pair(i, material));
			}
		}
	}

	std::shared_ptr<Mesh>& Model::GetMesh(int index)
	{
		if (index > m_meshes.size())
		{
			AE_LOG_CRITICAL("Model::GetMesh::Out of Bounds");
			exit(1);
		}

		return m_meshes[index];
	}

	Material* Model::GetMaterial(int meshIndex)
	{
		std::map<unsigned int, Material>::iterator it;
		it = m_materials.find(meshIndex);
		if (it != m_materials.end())
			return &it->second;
		else
			return nullptr;
	}

	int Model::Size()
	{
		return m_meshes.size();
	}
}