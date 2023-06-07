#include "Model.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Math/Math.h"
#include "AEngine/Resource/AssetManager.h"
#include "RenderCommand.h"

#ifdef AE_RENDER_OPENGL
#include "Platform/OpenGL/OpenGLMesh.h"
#endif

namespace AEngine
{
	SharedPtr<Model> Model::Create(const std::string& ident, const std::string& fname)
	{
		return MakeShared<Model>(ident, fname);
	}

	using mesh_material = std::pair<SharedPtr<Mesh>, int>;

	Model::Model(const std::string& ident, const std::string& path)
		: Asset(ident, path)
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

		if(scene->HasAnimations())
			m_animations.Load(scene);

		AE_LOG_TRACE("Model::Constructor::Success -> {}", path);
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
	mesh_material Model::CreateMesh(aiMesh* mesh)
	{
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		std::vector<int> boneIDs;
		std::vector<float> boneWeights;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			vertices.push_back(mesh->mVertices[i].x);
			vertices.push_back(mesh->mVertices[i].y);
			vertices.push_back(mesh->mVertices[i].z);

			if (mesh->HasNormals())
			{
				vertices.push_back(mesh->mNormals[i].x);
				vertices.push_back(mesh->mNormals[i].y);
				vertices.push_back(mesh->mNormals[i].z);
			}
			if (mesh->HasTextureCoords(0))
			{
				vertices.push_back(mesh->mTextureCoords[0][i].x);
				vertices.push_back(mesh->mTextureCoords[0][i].y);
			}
			else
			{
				vertices.push_back(0.0f);
				vertices.push_back(0.0f);
			}
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (unsigned int y = 0; y < face.mNumIndices; y++)
			{
				indices.push_back(face.mIndices[y]);
			}
		}

		if(mesh->HasBones())
		{
			boneIDs.resize(vertices.size() * MAX_BONE_INFLUENCE);
			boneWeights.resize(vertices.size() * MAX_BONE_INFLUENCE);

			for (unsigned int t = 0; t < boneIDs.size(); t++)
			{
				boneIDs[t] = -1;
				boneWeights[t] = 0.0f;
			}

			LoadMeshBones(mesh, boneWeights, boneIDs);
		}

		// generate structures
		m_indexes.push_back(mesh->mMaterialIndex);
		return std::make_pair(
			Mesh::Create(
				vertices.data(), static_cast<unsigned int>(vertices.size()), 
				indices.data(), static_cast<unsigned int>(indices.size()), 
				boneIDs.data(), boneWeights.data(), MAX_BONE_INFLUENCE),
				mesh->mMaterialIndex
			);
	}

	int Model::NameToID(std::string& name, aiBone* bone)
	{
		auto it = m_BoneInfoMap.find(name);
		if (it != m_BoneInfoMap.end())
		{
			return it->second;
		}

		int id = static_cast<int>(m_BoneInfoMap.size());
		m_BoneInfoMap.emplace(name, id);
		return id;
	}
	
	void Model::LoadMeshBones(aiMesh* mesh, std::vector<float>& BoneWeights, std::vector<int>& BoneIDs)
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

	void Model::Clear()
	{
		m_materials.clear();
		m_meshes.clear();
		AE_LOG_DEBUG("Model::Clear");
	}

	void Model::Render(const Math::mat4& transform, const Shader &shader, const Math::mat4 &projectionView) const
	{
		shader.Bind();
		shader.SetUniformInteger("u_texture1", 0);
		shader.SetUniformMat4("u_transform", transform);
		shader.SetUniformMat4("u_projectionView", projectionView);

		for (auto it = m_meshes.begin(); it != m_meshes.end(); ++it)
		{
			/// @todo Make this work with other material types...
			SharedPtr<Texture> tex = AssetManager<Texture>::Instance().Get(GetMaterial(it->second)->DiffuseTexture);
			Mesh& mesh = *(it->first);

			tex->Bind();
			mesh.Bind();

			// draw
			RenderCommand::DrawIndexed(PrimitiveDraw::Triangles, mesh.GetIndexCount(), 0);

			tex->Unbind();
			mesh.Unbind();
		}

		shader.Unbind();
	}

	void Model::Render(const Math::mat4& transform, const Shader& shader, const Math::mat4 & projectionView, const TimeStep dt)
	{
		shader.Bind();
		shader.SetUniformInteger("u_texture1", 0);
		shader.SetUniformMat4("u_transform", transform);
		shader.SetUniformMat4("u_projectionView", projectionView);

		m_animations.UpdateAnimation(dt);

		auto transforms = m_animations.GetFinalBoneMatrices();
		for (int i = 0; i < transforms.size(); ++i)
			shader.SetUniformMat4("u_finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

		std::vector<std::pair<SharedPtr<Mesh>, int>>::const_iterator it;
		for (it = m_meshes.begin(); it != m_meshes.end(); ++it)
		{
			/// @todo Make this work with other material types...
			SharedPtr<Texture> tex = AssetManager<Texture>::Instance().Get(GetMaterial(it->second)->DiffuseTexture);
			Mesh& mesh = *(it->first);			

			tex->Bind();
			mesh.Bind();

			// draw
			RenderCommand::DrawIndexed(PrimitiveDraw::Triangles, mesh.GetIndexCount(), 0);

			tex->Unbind();
			mesh.Unbind();
		}

		shader.Unbind();
	}

	std::string Model::LoadTextures(aiMaterial* mat, aiTextureType type)
	{
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			std::string filename = str.C_Str();
			Size_t last = filename.find_last_of("\\");

			if (last != std::string::npos)
				filename = filename.substr(last + 1);

			std::string path = m_directory + "/" + filename;

			AssetManager<Texture>::Instance().Load(path);

				// This is only necessary for dancing_vampire.dae
				// because it's file directory is set as /textures/
				// Reading models should be organisation like this later
			Size_t last2 = filename.find_last_of("/");
			if(last2 != std::string::npos)
				filename = filename.substr(last2 + 1);

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
				//material.SpecularTexture = LoadTextures(mat, aiTextureType_SPECULAR);
				m_materials.emplace(std::make_pair(m_indexes[i], material));
			}
		}
	}

	const Material* Model::GetMaterial(int meshIndex) const
	{
		std::map<int, Material>::const_iterator it;
		it = m_materials.find(meshIndex);
		if (it != m_materials.end())
			return &it->second;
		else
			return nullptr;
	}
}