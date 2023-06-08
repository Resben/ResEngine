#include "Animation.h"
#include "AEngine/Core/Logger.h"

namespace AEngine
{
	//------------------- Load Data ---------------------//

	Animation::Animation() : isAnimated(false) {}

	void Animation::Load(const aiScene* scene)
	{
		if (!scene || !scene->mRootNode)
			AE_LOG_ERROR("Animation::Constructor::Failed to load animation");

		MapBones(scene);
		ProcessNode(m_RootNode, scene->mRootNode);

		aiAnimation* animation = scene->mAnimations[0];

		m_duration = static_cast<float>(animation->mDuration);
		m_ticksPerSecond = static_cast<float>(animation->mTicksPerSecond);

		for (unsigned int i = 0; i < animation->mNumChannels; i++)
			m_bones.push_back(Bone(animation->mChannels[i]));

		for (unsigned int i = 0; i < 100; i++)
			m_FinalBoneMatrices.push_back(Math::mat4(1.0f));
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

	//------------------- Play Animation ---------------------//

	void Animation::UpdateAnimation(float dt)
	{
		if(isAnimated)
		{
				// Loop animation seamlessly with fmod
			m_currentTime += m_ticksPerSecond * dt;
			m_currentTime = fmod(m_currentTime, m_duration);
			CalculateBoneTransform(&m_RootNode, Math::mat4(1.0f));
		}
	}

	void Animation::CalculateBoneTransform(const SceneNode* node, Math::mat4 parentTransform)
	{
		Math::mat4 nodeTransform = node->transformation;
		Bone* Bone = GetBone(node->name);

			// Check if node is a bone
		if (Bone)
			nodeTransform = Bone->GetLocalTransform(m_currentTime);

			// Apply parent transform to child transform
		Math::mat4 globalTransformation = parentTransform * nodeTransform;

			// Store bone final matrice for bone ID
		if (m_BoneInfoMap.find(node->name) != m_BoneInfoMap.end())
			m_FinalBoneMatrices[m_BoneInfoMap[node->name].id] = globalTransformation * m_BoneInfoMap[node->name].offset;

			// Calculate matrices for bone children
		for (int i = 0; i < node->numChildren; i++)
			CalculateBoneTransform(&node->children[i], globalTransformation);
	}

	Bone* Animation::GetBone(const std::string& name)
	{
		for (unsigned int i = 0; i < m_bones.size(); i++)
		{
			if (m_bones[i].GetBoneName() == name)
				return &m_bones[i];
		}
		return nullptr;
	}

	std::vector<Math::mat4>& Animation::GetFinalBoneMatrices()
	{
		return m_FinalBoneMatrices;
	}

	std::string Animation::GetName()
	{
		return m_name;
	}
}