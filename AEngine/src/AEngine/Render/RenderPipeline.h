#pragma once
#include "Types.h"
#include "Framebuffer.h"
#include "AEngine/Math/Math.h"
#include "Shader.h"
#include "VertexArray.h"
#include <glm/glm.hpp>
#include <map>

namespace AEngine
{
    class RenderPipeline
    {
    public:
		static RenderPipeline& Instance();
        void OnWindowResize(const Math::uvec2 windowSize);
        void BindGeometryPass();
        void UnbindGeometryPass();
        void BindForwardPass();
        void UnbindForwardPass();
        void BindResultTexture();
        void LightingPass();
        void ClearBuffers();
        void TestRender();

        SharedPtr<Shader> GetTransparentShader();

    private:
        RenderPipeline();

        SharedPtr<VertexArray> m_screenQuad;
        SharedPtr<Framebuffer> m_geometryPass;
        SharedPtr<Shader> m_lightingShader;
        SharedPtr<Shader> m_transparentShader;
        SharedPtr<Shader> m_finalShader;
    };
}