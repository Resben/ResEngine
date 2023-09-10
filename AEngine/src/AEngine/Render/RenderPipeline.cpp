#include "RenderPipeline.h"
#include "AEngine/Core/Application.h"
#include "AEngine/Core/Window.h"
#include "RenderCommand.h"


#include "Texture.h"
#include "AEngine/Resource/AssetManager.h"

namespace
{
	static constexpr char* lightingCode = R"(
        #type vertex
		#version 330 core

		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec2 aTexCoord;

		out vec2 TexCoord;

		void main()
		{
			gl_Position = vec4(aPos, 1.0);
			TexCoord = aTexCoord;
		}

        #type fragment
		#version 330 core

		in vec2 TexCoord;

		out vec4 FragColor;

		uniform sampler2D positionTexture;
		uniform sampler2D normalTexture;
		uniform sampler2D albedoTexture;

		void main()
		{
			vec3 FragPos = texture(positionTexture, TexCoord).xyz;
			vec3 Normal = texture(normalTexture, TexCoord).xyz;
			vec4 Albedo = texture(albedoTexture, TexCoord);

			vec3 LightPos = vec3(10.0, 10.0, 10.0);
			vec3 LightColor = vec3(1.0, 1.0, 1.0);
			
			vec3 LightDir = normalize(LightPos - FragPos);
			float diff = max(dot(Normal, LightDir), 0.35);
			
			vec3 result = Albedo.rgb * diff * LightColor;

			FragColor = vec4(result, 1.0);
		}
	)";

    static constexpr char* transparentCode = R"(
        #type vertex
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTexCoord;
        layout (location = 2) in vec3 aNormal;

        out vec3 FragPos;
        out vec3 Normal;

        uniform mat4 u_projectionView;
        uniform mat4 u_transform;

        void main()
        {
            gl_Position = u_projectionView * u_transform * vec4(aPos, 1.0);
            FragPos = vec3(u_transform * vec4(aPos, 1.0));
            Normal = aNormal;
        }

        #type fragment
        #version 330 core

        uniform vec4 u_baseColor;
        uniform samplerCube u_hdr;
        uniform vec3 u_camPos;

        in vec3 FragPos;
        in vec3 Normal;

        out vec4 FragColor;

        void main()
        {
            vec3 I = normalize(FragPos - u_camPos);
            vec3 R = reflect(I, normalize(Normal));

            vec3 hdr = texture(u_hdr, R).rgb;

                // Last is reflection add it to the uniform later
            FragColor = mix(u_baseColor, vec4(hdr, 1.0), 0.5);
        }
    )";

    float quadVertices[] = {
    // Positions          // Texture Coords
    -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  0.0f,  1.0f, 1.0f
    };

    unsigned int quadIndices[] = {
    0, 1, 2,
    0, 2, 3
    };
}

namespace AEngine
{
    RenderPipeline::RenderPipeline()
    {
        m_screenQuad = VertexArray::Create();

        SharedPtr<IndexBuffer> indexBuffer = IndexBuffer::Create();
        indexBuffer->SetData(quadIndices, static_cast<Uint32>(6), BufferUsage::StaticDraw);
		m_screenQuad->SetIndexBuffer(indexBuffer);

		SharedPtr<VertexBuffer> positionAndTextureBuffer = VertexBuffer::Create();
		positionAndTextureBuffer->SetData(quadVertices, static_cast<Intptr_t>(20 * sizeof(float)), BufferUsage::StaticDraw);
		positionAndTextureBuffer->SetLayout({ { BufferElementType::Float3, false }, { BufferElementType::Float2, false } });
		m_screenQuad->AddVertexBuffer(positionAndTextureBuffer);

        m_geometryPass = Framebuffer::Create(Application::Instance().GetWindow()->GetSize());
        m_lightingShader = Shader::Create(lightingCode);
        m_transparentShader = Shader::Create(transparentCode);
        m_geometryPass->Attach(FramebufferAttachment::Color, static_cast<unsigned int>(RenderPipelineTarget::Positon));
        m_geometryPass->Attach(FramebufferAttachment::Color, static_cast<unsigned int>(RenderPipelineTarget::Normal));
        m_geometryPass->Attach(FramebufferAttachment::Color, static_cast<unsigned int>(RenderPipelineTarget::Diffuse));
        m_geometryPass->Attach(FramebufferAttachment::Depth);
    }

	RenderPipeline& RenderPipeline::Instance()
	{
		static RenderPipeline instance;
		return instance;
	}

    SharedPtr<Shader> RenderPipeline::GetTransparentShader()
    {
        return m_transparentShader;
    }

        // Readability?
    void RenderPipeline::SetTargets(const std::vector<RenderPipelineTarget>& targets)
    {
        m_targets.clear();
        for(RenderPipelineTarget target : targets)
        {
            m_targets.push_back(static_cast<unsigned int>(target));
        }

        m_geometryPass->SetActiveDrawBuffers(m_targets);
    }

    void RenderPipeline::OnWindowResize(const Math::uvec2 windowSize)
    {
        m_geometryPass->ResizeBuffers(windowSize);
    }

    void RenderPipeline::BindGeometryPass()
    {
        m_geometryPass->Bind();
        RenderCommand::Clear();
    }

    void RenderPipeline::UnbindGeometryPass()
    {
        m_geometryPass->Unbind();
    }

    void RenderPipeline::LightingPass()
    {
        RenderCommand::EnableDepthTest(false);
        m_lightingShader->Bind();
            // Hard coding for now
        m_geometryPass->BindBuffers({0, 1, 2});

        m_lightingShader->SetUniformInteger("positionTexture", 0);
        m_lightingShader->SetUniformInteger("normalTexture", 1);
        m_lightingShader->SetUniformInteger("albedoTexture", 2);

        m_screenQuad->Bind();
        RenderCommand::DrawIndexed(Primitive::Triangles, m_screenQuad->GetIndexBuffer()->GetCount(), 0);

        m_screenQuad->Unbind();
        m_geometryPass->UnbindBuffers();
        m_lightingShader->Unbind();
        RenderCommand::EnableDepthTest(true);

            // Transferring the geometry depth buffer
            // To the default screenbuffer
        m_geometryPass->TransferDepthBuffer(0);
    }
}