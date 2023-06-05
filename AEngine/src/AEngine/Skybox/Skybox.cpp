#include "Skybox.h"

#include <stb/stb_image.h>

namespace AEngine
{
	Skybox::Skybox(std::vector<std::string> faces)
	{
		m_Texture = 0;

		m_Faces = faces;

		float m_Vertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
		 	1.0f, -1.0f, -1.0f,
		 	1.0f, -1.0f, -1.0f,
		 	1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
		};

		// glGenVertexArrays(1, &m_VAO);
		// glGenBuffers(1, &m_VBO);
		// glBindVertexArray(m_VAO);
		// glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		// glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices), &m_Vertices, GL_STATIC_DRAW);
		// glEnableVertexAttribArray(0);
		// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	}

	Skybox::~Skybox()
	{
		m_Vertices.clear();
		m_Faces.clear();
		//glDeleteVertexArrays(1, &m_VAO);
		//glDeleteBuffers(1, &m_VBO);
	}

	void Skybox::load()
	{
		// unsigned int textureID;
		// glGenTextures(1, &textureID);
		// glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		// int width, height, nrChannels;
		// for (unsigned int i = 0; i < m_Faces.size(); i++)
		// {
		// 	unsigned char* data = stbi_load(m_Faces[i].c_str(), &width, &height, &nrChannels, 0);
		// 	if (data)
		// 	{
		// 		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		// 		stbi_image_free(data);
		// 	}
		// 	else
		// 	{
		// 		std::cout << "Cubemap texture failed to load at path: " << m_Faces[i] << std::endl;
		// 		stbi_image_free(data);
		// 	}
		// }

		// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		// m_Texture = textureID;
	}

	void Skybox::render(Shader& shader, Math::mat4& projection, Math::mat4& camView)
	{
		// glDepthFunc(GL_LEQUAL);
		// shader.bind();
		// Math::mat4 view = Math::mat4(Math::mat3(camView));
		// shader.setMat4("view", view);
		// shader.setMat4("projection", projection);
		// glBindVertexArray(m_VAO);
		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);
		// glDrawArrays(GL_TRIANGLES, 0, 36);
		// glBindVertexArray(0);
		// glDepthFunc(GL_LESS);

		// shader.unbind();
	}
}


