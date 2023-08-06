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

        Animator(const Animation& animation);

        void Load(const Animation& animation);

        float GetDuration() const;

        void Animator::UpdateAnimation(float dt);

        std::vector<Math::mat4>& GetFinalBoneMatrices();

        std::string GetName();


    private:
        const Bone* GetBone(const std::string& name) const;
        void CalculateBoneTransform(const SceneNode* node, Math::mat4 parentTransform);

        const Animation* m_loadedAnimation;
        float m_lastTime = -1.0f;
        float m_currentTime;
        std::vector<Math::mat4> m_FinalBoneMatrices;
    };
}