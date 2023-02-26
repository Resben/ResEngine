/**
 *	@file
 *	@brief		Model manager to load 1 or more meshes from an obj file
 *	@author		Ben Hawkins
 *	@author		Christien Alden (34119981)
 *	@date		12/09/2022
**/
#pragma once
#include <string>
#include <map>
#include <memory>
#include "../Render/Mesh.h"

namespace AEngine
{
		/**
		 * @class MeshManager
		 * @brief Holds shared pointers to loaded Meshes
		 * @author Ben Hawkins
		 * @author Christien Alden (34119981)
		**/
	class MeshManager
	{
	public:
			/**
			 * @retval MeshManager* Instance of MeshManager
			**/
		static MeshManager* Instance();

			/**
			 * @brief Clears loaded Meshes
			 * @retval void
			**/
		void Clear();

			/**
			 * @brief Loads all Meshes contained in directory
			 * @param[in] dir directory of Meshes
			 * @retval void
			**/
		void LoadModelDirectory(const std::string& dir);

			/**
			 *	@brief		Loads a single mesh from an obj file
			 *	@param[in]	filename of obj to load
			 *	@retval		std::shared_ptr<Mesh> to mesh just loaded
			**/
		std::shared_ptr<Mesh> LoadModel(const std::string& filename);

			/**
			 * @brief Searches loaded Meshes for a given model
			 * @param[in] meshName to search for
			 * @retval Mesh* if found
			 * @retval nullptr if not found
			**/
		Mesh* GetMesh(const std::string& meshName);

		MeshManager(const MeshManager&) = delete;
		void operator=(const MeshManager&) = delete;

	private:
		static MeshManager* s_instance;
		MeshManager() = default;

			/// Map of mesh names to Mesh objects
		std::map<std::string, std::shared_ptr<Mesh>> m_meshes;
	};
}
