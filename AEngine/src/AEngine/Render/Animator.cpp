#include "Animator.h"

namespace AEngine
{
    Animator::Animator() {}

    Animator::Animator(Animation& animation)
    {
        Load(animation);
    }

    void Animator::Load(Animation& animation)
    {
		m_FinalBoneMatrices.clear();

        m_RootNode = animation.GetRoot();
		m_BoneInfoMap = animation.GetBoneMap();
		m_name = animation.GetName();
		m_duration = animation.GetDuration();
		m_ticksPerSecond = animation.GetTicksPerSecond();
		m_bones = animation.GetBones();
		m_currentTime = 0;
		
        for (unsigned int i = 0; i < 100; i++)
			m_FinalBoneMatrices.push_back(Math::mat4(1.0f));
    }

	float Animator::GetDuration() const
	{
		return m_duration / m_ticksPerSecond;
	}

	void Animator::UpdateAnimation(float dt)
	{
			// Loop animation seamlessly with fmod
		m_currentTime += m_ticksPerSecond * dt;
		m_currentTime = fmod(m_currentTime, m_duration);
		CalculateBoneTransform(&m_RootNode, Math::mat4(1.0f));
	}

	void Animator::CalculateBoneTransform(const SceneNode* node, Math::mat4 parentTransform)
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

	Bone* Animator::GetBone(const std::string& name)
	{
		for (unsigned int i = 0; i < m_bones.size(); i++)
		{
			if (m_bones[i].GetBoneName() == name)
				return &m_bones[i];
		}
		return nullptr;
	}

	std::vector<Math::mat4>& Animator::GetFinalBoneMatrices()
	{
		return m_FinalBoneMatrices;
	}

	std::string Animator::GetName()
	{
		return m_name;
	}


}