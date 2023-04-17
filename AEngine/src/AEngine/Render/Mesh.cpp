#include "Mesh.h"
#include "Platform/OpenGL/OpenGLMesh.h"
#include "AEngine/Core/Application.h"
#include "AEngine/Core/Logger.h"

namespace AEngine
{
    std::shared_ptr<Mesh> AEngine::Mesh::Create(float* vertices, unsigned int nverts, unsigned int* indices, unsigned int nindices)
    {
        switch (Application::Instance().Graphics().GetType())
        {
        case GraphicsType::OPENGL:
            return std::make_shared<OpenGLMesh>(vertices, nverts, indices, nindices);
        default:
            AE_LOG_FATAL("Mesh::Create::GraphicsType::Error -> None selected");
        }
    }
}

