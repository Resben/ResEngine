#include "Shader.h"

#ifdef AE_RENDER_OPENGL
    #include "Platform/OpenGL/OpenGLShader.h"
#endif

namespace AEngine
{
    std::shared_ptr<Shader> AEngine::Shader::Create(const std::string& fname)
    {
#ifdef AE_RENDER_OPENGL
        return std::make_shared<OpenGLShader>(fname);
#else
    #error "Graphics API not supported!"
#endif
    }
}

