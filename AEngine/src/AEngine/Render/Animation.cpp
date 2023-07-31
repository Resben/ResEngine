#include "Animation.h"
#include "AEngine/Core/Logger.h"

namespace AEngine
{
	Animation::Animation(const aiScene* scene, const std::map<std::string, BoneInfo>& bone_map, int animationIndex)
	{
		m_BoneInfoMap = bone_map;

		ProcessNode(m_RootNode, scene->mRootNode);

		aiAnimation* animation = scene->mAnimations[animationIndex];

		m_duration = static_cast<float>(animation->mDuration);
		m_ticksPerSecond = static_cast<float>(animation->mTicksPerSecond);
		m_name = animation->mName.C_Str();

		for (unsigned int i = 0; i < animation->mNumChannels; i++)
			m_bones.push_back(Bone(animation->mChannels[i]));

		AE_LOG_DEBUG("Animation::Constructor::Animation loaded -> {}", animation->mName.C_Str());
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