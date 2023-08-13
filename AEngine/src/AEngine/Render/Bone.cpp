#include "Bone.h"

namespace AEngine
{
    const std::string& Bone::GetBoneName() const
	{ 
		return m_name; 
	}

	const float Bone::GetScaleFactor(float currentTimeStamp, float nextTimeStamp, float animationTime) const
	{
		return (animationTime - currentTimeStamp) / (nextTimeStamp - currentTimeStamp);
	}

	const Math::mat4 Bone::InterpolatePosition(float animationTime) const
	{
		Math::vec3 position;

			// Get closest index to animationTime
		if (m_positions.size() > 1)
		{
			unsigned int posIndex = 0;
			for (unsigned int i = 0; i < m_positions.size() - 1; i++)
			{
				if (animationTime < m_positions[i + 1].timeStamp)
				{
					posIndex = i;
					break;
				}
			}

				// Calculate scale factor between two keyframes
			float scaleFactor = GetScaleFactor(m_positions[posIndex].timeStamp, m_positions[posIndex + 1].timeStamp, animationTime);
				// Interpolate between two vectors based on scale
			position = Math::mix(m_positions[posIndex].position, m_positions[posIndex + 1].position, scaleFactor);
		}
		else
			position = m_positions[0].position;

		return Math::translate(Math::mat4(1.0f), position);
	}

	const Math::mat4 Bone::InterpolateRotation(float animationTime) const
	{
		Math::quat rotation;

			// Get closest index to animationTime
		if (m_rotations.size() > 1)
		{        
			unsigned int rotIndex = 0;
			for (unsigned int i = 0; i < m_rotations.size() - 1; i++)
			{
				if (animationTime < m_rotations[i + 1].timeStamp)
				{
					rotIndex = i;
					break;
				}
			}

				// Calculate scale factor between two keyframes
			float scaleFactor = GetScaleFactor(m_rotations[rotIndex].timeStamp, m_rotations[rotIndex + 1].timeStamp, animationTime);
				// Interpolate between two quaternions based on scale 
			rotation = Math::slerp(m_rotations[rotIndex].rotation, m_rotations[rotIndex + 1].rotation, scaleFactor);
			rotation = Math::normalize(rotation);
		}
		else
			rotation = m_rotations[0].rotation;

		return Math::toMat4(rotation);
	}

	const Math::mat4 Bone::InterpolateScaling(float animationTime) const
	{
		Math::vec3 scale;

			// Get closest index to animationTime
		if (m_scales.size() > 1)
		{
			unsigned int scaleIndex = 0;
			for (unsigned int i = 0; i < m_scales.size() - 1; i++)
			{
				if (animationTime < m_scales[i + 1].timeStamp)
				{
					scaleIndex = i;
					break;
				}
			}

				// Calculate scale factor between two keyframes
			float scaleFactor = GetScaleFactor(m_scales[scaleIndex].timeStamp, m_scales[scaleIndex + 1].timeStamp, animationTime);
				// Interpolate between two vectors based on scale
			scale = Math::mix(m_scales[scaleIndex].scale, m_scales[scaleIndex + 1].scale, scaleFactor);
		}
		else
			scale = m_scales[0].scale;

		return Math::scale(Math::mat4(1.0f), scale);
	}

    const Math::mat4 Bone::GetLocalTransform(float animationTime) const
	{ 
			// Interpolate all transforms
		Math::mat4 translation = InterpolatePosition(animationTime);
		Math::mat4 rotation = InterpolateRotation(animationTime);
		Math::mat4 scale = InterpolateScaling(animationTime);
		translation * rotation * scale;

			// return a final transform
		return translation * rotation * scale;
	}
}