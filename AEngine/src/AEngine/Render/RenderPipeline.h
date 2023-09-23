#pragma once
#include "Types.h"
#include "Framebuffer.h"
#include "AEngine/Math/Math.h"
#include "Shader.h"
#include "VertexArray.h"
#include <glm/glm.hpp>
#include <map>

    /**
     * \class Renderpipeline
     * \brief Encapsulates all renderpipeline functions/shader (simple implementation)
     * \author Ben Hawkins (34112619)
    **/
namespace AEngine
{
    class RenderPipeline
    {
    public:
		static RenderPipeline& Instance();

			/**
			 * \brief Update the buffer sizes to match a new window size
			 * \param[in] windowSize the new window size
			 * \retval void
			**/
        void OnWindowResize(const Math::uvec2 windowSize);
			/**
			 * \brief Bind the geometry buffers call all geometry render functions
			 * \retval void
			**/
        void BindGeometryPass();
			/**
			 * \brief Bind the result buffer for forward rendering
			 * \retval void
			**/
        void BindForwardPass();
            /**
			 * \brief Bind the result texture
			 * \retval void
			**/
        void BindResultTexture();
        	/**
			 * \brief Unbind the framebuffer
			 * \retval void
			**/
        void Unbind();
        	/**
			 * \brief Calculate lighting
			 * \retval void
             * \note This will render to the result buffer so forwardpass must be bound
			**/
        void LightingPass();
        	/**
			 * \brief Clear the buffers for the next renderpass
			 * \retval void
			**/
        void ClearBuffers();

            /// @todo REMOVE
        void TestRender();

			/**
			 * \brief Store the transparent shader here so we can access it elsewhere
			 * \retval void
             * \todo Move the shader to the config settings when setup
			**/
        SharedPtr<Shader> GetTransparentShader();

    private:
        RenderPipeline();

        SharedPtr<VertexArray> m_screenQuad;
        SharedPtr<Framebuffer> m_gbuffer;
        SharedPtr<Shader> m_lightingShader;
        SharedPtr<Shader> m_transparentShader;
        SharedPtr<Shader> m_finalShader;
    };
}