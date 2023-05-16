/**
 * \file
 * \author Christien Alden (34119981)
 * \brief SceneManager header file
*/
#pragma once
#include <string>
#include "Scene.h"

namespace AEngine
{
		/**
		 * \class SceneManager
		 * \brief Manages the scenes in the game
		 * \author Christien Alden (34119981)
		*/
	class SceneManager
	{
	public:
			/**
			 * \brief Loads scene into the scene manager
			 * \param[in] ident name of scene to create
			 * \return Pointer to loaded scene
			 * \note If scene with the same ident already exists,
			 * it won't be loaded and nullptr will be returned
			*/
		static Scene* CreateScene(const std::string& ident);
			/**
			 * \brief Unloads scene from the scene manager
			 * \param[in] ident Scene ident to unload
			 * \retval true Scene was unloaded
			 * \retval false Scene didn't exist
			 * \warning This will completely remove the scene from memory
			 * and call its destructor, any pointers to this scene will be invalid
			*/
		static bool UnloadScene(const std::string& ident);
			/**
			 * \brief Unloads all scenes from the scene manager
			 * \warning This will completely remove all scenes from memory
			 * \see UnloadScene
			*/
		static void UnloadAllScenes();
			/**
			 * \brief Returns vector of all scene idents
			 * \return Vector of all scene idents
			*/
		static std::vector<std::string> GetSceneIdents();
			/**
			 * \brief Checks if scene with given ident exists
			 * \param[in] ident Scene ident to check
			 * \retval true Scene exists
			 * \retval false Scene doesn't exist
			*/
		static bool HasScene(const std::string& ident);

			/**
			 * \brief Sets active scene
			 * \param[in] ident Scene ident to set as active
			 * \retval true Scene was set as active
			 * \retval false Scene doesn't exist
			*/
		static bool SetActiveScene(const std::string& ident);
			/**
			 * \brief Returns pointer to active scene
			 * \return Pointer to active scene
			 * \retval nullptr No active scene
			*/
		static Scene* GetActiveScene();
			/**
			 * \brief Returns pointer to scene with given ident
			 * \param[in] ident Scene ident to get
			 * \return Pointer to scene with given ident
			 * \retval nullptr Scene doesn't exist
			*/
		static Scene* GetScene(const std::string& ident);
	};
}
