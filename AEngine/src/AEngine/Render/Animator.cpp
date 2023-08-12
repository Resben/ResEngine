#include "Animator.h"
#include "AEngine/Core/Logger.h"

namespace AEngine
{
    Animator::Animator() {}

    Animator::Animator(const Animation& animation)
	: m_loadedAnimation(&animation)
    {
        Load(animation);
    }

	void Animator::Load(const Animation& animation)
    {
		m_FinalBoneMatrices.clear();

		m_loadedAnimation = &animation;

		m_currentTime = 0;
		
		AE_LOG_DEBUG("Animation loaded: {}", m_loadedAnimation->GetName());

        for (unsigned int i = 0; i < 100; i++)
			m_FinalBoneMatrices.push_back(Math::mat4(1.0f));
    }

	float Animator::GetDuration() const
	{
		return m_loadedAnimation->GetDuration() / m_loadedAnimation->GetTicksPerSecond();
	}

	void Animator::UpdateAnimation(float dt)
	{
			// Loop animation seamlessly with fmod
		m_currentTime += m_loadedAnimation->GetTicksPerSecond() * dt;
		m_currentTime = fmod(m_currentTime, m_loadedAnimation->GetDuration());
		CalculateBoneTransform(&m_loadedAnimation->GetRoot(), Math::mat4(1.0f));
	}

	void Animator::CalculateBoneTransform(const SceneNode* node, Math::mat4 parentTransform)
	{
		Math::mat4 nodeTransform = node->transformation;
		const Bone* Bone = GetBone(node->name);

			// Check if node is a bone
		if (Bone)
			nodeTransform = Bone->GetLocalTransform(m_currentTime);

			// Apply parent transform to child transform
		Math::mat4 globalTransformation = parentTransform * nodeTransform;

  		const std::map<std::string, BoneInfo>& bonemap = m_loadedAnimation->GetBoneMap();

			// Store bone final matrice for bone ID
		if (bonemap.find(node->name) != bonemap.end())
			m_FinalBoneMatrices[bonemap.at(node->name).id] = globalTransformation * bonemap.at(node->name).offset;

			// Calculate matrices for bone children
		for (int i = 0; i < node->numChildren; i++)
			CalculateBoneTransform(&node->children[i], globalTransformation);
	}

	const Bone* Animator::GetBone(const std::string& name) const
	{
  		const std::vector<SharedPtr<Bone>> bones = m_loadedAnimation->GetBones();

		for (const auto& sharedPtrBone : bones)
		{
			if (sharedPtrBone->GetBoneName() == name)
				return sharedPtrBone.get();
		}
		return nullptr;
	}

	std::vector<Math::mat4>& Animator::GetFinalBoneMatrices()
	{
		return m_FinalBoneMatrices;
	}

	std::string Animator::GetName()
	{
		return m_loadedAnimation->GetName();
	}
}