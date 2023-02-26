/**
 * @file
 * @author Ben Hawkins (34112619)
 * @brief Loads a directory of .ttf font files and stores
 * then in a map of Font objects
 * 
 * Credit to Learnopengl for part of the implementation
 * of function LoadFont() from:
 * https://learnopengl.com/In-Practice/Text-Rendering
**/

#include <string>
#include <map>
#include <memory>
#include <AEngine/Core/Font.h>

namespace AEngine
{

		/**
		 * @class TextManager
		 * @brief Holds shared pointers to loaded Fonts
		 * @author Ben Hawkins
		**/
	class TextManager
	{
	public:
			/**
			 * @retval TextManager* Instance of TextManager
			**/
		static TextManager* Instance();

		~TextManager();

			/**
			 * @brief Loads all Fonts contained in directory
			 * @param[in] dir directory of Fonts
			 * @retval void
			**/
		void LoadFontDirectory(const std::string& dir);

			/**
			 * @brief Searches loaded Fonts for a given string
			 * @param[in] font name to search for (minus .ttf)
			 * @retval Font* if found
			 * @retval nullptr if not found
			**/
		Font* GetFont(const std::string& fontID);

		TextManager(const TextManager&) = delete;
		void operator=(const TextManager&) = delete;

	private:
		static TextManager* s_instance;
		TextManager() = default;

			/**
			 * @brief Loads a font from a path stores in map
			 * @param[in] font path
			 * @retval void
			**/
		void LoadFont(const std::string& fontPath);

			/// Map of filenames to Font objects
		std::map<std::string, std::shared_ptr<Font>> m_fonts;
	};
}