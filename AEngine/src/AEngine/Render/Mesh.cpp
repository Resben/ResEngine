#include "Mesh.h"

#ifdef AE_RENDER_OPENGL
    #include "Platform/OpenGL/OpenGLMesh.h"
#endif

namespace AEngine
{
    std::shared_ptr<Mesh> AEngine::Mesh::Create(float* vertices, unsigned int nverts, unsigned int* indices, unsigned int nindices)
    {
#ifdef AE_RENDER_OPENGL
        return std::make_shared<OpenGLMesh>(vertices, nverts, indices, nindices);
#else
    #error "Graphics API not supported!"
#endif
    }
}

