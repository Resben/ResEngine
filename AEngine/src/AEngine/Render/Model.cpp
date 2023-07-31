#include "Model.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Math/Math.h"
#include "AEngine/Resource/AssetManager.h"
#include "RenderCommand.h"

namespace AEngine
{
	SharedPtr<Model> Model::Create(const std::string& ident, const std::string& fname)
	{
		return MakeShared<Model>(ident, fname);
	}

	Model::Model(const std::string& ident, const std::string& path)
		: Asset(ident, path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			return;
		}
		AE_LOG_DEBUG("Model::Constructor::Loading -> {}", path);

		m_directory = path.substr(0, path.find_last_of('/'));

		ProcessNode(scene->mRootNode, scene);

		if(scene->HasAnimations())
		{
			for(int i = 0; i < scene->mNumAnimations; i++)
			{
				SharedPtr<Animation> anim = MakeShared<Animation>(scene, m_BoneInfoMap, i);
				m_animations.emplace(scene->mAnimations[i]->mName.C_Str(), anim);
			}
		}

		AE_LOG_TRACE("Model::Constructor::Success -> {}", path);
	}

	Model::~Model()
	{
		Clear();
	}

	void Model::ProcessNode(const aiNode* node, const aiScene* scene)
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
	Model::mesh_material Model::CreateMesh(const aiScene* scene, const aiMesh* mesh)
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
				normalData.push_back(mesh->mNormals[i].x);
				normalData.push_back(mesh->mNormals[i].y);
				normalData.push_back(mesh->mNormals[i].z);
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

	int Model::NameToID(std::string& name, const aiBone* bone)
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

	void Model::LoadMeshBones(const aiMesh* mesh, std::vector<float>& BoneWeights, std::vector<int>& BoneIDs)
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

	void Model::Render(const Math::mat4& transform, const Shader& shader, const Math::mat4 & projectionView, Animator& animation, const TimeStep dt)
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

	void Model::LoadTextures(SharedPtr<Material> ae_material, AE_TEXTURETYPE ae_type, const aiMaterial* ai_material, const aiTextureType ai_type)
	{
		if(ai_material->GetTextureCount(ai_type) > 0)
		{
			aiString str;
			if(ai_material->GetTexture(ai_type, 0, &str) == AI_SUCCESS)
			{
				std::string filename = str.C_Str();
				Size_t last = filename.find_last_of("\\");

				if (last != std::string::npos)
					filename = filename.substr(last + 1);

				std::string path = m_directory + "/" + filename;
				ae_material->addTexture(ae_type, AssetManager<Texture>::Instance().Load(path));
			}
		}
	}

	SharedPtr<Material> Model::GenerateMaterials(const aiScene* scene, int index)
	{
		if (index >= 0)
		{
			SharedPtr<Material> ae_mat = MakeShared<Material>();
			aiMaterial* ai_mat = scene->mMaterials[index];
				// Add more if necessary
			LoadTextures(ae_mat, AE_TEXTURETYPE_DIFFUSE, ai_mat, aiTextureType_DIFFUSE);
			LoadTextures(ae_mat, AE_TEXTURETYPE_SPECULAR, ai_mat, aiTextureType_SPECULAR);
			LoadTextures(ae_mat, AE_TEXTURETYPE_NORMALS, ai_mat, aiTextureType_NORMALS);
			LoadTextures(ae_mat, AE_TEXTURETYPE_SPECULAR, ai_mat, aiTextureType_SPECULAR);
			return ae_mat;
		}
	}

	const VertexArray* Model::GetMesh(int index) const
	{
		if (index > m_meshes.size())
		{
			AE_LOG_FATAL("Model::GetMesh::Out of Bounds");
		}

		return (m_meshes[index].first).get();
	}

	const Material* Model::GetMaterial(int index) const
	{
		if (index > m_meshes.size())
		{
			AE_LOG_FATAL("Model::GetMesh::Out of Bounds");
		}

		return (m_meshes[index].second).get();
	}

	Animation* Model::GetAnimation(std::string id)
	{
			// Caused when you try serialize a model that does not exist
		if(this == nullptr)
		{
			AE_LOG_FATAL("Model::GetAnimation::Model is Null -> This may be because you have a null model in your component");
		}

		std::map<std::string, SharedPtr<Animation>>::const_iterator it;
		it = m_animations.find(id);
		if (it != m_animations.end())
			return it->second.get();
		else
			return nullptr;
	}
}