#include "Mesh.h"
#include "AEngine/Core/Application.h"
#include "AEngine/Core/Logger.h"
#include "Platform/OpenGL/OpenGLMesh.h"

namespace AEngine
{
    SharedPtr<Mesh> AEngine::Mesh::Create(float* vertices, unsigned int nverts, unsigned int* indices, unsigned int nindices)
    {
        switch (Application::Instance().Graphics().GetLibrary())
        {
        case GraphicsLibrary::OpenGL:
            return MakeShared<OpenGLMesh>(vertices, nverts, indices, nindices);
        default:
            AE_LOG_FATAL("Mesh::Create::GraphicsLibrary::Error -> None selected");
        }
    }
}

