#include "Shader.h"
#include "AEngine/Core/Application.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "AEngine/Core/Logger.h"

namespace AEngine
{
    std::shared_ptr<Shader> AEngine::Shader::Create(const std::string& fname)
    {
        switch (Application::Instance().Graphics().GetType())
        {
        case GraphicsType::OPENGL:
            return std::make_shared<OpenGLShader>(fname);
        default:
            AE_LOG_FATAL("Texture::Create::Graphics type does not exist");
        }
    }
}

