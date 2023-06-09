#include "Animation.h"
#include "AEngine/Core/Logger.h"

namespace AEngine
{

	SharedPtr<Animation> Animation::Create(const std::string& ident, const std::string& fname)
	{
		return MakeShared<Animation>(ident, fname);
	}

	Animation::Animation(const std::string ident, const std::string& fname)  
		: Asset(ident, fname), m_name(ident)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(fname, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			return;

		if (!scene || !scene->mRootNode)
			AE_LOG_ERROR("Animation::Constructor::Failed to load animation");

		MapBones(scene);
		ProcessNode(m_RootNode, scene->mRootNode);

		aiAnimation* animation = scene->mAnimations[0];

		m_duration = static_cast<float>(animation->mDuration);
		m_ticksPerSecond = static_cast<float>(animation->mTicksPerSecond);

		for (unsigned int i = 0; i < animation->mNumChannels; i++)
			m_bones.push_back(Bone(animation->mChannels[i]));

		AE_LOG_DEBUG("Animation::Constructor::Animation loaded -> {}", ident);
	}

	void Animation::ProcessNode(SceneNode& node, const aiNode* src)
	{
		if (!src)
			AE_LOG_ERROR("Animation::ProcessNode::Error reading node data");

		node.name = src->mName.C_Str();
		node.transformation = Math::transpose(Math::make_mat4(&src->mTransformation.a1));
		node.numChildren = src->mNumChildren;

		for (unsigned int i = 0; i < src->mNumChildren; i++)
		{
				// Load children into struct vector
			SceneNode newData;
			ProcessNode(newData, src->mChildren[i]);
			node.children.push_back(newData);
		}
	}

	void Animation::MapBones(const aiScene* scene)
	{
		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			for (unsigned int y = 0; y < scene->mMeshes[i]->mNumBones; y++)
			{
					// Check if bone name already exists
				auto it = m_BoneInfoMap.find(scene->mMeshes[i]->mBones[y]->mName.C_Str());
				if (it != m_BoneInfoMap.end())
					continue;

					// else load bone data
				BoneInfo info;
				info.id = static_cast<int>(m_BoneInfoMap.size());
				info.offset = Math::transpose(Math::make_mat4(&scene->mMeshes[i]->mBones[y]->mOffsetMatrix.a1));
				m_BoneInfoMap.emplace(scene->mMeshes[i]->mBones[y]->mName.C_Str(), info);
			}
		}
	}

	Animation::~Animation() {}

	std::string& Animation::GetName()
	{
		return m_name;
	}

	float Animation::GetDuration()
	{
		return m_duration;
	}

	float Animation::GetTicksPerSecond()
	{
		return m_ticksPerSecond;
	}

	std::vector<Bone>& Animation::GetBones()
	{
		return m_bones;
	}

	std::map<std::string, BoneInfo>& Animation::GetBoneMap()
	{
		return m_BoneInfoMap;
	}

	SceneNode& Animation::GetRoot()
	{
		return m_RootNode;
	}

}