#pragma once

namespace AEngine
{
		/**
		 * @class Texture
		 * @brief Abstract texture object
		 * @author Ben Hawkins
		 * @author Christien Alden (34119981)
		**/
	class Texture {
	public:
			/**
			 * @param[in] imgData byte representation of the image to generate texture from
			 * @param[in] width of the image
			 * @param[in] height of the image
			**/
		Texture(unsigned char* imgData, int width, int height);
			/**
			 * @brief Deletes OpenGL data structures
			**/
		~Texture();
			/**
			 * @brief Binds the texture to the rendering API
			 * @return void
			**/
		void Bind() const;
			/**
			 * @brief Unbinds the texture from the rendering API
			 * @return void
			**/
		void Unbind() const;

	private:
			/// ID of the texture object
		unsigned int m_id;
			/**
			 * @brief Generates the texture object for the rendering API
			 * @param[in] imgData byte representation of the image to generate texture from
			 * @param[in] width of the image
			 * @param[in] height of the image
			 * @return void
			**/
		void Generate(unsigned char* imgData, int width, int height);
	};
}
