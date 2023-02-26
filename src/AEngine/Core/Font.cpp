/**
 * @file
 * @author Ben Hawkins (34112619)
 * @brief Font implementation
 * Credit to Learnopengl refer to header file
**/
#include "AEngine/Core/Font.h"
#include "AEngine/Core/Logger.h"
#include <glm/gtc/type_ptr.hpp>
#include <glad.h>

namespace AEngine
{

	Font::Font(std::map<char, Character> fontData) :
		m_fontData(fontData)
	{
		GenerateFont();
	}

	Font::~Font()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		m_vao = m_vbo = 0;
	}

	void Font::GenerateFont()
	{
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

			// position
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
			// texture
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	void Font::RenderText(Shader& s, std::string text, glm::vec2 pos, float scale, glm::vec3 colour, float width, float height)
	{
		glDisable(GL_DEPTH_TEST);

		glm::mat4 projection = glm::ortho(0.0f, width, 0.0f, height);

		s.Bind();
		s.SetUniformMat4("u_projection", projection);
		s.SetUniformFloat3("fontColour", colour);

		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(m_vao);

		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = m_fontData[*c];

			float xpos = pos.x + ch.GlypthOffset.x * scale;
			float ypos = pos.y - (ch.Size.y - ch.GlypthOffset.y) * scale;

			float w = ch.Size.x * scale;
			float h = ch.Size.y * scale;

			float vertices[] = {
				// position				// texcoords
				xpos, ypos + h,			0.0f, 0.0f,
				xpos, ypos,				0.0f, 1.0f,
				xpos + w, ypos,			1.0f, 1.0f,

				xpos, ypos + h,			0.0f, 0.0f,
				xpos + w, ypos,			1.0f, 1.0f,
				xpos + w, ypos + h,		1.0f, 0.0f
			};

			glBindTexture(GL_TEXTURE_2D, ch.CharacterID);

			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			// position
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			// texture
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			pos.x += (ch.Stride >> 6) * scale;
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glEnable(GL_DEPTH_TEST);
	}
}