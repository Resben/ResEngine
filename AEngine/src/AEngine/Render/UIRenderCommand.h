#pragma once

#include "VertexArray.h"
#include "Texture.h"
#include "AEngine/Core/Types.h"
#include "Shader.h"
#include "AEngine/Core/PerspectiveCamera.h"

namespace AEngine
{
        /**
		 * @class UIRenderCommand
		 * @brief Command for rendering panel UI
		 * @author Ben Hawkins
		**/
    class UIRenderCommand
    {
    public:
        /**
         * @brief Initialises the quad and shader
         * @retval void
        **/
        static void Init();
        static void Teardown();

        /**
         * @brief Renders a quad to screen with colour or texture
         * @param[in] billboard : Should UI be a billboard
         * @param[in] screenspace : true if camera screenspace false if worldspace
         * @param[in] camera : active camera
         * @param[in] transform : transform of panel
         * @param[in] texture : texture to render
         * @param[in] color : colour of panel
         * @retval void
        **/
        static void Render(bool billboard, bool screenspace, const PerspectiveCamera* camera, const Math::mat4 transform, const SharedPtr<Texture> texture, const Math::vec4 color);

    private:
        static SharedPtr<VertexArray> s_quad;
        static SharedPtr<Shader> s_shader;
    };
}