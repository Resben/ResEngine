#pragma once

#include <vector>
#include <map>
#include <string>

#include "Animation.h"

namespace AEngine
{
    class Animator
    {
    public:
        Animator();

        Animator(Animation& animation);

        void Load(Animation& animation);

        void Animator::UpdateAnimation(float dt);

        void CalculateBoneTransform(const SceneNode* node, Math::mat4 parentTransform);

        Bone* GetBone(const std::string& name);

        std::vector<Math::mat4>& GetFinalBoneMatrices();

        std::string GetName();


    private:
		SceneNode m_RootNode;
		std::map<std::string, BoneInfo> m_BoneInfoMap;
		std::string m_name;
		float m_duration;
		float m_ticksPerSecond;
        float m_lastTime = -1.0f;
        float m_currentTime;
		std::vector<Bone> m_bones;
        std::vector<Math::mat4> m_FinalBoneMatrices;
    };
}