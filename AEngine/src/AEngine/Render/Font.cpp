#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "AEngine/Math/Math.h"
#include "Font.h"
#include "AEngine/Core/Logger.h"

namespace AEngine
{

	SharedPtr<Font> Font::Create(const std::string& ident, const std::string& fname)
	{
		return MakeShared<Font>(ident, fname);
	}

	Font::Font(const std::string& ident, const std::string& path)
		: Asset(ident, path)
	{
		Load(path);
	}

    void Font::Load(const std::string& path)
    {
		FT_Library ft;

		if (FT_Init_FreeType(&ft))
		{
			AE_LOG_ERROR("TextManager::Load::Failed -> Could not init FreeType Library");
			exit(1);
		}

		FT_Face face;
		if (FT_New_Face(ft, path.c_str(), 0, &face)) {
			AE_LOG_ERROR("TextManager::Load::Failed -> {}", path);
			return;
		}

		FT_Set_Pixel_Sizes(face, 0, 48);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for (unsigned char c = 0; c < 128; c++)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				AE_LOG_WARN("TextManager::Load::Warning -> Failed to load a character");

			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			Character character = {
				texture,
				Math::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				Math::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			m_fontData.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);

		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		AE_LOG_TRACE("TextManager::Load::Success -> {}", fontPath);

		GenerateFont();
    }

	Font::~Font()
	{}

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

	void Font::Render(Shader& shader, std::string text, Math::vec2 pos, float scale, Math::vec3 colour, Math::vec2 windowDimensions)
	{        
        glDisable(GL_DEPTH_TEST);

		Math::mat4 projection = Math::ortho(0.0f, windowDimensions.x, 0.0f, windowDimensions.y);

		shader.Bind();
		shader.SetUniformMat4("u_projection", projection);
		shader.SetUniformFloat3("u_fontColour", colour);

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

        shader.Unbind();
	}
}