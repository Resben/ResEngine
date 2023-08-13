/**
 * \file
 * \author Ben Hawkins (34112619)
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
		 * \class Model
		 * \brief Abstract class that provides methods to use a Model
		**/
	class Model : public Asset
	{
	public:
		using mesh_material = std::pair<SharedPtr<VertexArray>, std::string>;
			/**
			 * \brief Clear model data
			**/
		void Clear();
			/**
			 * \brief Render command for RenderableComponent
			 * \param[in] transform model transform
			 * \param[in] shader shader to use
			 * \param[in] projectionview camera projection view
			 * \todo remove shader pass responsibility to Material
			**/
		void Render(const Math::mat4& transform, const Shader& shader, const Math::mat4& projectionView) const;
			/**
			 * \brief Render command for SkinnedRenderableComponent
			 * \param[in] transform model transform
			 * \param[in] shader shader to use
			 * \param[in] projectionview camera projection view
			 * \param[in] animator contains a currently loaded animation
			 * \param[in] time TimeStep for updating animation
			 * \todo remove shader pass responsibility to Material
			**/
		void Render(const Math::mat4& transform, const Shader& shader, const Math::mat4& projectionView, Animator& animator, const TimeStep time);
			/**
			 * \brief Get VertexArray for a mesh by index
			 * \param[in] index Index of mesh
			**/
		const VertexArray* GetMesh(int index) const;
			/**
			 * \brief Get Material ID for a mesh by index
			 * \param[in] meshIndex Index of mesh
			**/
		const std::string& GetMaterial(int meshIndex) const;

		virtual ~Model() = default;

		std::vector<mesh_material>::const_iterator begin() const { return m_meshes.begin(); }
		std::vector<mesh_material>::const_iterator end() const { return m_meshes.end(); }

			/**
			 * \brief Asset manager create function
			 * \param[in] ident Asset ident
			 * \param[in] fname Asset path
			**/
		static SharedPtr<Model> Create(const std::string& ident, const std::string& fname);

	protected:
			/**
			 * \brief Protected Constructor
			 * \param[in] ident Asset ident
			 * \param[in] path Asset path
			**/
		Model(const std::string& ident, const std::string& path);

		std::vector<mesh_material> m_meshes;
		std::map<std::string, BoneInfo> m_BoneInfoMap;
		std::string m_directory;
	};
}