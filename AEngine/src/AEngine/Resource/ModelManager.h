/**
 *	@file
 *	@brief		Model manager to store a model
 *	@author		Ben Hawkins
 *	@date		12/09/2022
**/
#pragma once
#include <string>
#include <map>
#include "../Render/Model.h"

namespace AEngine
{
		/**
		 * @class ModelManager
		 * @brief Holds shared pointers to loaded Models
		 * @author Ben Hawkins
		**/
	class ModelManager
	{
	public:
			/**
			 * @retval ModelManager* Instance of ModelManager
			**/
		static ModelManager* Instance();

			/**
			 * @brief Clears loaded Models
			 * @retval void
			**/
		void Clear();

			/**
			 *	@brief		Loads a single model from a file
			 *	@param[in]	filename of the file that contains the mesh data
			 *	@retval		void
			**/
		void LoadModel(const std::string& filename);

			/**
			 * @brief Searches loaded Models for a given model name
			 * @param[in] modelName to search for
			 * @retval Model* if found
			 * @retval nullptr if not found
			**/
		const Model* GetModel(const std::string& modelName) const;

		ModelManager(const ModelManager&) = delete;
		void operator=(const ModelManager&) = delete;

	private:
		static ModelManager* s_instance;
		ModelManager() = default;

			/// Map of model names to Model objects
		std::map<std::string, std::shared_ptr<Model>> m_models;
	};
}
