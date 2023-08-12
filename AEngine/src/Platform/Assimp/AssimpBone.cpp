#include "AssimpBone.h"

namespace AEngine
{
	AssimpBone::AssimpBone(const aiNodeAnim* animNode)
    {
        m_name = animNode->mNodeName.C_Str();
		LoadData(animNode);
    }

	void AssimpBone::LoadData(const aiNodeAnim* animNode)
	{
		for (unsigned int p = 0; p < animNode->mNumPositionKeys; p++)
		{
			KeyPosition data;
			aiVectorKey key = animNode->mPositionKeys[p];
			data.position = Math::vec3(key.mValue.x, key.mValue.y, key.mValue.z);
			data.timeStamp = static_cast<float>(key.mTime);
			m_positions.push_back(data);
		}

		for (unsigned int r = 0; r < animNode->mNumRotationKeys; r++)
		{
			KeyRotation data;
			aiQuatKey key = animNode->mRotationKeys[r];
			data.rotation = Math::quat(key.mValue.w, key.mValue.x, key.mValue.y, key.mValue.z);
			data.timeStamp = static_cast<float>(key.mTime);
			m_rotations.push_back(data);
		}

		for (unsigned int s = 0; s < animNode->mNumScalingKeys; s++)
		{
			KeyScale data;
			aiVectorKey key = animNode->mScalingKeys[s];
			data.scale = Math::vec3(key.mValue.x, key.mValue.y, key.mValue.z);
			data.timeStamp = static_cast<float>(key.mTime);
			m_scales.push_back(data);
		}
	}
}