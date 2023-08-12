/**
 * @file
 * @author Ben Hawkins (34112619)
**/

#pragma once
#include "AEngine/Core/TimeStep.h"
#include "AEngine/Core/Types.h"
#include "AEngine/Resource/Asset.h"

#include "VertexArray.h"
#include "Shader.h"
#include "Material.h"
#include "Animation.h"
#include "Animator.h"

#include <string>

#define MAX_BONE_INFLUENCE 4

namespace AEngine
{
		/**
		 * @class Model
		 * @brief Model object, contains material and animations
		**/
	class Model : public Asset
	{
	public:
		using mesh_material = std::pair<SharedPtr<VertexArray>, std::string>;
			/**
			 * @brief Constructor
			 * @param[in] ident Asset ident
			 * @param[in] path Asset path
			**/
		Model(const std::string& ident, const std::string& path);
			/**
			 * @brief Clear model data
			**/
		virtual void Clear() = 0;
		virtual void Render(const Math::mat4& transform, const Shader& shader, const Math::mat4& projectionView) const = 0;
		virtual void Render(const Math::mat4& transform, const Shader& shader, const Math::mat4& projectionView, Animator& animator, const TimeStep time) = 0;
			/**
			 * @brief Get material for a mesh by index
			 * @param[in] meshIndex Index of mesh
			**/
		virtual const VertexArray* GetMesh(int index) const = 0;

		virtual const std::string& GetMaterial(int meshIndex) const = 0;
			/**
			 * @brief Get animation object for model
			 * @return Animation reference
			**/

			/**
			 * @brief Deconstructor
			**/
		virtual ~Model() = default;

		virtual std::vector<mesh_material>::const_iterator begin() const = 0;
		virtual std::vector<mesh_material>::const_iterator end() const = 0;

		static SharedPtr<Model> Create(const std::string& ident, const std::string& fname);
	};
}