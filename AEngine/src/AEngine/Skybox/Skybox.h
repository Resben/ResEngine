/**
 * \file
 * \author Geoff Candy
*/
#pragma once
#include "AEngine/Math/Math.h"
#include "AEngine/Render/Shader.h"
#include "CubeMapTexture.h"
#include "SkyboxMesh.h"
#include <string>
#include <vector>

namespace AEngine
{
		/**
		 * \class Skybox
		 * \brief Renders a skybox
		*/
	class Skybox
	{
	public:
			/**
			 * \param[in] texturePaths Paths to the skybox textures
			*/
		Skybox(const std::vector<std::string>& texturePaths);
			/**
			 * \brief Renders the skybox
			 * \param[in] shader The shader to use
			 * \param[in] projection The projection matrix
			 * \param[in] view The view matrix
			*/
		void Render(Shader& shader, const Math::mat4& projection, const Math::mat4& view);

	private:
		SkyboxMesh m_mesh;
		CubeMapTexture m_texture;
	};
}
