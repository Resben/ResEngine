#pragma once

#include <vector>
#include <map>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Bone.h"
#include "AEngine/Resource/Asset.h"
#include "AEngine/Math/Math.h"
#include "AEngine/Core/Types.h"

namespace AEngine
{

    struct AnimationData
    {
        float duration;
        int ticksPerSecond;
        float currentTime;
        float lastTime = -1.0f;
        std::vector<Bone> bones;
    };

    struct BoneInfo
    {
        int id;
        Math::mat4 offset;
    };

    struct SceneNode
    {
        Math::mat4 transformation;
        std::string name;
        int numChildren;
        std::vector<SceneNode> children;
    };

    class Animation
    {
    public:
        ~Animation();
		Animation();

        void Load(const aiScene* scene);
        void Clear();

        void SetAnimation(std::string id);
        void UpdateAnimation(float dt);
        std::vector<Math::mat4>& GetFinalBoneMatrices();

    private:

        void MapBones(const aiScene* scene);
        void ProcessNode(SceneNode& dest, const aiNode* src);
        void LoadAnimation(const aiAnimation* animation);

        void CalculateBoneTransform(const SceneNode* node, Math::mat4 parentTransform);
        Bone* GetBone(const std::string& name);

        AnimationData m_currentAnimation;
        bool isAnimated;
        std::vector<Math::mat4> m_FinalBoneMatrices;
        SceneNode m_RootNode;
        std::map<std::string, BoneInfo> m_BoneInfoMap;
        std::map<std::string, AnimationData> m_animations;
    };
}