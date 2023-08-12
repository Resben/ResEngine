#pragma once
#include "AEngine/Render/Material.h"

#include <assimp/material.h>

namespace AEngine
{
    class AssimpMaterial : public Material
    {
    public:
        virtual ~AssimpMaterial();
        AssimpMaterial(const aiMaterial* material, const std::string& directory);
        AssimpMaterial(const std::string& ident, const std::string& parent, const SharedPtr<Material> material);
    private:
	    void LoadTextures(const aiMaterial* ai_material, const aiTextureType ai_type);
	    void GenerateMaterial(const aiMaterial* ai_mat);
        std::string m_directory;
    };
}