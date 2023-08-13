/**
 * \file
 * \author Ben Hawkins (34112619)
**/
#pragma once
#include "AEngine/Render/Material.h"

#include <assimp/material.h>

namespace AEngine
{
		/**
		 * \class AssimpMaterial
		 * \brief Contains implementation to load Material data using Assimp
		**/
	class AssimpMaterial : public Material
	{
	public:
		virtual ~AssimpMaterial() = default;
			/**
			 * \brief Constructor to create a Material from an Assimp object
			 * \param[in] ident Asset ident
			 * \param[in] path Asset path
			 * \param[in] material Assimp material to load from
			 * \param[in] directory directory of the model object to load textures from
			*/
		AssimpMaterial(const std::string& ident, const std::string& path, const aiMaterial* material, const std::string& directory);

	private:
			/**
			 * \brief Generate a material from a Assimp material
			 * \param[in] ai_mat Assimp material to read
			**/
		void GenerateMaterial(const aiMaterial* ai_mat);
			/**
			 * \brief Load a specific texture type from material
			 * \param[in] ai_material Assimp material to load from
			 * \param[in] ai_type Assimp material type to load
			**/
		void LoadTextures(const aiMaterial* ai_material, const aiTextureType ai_type);

	private:
		std::string m_directory;
	};
}