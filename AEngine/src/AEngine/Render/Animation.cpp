#include "Animation.h"
#include "AEngine/Core/Application.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Render/RenderCommand.h"
#include "Platform/Assimp/AssimpAnimation.h"

namespace AEngine
{
	Animation::Animation(const std::string& ident, const std::string& parent)
		: Asset(ident, parent) {}

	const std::string& Animation::GetName() const
	{
		return m_name;
	}

	const float Animation::GetDuration() const
	{
		return m_duration;
	}

	const float Animation::GetTicksPerSecond() const
	{
		return m_ticksPerSecond;
	}

	const std::vector<SharedPtr<Bone>>& Animation::GetBones() const
	{
		return m_bones;
	}

	const std::map<std::string, BoneInfo>& Animation::GetBoneMap() const
	{
		return m_BoneInfoMap;
	}

	const SceneNode& Animation::GetRoot() const
	{
		return m_RootNode;
	}
}