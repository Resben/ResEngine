/**
 * \file
 * \author Ben Hawkins (34112619)
**/

#pragma once

#include <vector>
#include <map>
#include <string>

#include "Animation.h"

namespace AEngine
{
		/**
		 * \class Animator
		 * \brief Loads an animation for a Skinned Model
		**/
    class Animator
    {
    public:
            /**
             * \brief Default constructor
             * \note No animation is present
            */
        Animator() = default;
            /**
             * \brief Constructor with animation
             * \param[in] animation Animation to reference
            */
        Animator(const Animation& animation);
            /**
             * \brief Load animation to animator
             * \param[in] animation Animation to reference
            */
        void Load(const Animation& animation);
            /**
             * \brief Get duration of loaded animation
             * \return float
            */
        float GetDuration() const;
            /**
             * \brief Updated loaded animation
             * \param[in] dt delta time
            */
        void Animator::UpdateAnimation(float dt);
            /**
             * \brief Get bone matrices for update time
             * \return vector<mat4>&
            */
        std::vector<Math::mat4>& GetFinalBoneMatrices();
            /**
             * \brief Get name of loaded animation
             * \return string&
            */
        const std::string& GetName();


    private:
            /**
             * \brief Get Bone by name ID
             * \return Bone*
            */
        const Bone* GetBone(const std::string& name) const;
            /**
             * \brief Calculates Bone transforms at a given time
             * \param[in] node Node to search
             * \param[in] parentTransform transform of parent
            */
        void CalculateBoneTransform(const SceneNode* node, Math::mat4 parentTransform);

        const Animation* m_loadedAnimation;
        float m_lastTime = -1.0f;
        float m_currentTime;
        std::vector<Math::mat4> m_FinalBoneMatrices;
    };
}