#pragma once

#include <vector>
#include <string>
#include <glm/gtx/quaternion.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "AEngine/Math/Math.h"

namespace AEngine
{
    struct KeyPosition
    {
        Math::vec3 position;
        float timeStamp;
    };

    struct KeyRotation
    {
        Math::quat rotation;
        float timeStamp;
    };

    struct KeyScale
    {
        Math::vec3 scale;
        float timeStamp;
    };

    class Bone
    {
    public:

        Bone(const aiNodeAnim* animNode);
        Math::mat4 GetLocalTransform(float animationTime);
        std::string GetBoneName() const;

    private:

        void LoadData(const aiNodeAnim* animNode);
        
        float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
        Math::mat4 InterpolatePosition(float animationTime);
        Math::mat4 InterpolateRotation(float animationTime);
        Math::mat4 Bone::InterpolateScaling(float animationTime);

        std::vector<KeyPosition> m_positions;
        std::vector<KeyRotation> m_rotations;
        std::vector<KeyScale> m_scales;

        std::string m_name;

    };
}