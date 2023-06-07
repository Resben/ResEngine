#include "Bone.h"
#include "AEngine/Math/Math.h"
#include "AEngine/Core/Logger.h"

namespace AEngine
{
    Bone::Bone(const aiNodeAnim* animNode)
    {
        m_name = animNode->mNodeName.C_Str();
        LoadData(animNode);
    }

        // Load Position+Rotation+Scale key frames
    void Bone::LoadData(const aiNodeAnim* animNode)
    {
        for (int p = 0; p < animNode->mNumPositionKeys; p++)
        {
            KeyPosition data;
            aiVectorKey key = animNode->mPositionKeys[p];
            data.position = Math::vec3(key.mValue.x, key.mValue.y, key.mValue.z);
            data.timeStamp = key.mTime;
            m_positions.push_back(data);
        }

        for (int r = 0; r < animNode->mNumRotationKeys; r++)
        {
            KeyRotation data;
            aiQuatKey key = animNode->mRotationKeys[r];
            data.rotation = Math::quat(key.mValue.w, key.mValue.x, key.mValue.y, key.mValue.z);
            data.timeStamp = key.mTime;
            m_rotations.push_back(data);
        }

        for (int s = 0; s < animNode->mNumScalingKeys; s++)
        {
            KeyScale data;
            aiVectorKey key = animNode->mScalingKeys[s];
            data.scale = Math::vec3(key.mValue.x, key.mValue.y, key.mValue.z);
            data.timeStamp = key.mTime;
            m_scales.push_back(data);
        }
    }

    Math::mat4 Bone::GetLocalTransform(float animationTime)
    { 
        Math::mat4 translation = InterpolatePosition(animationTime);
        Math::mat4 rotation = InterpolateRotation(animationTime);
        Math::mat4 scale = InterpolateScaling(animationTime);
        translation * rotation * scale;

        return translation * rotation * scale;
    }

    std::string Bone::GetBoneName() const
    { 
        return m_name; 
    }

    float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
    {
        return (animationTime - lastTimeStamp) / (nextTimeStamp - lastTimeStamp);
    }

        // Interpolate position between two keys by a scale factor
    Math::mat4 Bone::InterpolatePosition(float animationTime)
    {
        Math::vec3 position;

        if (m_positions.size() > 1)
        {
            int posIndex = 0;
            for (int i = 0; i < m_positions.size() - 1; i++)
            {
                if (animationTime < m_positions[i + 1].timeStamp)
                {
                    posIndex = i;
                    break;
                }
            }

            float scaleFactor = GetScaleFactor(m_positions[posIndex].timeStamp, m_positions[posIndex + 1].timeStamp, animationTime);
            position = Math::mix(m_positions[posIndex].position, m_positions[posIndex + 1].position, scaleFactor);
        }
        else
            position = m_positions[0].position;

        return Math::translate(Math::mat4(1.0f), position);
    }

        // Interpolate rotation between two keys by a scale factor
    Math::mat4 Bone::InterpolateRotation(float animationTime)
    {
        Math::quat rotation;

        if (m_rotations.size() > 1)
        {        
            int rotIndex = 0;
            for (int i = 0; i < m_rotations.size() - 1; i++)
            {
                if (animationTime < m_rotations[i + 1].timeStamp)
                {
                    rotIndex = i;
                    break;
                }
            }

            float scaleFactor = GetScaleFactor(m_rotations[rotIndex].timeStamp, m_rotations[rotIndex + 1].timeStamp, animationTime);
            rotation = Math::slerp(m_rotations[rotIndex].rotation, m_rotations[rotIndex + 1].rotation, scaleFactor);
            rotation = Math::normalize(rotation);
        }
        else
            rotation = m_rotations[0].rotation;

        return Math::toMat4(rotation);
    }

        // Interpolate scaling between two keys by a scale factor
    Math::mat4 Bone::InterpolateScaling(float animationTime)
    {
        Math::vec3 scale;

        if (m_scales.size() > 1)
        {
            int scaleIndex = 0;
            for (int i = 0; i < m_scales.size() - 1; i++)
            {
                if (animationTime < m_scales[i + 1].timeStamp)
                {
                    scaleIndex = i;
                    break;
                }
            }

            float scaleFactor = GetScaleFactor(m_scales[scaleIndex].timeStamp, m_scales[scaleIndex + 1].timeStamp, animationTime);
            scale = Math::mix(m_scales[scaleIndex].scale, m_scales[scaleIndex + 1].scale, scaleFactor);
        }
        else
            scale = m_scales[0].scale;

        return Math::scale(Math::mat4(1.0f), scale);
    }
}