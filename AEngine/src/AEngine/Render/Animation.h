/**
 * \file
 * \author Ben Hawkins (34112619)
**/

#pragma once

#include <vector>
#include <map>
#include <string>

#include "Bone.h"
#include "AEngine/Resource/Asset.h"
#include "AEngine/Math/Math.h"
#include "AEngine/Core/Types.h"

namespace AEngine
{
		/**
		 * \struct BoneInfo
		 * \brief Stores a bone ID and local offset
		**/
	struct BoneInfo
	{
		int id;
		Math::mat4 offset;
	};

		/**
		 * \struct SceneNode
		 * \brief Stores Assimp node data
		**/
	struct SceneNode
	{
		Math::mat4 transformation;
		std::string name;
		int numChildren;
		std::vector<SceneNode> children;
	};

		/**
		 * \class Animation
		 * \brief Abstract class that provides methods to use Animation
		**/
	class Animation : public Asset
	{
	public:
			/// \brief Deconstructor
		virtual ~Animation() = default;

			/** 
			 * \brief Get name of animation
			 * \return string&
			*/
		const std::string& GetName() const; 
			/** 
			 * \brief Get duration of animation
			 * \return float
			*/
		const float GetDuration() const;
			/** 
			 * \brief Get ticks per second
			 * \return float
			*/
		const float GetTicksPerSecond() const;
			/** 
			 * \brief Get vector of Bones
			 * \return vector<SharedPtr<Bone>>&
			*/
		const std::vector<SharedPtr<Bone>>& GetBones() const;
			/** 
			 * \brief Get map of bone name to information
			 * \return map<string, BoneInfo>&
			*/
		const std::map<std::string, BoneInfo>& GetBoneMap() const;
			/** 
			 * \brief Get root of scene node
			 * \return SceneNode&
			*/
		const SceneNode& GetRoot() const;
	
	protected:
			/**
			 * \brief Protected constructor
			 * \param[in] ident Unique asset identifier
			 * \param[in] path Asset path
			 * \note path is of parent asset
			*/
		Animation(const std::string& ident, const std::string& path);
		
		SceneNode m_RootNode;
		std::map<std::string, BoneInfo> m_BoneInfoMap;
		std::string m_name;
		float m_duration;
		float m_ticksPerSecond;
		std::vector<SharedPtr<Bone>> m_bones;
	};
}