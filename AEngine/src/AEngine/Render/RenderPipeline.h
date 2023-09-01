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
        RenderPipeline();
        void SetTargets(const std::vector<RenderPipelineTarget>& targets);
        void OnWindowResize(const Math::uvec2 windowSize);
        void BindGeometryPass();
        void UnbindGeometryPass();
        void LightingPass();

    private:
        std::vector<unsigned int> m_targets;
        SharedPtr<VertexArray> m_screenQuad;
        SharedPtr<Framebuffer> m_geometryPass;
        SharedPtr<Shader> m_lightingShader;
    };
}