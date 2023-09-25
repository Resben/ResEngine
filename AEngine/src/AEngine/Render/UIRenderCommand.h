#pragma once

#include "VertexArray.h"
#include "Texture.h"
#include "AEngine/Core/Types.h"
#include "Shader.h"

namespace AEngine
{
    class UIRenderCommand
    {
    public:
        static void Init();
        static void Render(const Math::mat4 transform, const SharedPtr<Texture> texture, const Math::vec4 color);

    private:
        static SharedPtr<VertexArray> s_quad;
        static SharedPtr<Shader> s_shader;
    };
}