#include "GLModel.h"
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


	GLModel::GLModel(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			return;
		}

		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);

		generateMaterials(scene);
	}

	GLModel::~GLModel()
	{
		Delete();
	}

	void GLModel::processNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			m_meshes.push_back(
				createMesh(scene->mMeshes[node->mMeshes[i]])
			);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	/*
	 * NOTE:
	 * Is it necessary to copy data
	 * Possibly needed for physics
	*/
	std::shared_ptr<Mesh> GLModel::createMesh(aiMesh* mesh)
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

		indexes.push_back(mesh->mMaterialIndex);

		std::cout << "Loaded a mesh::vert " << vertices.size() << "::ind " << indices.size() << std::endl;

		return std::make_shared<Mesh>(vertices, indices);
	}

	void GLModel::Delete()
	{
		m_materials.clear();
		m_meshes.clear();
	}

	void GLModel::loadTextures(aiMaterial* mat, aiTextureType type)
	{
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			std::string filename = str.C_Str();
			std::size_t last = filename.find_last_of("\\");

			if (last != std::string::npos)
				filename = filename.substr(last + 1);

			std::string path = directory + "/" + filename;

			std::cout << "Loaded a texture..." << std::endl;

			m_materials.push_back(std::make_shared<Texture>(path));
		}
	}

	void GLModel::generateMaterials(const aiScene* scene)
	{
		for (unsigned int i = 0; i < indexes.size(); i++)
		{
			if (indexes[i] >= 0)
			{
				aiMaterial* mat = scene->mMaterials[indexes[i]];
				loadTextures(mat, aiTextureType_DIFFUSE);
				loadTextures(mat, aiTextureType_SPECULAR);
			}
		}
	}

	std::shared_ptr<Mesh>& GLModel::getMesh(int index)
	{
		if (index > m_meshes.size())
		{
			std::cout << "Out of bounds" << std::endl;
			exit(1);
		}

		return m_meshes[index];
	}

	std::shared_ptr<Texture>& GLModel::getTextures(int meshIndex)
	{
		// Match a meshindex to a texture using a map?
		if (meshIndex > m_meshes.size())
		{
			std::cout << "Out of bounds" << std::endl;
			exit(1);
		}

		return m_materials[meshIndex];
	}

	int GLModel::Size()
	{
		return m_meshes.size();
	}
}