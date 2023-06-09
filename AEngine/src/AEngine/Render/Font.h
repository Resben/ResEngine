#pragma once

#include <map>
#include "AEngine/Resource/Asset.h"
#include "VertexArray.h"
#include "Shader.h"

namespace AEngine
{

		/**
		 * @struct Character
		 * @brief Data to contain a single character of a font
		**/
	struct Character {
		unsigned int CharacterID;
		Math::ivec2 Size;
		Math::ivec2 GlypthOffset;
		unsigned int Stride;
	};

		/**
		 * @class Font
		 * @brief Stores a map of font data, renders a
		 * text to screen
		 * @author Ben Hawkins
		**/
	class Font : public Asset
	{
	public:
			/**
			 * @brief Constructor to create font
			 * @param[in] map<char, Character> of font data
			**/
		Font(const std::string& ident, const std::string& path);
			
		~Font();

        void Load(const std::string& path);

			/**
			 * @brief Renders a string on a screen (credit Learnopengl refer to header)
			 * @param[in] s : Shader to use for rendering 
			 * @param[in] test : String of text to render
			 * @param[in] pos : vec2 float position on screen (0,0 at bottom left)
			 * @param[in] scale : float scale of text
			 * @param[in] color : vec3 float colour of text
			 * @param[in] width : float width of screen
			 * @param[in] height : float height of screen
			 * @retval void
			**/
		void Render(Shader& s, std::string text, Math::vec2 pos, float scale, Math::vec3 color, Math::vec2 windowDimensions);

		static SharedPtr<Font> Create(const std::string& ident, const std::string& fname);

	private:
			/**
			 * @brief Generates the OpenGL data objects needed
			 * @retval void
			**/
		void GenerateFont();

		unsigned int m_vbo, m_vao;

			/// map of char to a font character
		std::map<char, Character> m_fontData;
	};
}