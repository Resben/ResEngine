/**
 * @file
 * @author Ben Hawkins (34112619)
 * @brief Contains a map of font character data + renders
 * a string using the font data.
 * 
 * Credit to Learnopengl for part of the implementation
 * of function RenderText() from:
 * https://learnopengl.com/In-Practice/Text-Rendering
**/

#include <AEngine/Render/Shader.h>
#include <map>

namespace AEngine
{

		/**
		 * @struct Character
		 * @brief Data to contain a single character of a font
		**/
	struct Character {
		unsigned int CharacterID;
		glm::ivec2 Size;
		glm::ivec2 GlypthOffset;
		unsigned int Stride;
	};

		/**
		 * @class Font
		 * @brief Stores a map of font data, renders a
		 * text to screen
		 * @author Ben Hawkins
		**/
	class Font
	{
	public:
			/**
			 * @brief Constructor to create font
			 * @param[in] map<char, Character> of font data
			**/
		Font(std::map<char, Character> fontData);
			
		~Font();

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
		void RenderText(Shader& s, std::string text, glm::vec2 pos, float scale, glm::vec3 color, float width, float height);

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