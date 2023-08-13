#include "AssimpAnimation.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Resource/AssetManager.h"

namespace AEngine
{
	AssimpAnimation::AssimpAnimation(const std::string& ident, const std::string& parent, const aiScene* scene, const std::map<std::string, BoneInfo>& bone_map, int animationIndex)
    : Animation(ident, parent)
	{

		m_BoneInfoMap = bone_map;

		ProcessNode(m_RootNode, scene->mRootNode);

		aiAnimation* animation = scene->mAnimations[animationIndex];

		m_duration = static_cast<float>(animation->mDuration);
		m_ticksPerSecond = static_cast<float>(animation->mTicksPerSecond);
		m_name = animation->mName.C_Str();

		for (unsigned int i = 0; i < animation->mNumChannels; i++)
		{
			SharedPtr<Bone> bone = MakeShared<AssimpBone>(animation->mChannels[i]);
			m_bones.push_back(bone);
		}

		AE_LOG_DEBUG("Animation::Constructor::Animation loaded -> {}", animation->mName.C_Str());
	}

	void AssimpAnimation::ProcessNode(SceneNode& node, const aiNode* src)
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
}