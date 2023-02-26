/**
 * @file
 * @author Ben Hawkins (34112619)
 * @brief Font implementation
 * Credit to Learnopengl refer to header file
**/
#include <AEngine/Core/Logger.h>
#include <AEngine/Core/TextManager.h>
#include <glad.h>
#include <filesystem>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace fs = std::filesystem;

namespace AEngine
{

	TextManager* TextManager::s_instance = nullptr;

	TextManager* TextManager::Instance()
	{
		if (!s_instance)
		{
			s_instance = new TextManager;
		}

		return s_instance;
	}

	TextManager::~TextManager()
	{
		m_fonts.clear();
	}

	void TextManager::LoadFontDirectory(const std::string& dir)
	{
		for (const auto& entry : fs::directory_iterator(dir + '/'))
		{
			if (is_regular_file(entry))
			{
				LoadFont(fs::path(entry).string());
			}
		}
	}

		// Credit to learnopengl see header
	void TextManager::LoadFont(const std::string& fontPath)
	{
		std::map<char, Character> textToPrint;

		FT_Library ft;

		if (FT_Init_FreeType(&ft))
		{
			AE_LOG_ERROR("TextManager::Load::Failed -> Could not init FreeType Library");
			exit(1);
		}

		FT_Face face;
		if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
			AE_LOG_ERROR("TextManager::Load::Failed -> {}", fontPath);
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
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			textToPrint.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);

		FT_Done_Face(face);
		FT_Done_FreeType(ft);

			// Get an ID from file path
		size_t index = fontPath.find_last_of("/");
		std::string id = fontPath.substr(index + 1);
		index = id.find_last_of(".");
		id = id.substr(0, index);

		AE_LOG_TRACE("TextManager::Load::Success -> {}", fontPath);

		m_fonts.emplace(std::make_pair(id, std::make_shared<Font>(textToPrint)));
	}

	Font* TextManager::GetFont(const std::string& fontID)
	{
		std::map<std::string, std::shared_ptr<Font>>::iterator it;
		it = m_fonts.find(fontID);
		if (it != m_fonts.end())
			return it->second.get();
		else
			return nullptr;
	}
}