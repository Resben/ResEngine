#include "ResourceAPI.h"

namespace AEngine
{
    ModelLoaderLibrary ResourceAPI::s_api = ModelLoaderLibrary::None;

    void ResourceAPI::Initialise(ModelLoaderLibrary loader) 
    { 
        s_api = loader; 
    }

    ModelLoaderLibrary ResourceAPI::GetLibrary() 
    { 
        return s_api; 
    }
}