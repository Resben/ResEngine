#pragma once

#include <map>
#include "AEngine/Resource/Asset.h"
#include "VertexArray.h"
#include "Shader.h"
#include "AEngine/Core/PerspectiveCamera.h"

namespace AEngine
{

		/**
		 * @struct Character
		 * @brief Data to contain a single character of a font
		**/
	struct Character {
		unsigned int CharacterID;
		Math::vec2 Size;
		Math::vec2 GlypthOffset;
		float Stride;
	};

		/**
		 * @class Font
		 * @brief Stores a map of font data, renders a text to screen
		 * @author Ben Hawkins
		**/
	class Font : public Asset
	{
	public:
			/**
			 * @brief Constructor to create font
			 * @param[in] ident : identifier of font
			 * @param[in] path : path to font file
			**/
		Font(const std::string& ident, const std::string& path);
			
		~Font();

			/**
			 * @brief Loads a font from a file
			 * @param[in] path : path to font file
			 * @retval void
			**/
		void Load(const std::string& path);

			/**
			 * @brief Renders a string on a screen (credit Learnopengl refer to header)
			 * @param[in] billboard : Should UI be a billboard 
			 * @param[in] screenspace : true if camera screenspace false if worldspace
			 * @param[in] camera : active camera
			 * @param[in] text : text to render
			 * @param[in] transform : transform of text
			 * @param[in] colour : colour of text
			 * @retval void
			**/
		void Render(bool billboard, bool screenspace, const PerspectiveCamera* camera, std::string text, Math::mat4 transform, Math::vec4 colour);

		static SharedPtr<Font> Create(const std::string& ident, const std::string& fname);

	private:
			/**
			 * @brief Generates the OpenGL data objects needed
			 * @retval void
			**/
		void GenerateFont();

		unsigned int m_vbo, m_vao;
		std::map<char, Character> m_fontData;
		static SharedPtr<Shader> s_textShader;
	};
}