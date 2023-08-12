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

#include <map>
#include <string>
#include <utility>
#include <vector>

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
			 * @brief Clear model data
			**/
		void Clear();
		void Render(const Math::mat4& transform, const Shader& shader, const Math::mat4& projectionView) const;
		void Render(const Math::mat4& transform, const Shader& shader, const Math::mat4& projectionView, Animator& animator, const TimeStep time);
			/**
			 * @brief Get material for a mesh by index
			 * @param[in] meshIndex Index of mesh
			**/
		const VertexArray* GetMesh(int index) const;

		const std::string& GetMaterial(int meshIndex) const;
			/**
			 * @brief Get animation object for model
			 * @return Animation reference
			**/

			/**
			 * @brief Deconstructor
			**/
		virtual ~Model() = default;

		std::vector<mesh_material>::const_iterator begin() const { return m_meshes.begin(); }
		std::vector<mesh_material>::const_iterator end() const { return m_meshes.end(); }

		static SharedPtr<Model> Create(const std::string& ident, const std::string& fname);

	protected:
		std::vector<mesh_material> m_meshes;
			/**
			 * @brief Protected Constructor
			 * @param[in] ident Asset ident
			 * @param[in] path Asset path
			**/
		Model(const std::string& ident, const std::string& path);

		std::map<std::string, BoneInfo> m_BoneInfoMap;

		std::string m_directory;
	};
}