#include "Texture.h"
#include "Texture.h"

#ifdef AE_RENDER_OPENGL
	#include "Platform/OpenGL/OpenGLTexture.h"	
#endif

namespace AEngine
{
	std::shared_ptr<Texture> AEngine::Texture::Create(const std::string& fname)
	{
#ifdef AE_RENDER_OPENGL
		return std::make_shared<OpenGLTexture>(fname);
#else
		#error "Graphics API not supported!"
#endif
	}
}
