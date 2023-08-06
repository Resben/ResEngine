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

	const std::string& Animation::GetName() const
	{
		return m_name;
	}

	const float Animation::GetDuration() const
	{
		return m_duration;
	}

	const float Animation::GetTicksPerSecond() const
	{
		return m_ticksPerSecond;
	}

	const std::vector<Bone>& Animation::GetBones() const
	{
		return m_bones;
	}

	const std::map<std::string, BoneInfo>& Animation::GetBoneMap() const
	{
		return m_BoneInfoMap;
	}

	const SceneNode& Animation::GetRoot() const
	{
		return m_RootNode;
	}

}