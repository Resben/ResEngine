#pragma once

#include <vector>
#include <string>

#include "AEngine/Render/Shader.h"
#include "AEngine/Math/Math.h"

namespace AEngine
{
	class Skybox
	{
	public:
		Skybox(std::vector<std::string> faces);

		~Skybox();

		void load();

		void render(Shader& shader, Math::mat4& projection, Math::mat4& view);

	private:
		std::vector<std::string> m_Faces;
	
		unsigned int m_VAO;
		unsigned int m_VBO;
		unsigned int m_Texture;

		std::vector<float> m_Vertices;
	};
}
