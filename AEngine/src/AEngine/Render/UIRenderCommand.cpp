#include "UIRenderCommand.h"
#include "AEngine/Core/Logger.h"
#include "RenderCommand.h"

namespace AEngine
{
	static constexpr char* UI_Shader = R"(
        #type vertex
		#version 330 core

		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec2 aTexCoord;

		out vec2 TexCoord;

        uniform mat4 u_transform;

		void main()
		{
			gl_Position = u_transform * vec4(aPos, 1.0);
			TexCoord = aTexCoord;
		}

        #type fragment
		#version 330 core

		in vec2 TexCoord;

        out vec4 FragColor;

        uniform int hasTextures;
		uniform sampler2D u_texture;
        uniform vec4 u_color;

		void main()
		{
            if(hasTextures == 1)
			    FragColor = texture(u_texture, TexCoord);
            else
                FragColor = u_color;
		}
	)";

	SharedPtr<VertexArray> UIRenderCommand::s_quad = nullptr;
    SharedPtr<Shader> UIRenderCommand::s_shader = nullptr;

    void UIRenderCommand::Init()
    {
        if(s_quad && s_shader)
            return;

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

        s_quad = VertexArray::Create();

        SharedPtr<IndexBuffer> indexBuffer = IndexBuffer::Create();
        indexBuffer->SetData(quadIndices, static_cast<Uint32>(6), BufferUsage::StaticDraw);
		s_quad->SetIndexBuffer(indexBuffer);

		SharedPtr<VertexBuffer> positionAndTextureBuffer = VertexBuffer::Create();
		positionAndTextureBuffer->SetData(quadVertices, static_cast<Intptr_t>(20 * sizeof(float)), BufferUsage::StaticDraw);
		positionAndTextureBuffer->SetLayout({ { BufferElementType::Float3, false }, { BufferElementType::Float2, false } });
		s_quad->AddVertexBuffer(positionAndTextureBuffer);

        s_shader = Shader::Create(UI_Shader);
    }

    void UIRenderCommand::Render(const Math::mat4 transform, const SharedPtr<Texture> texture, const Math::vec4 color)
    {
        if (!s_quad || !s_shader)
            AE_LOG_FATAL("UIRenderCommand::Render -> VertexArray/Shader was never initiliased");

        Math::mat4 projectionTransform = Math::ortho(0.0f, 1.0f, 0.0f, 1.0f) * transform;

        if(texture)
        {
            s_shader->Bind();
            texture->Bind();
            s_quad->Bind();

            s_shader->SetUniformInteger("u_hasTextures", 1);
            s_shader->SetUniformInteger("u_texture", 0);
            s_shader->SetUniformFloat4("u_color", color);
            s_shader->SetUniformMat4("u_transform", projectionTransform);

            RenderCommand::DrawIndexed(Primitive::Triangles, s_quad->GetIndexBuffer()->GetCount(), 0);
            s_quad->Unbind();
            texture->Unbind();
            s_shader->Unbind();
        }
        else
        {
            s_shader->Bind();
            s_quad->Bind();

            s_shader->SetUniformInteger("u_hasTextures", 0);
            s_shader->SetUniformFloat4("u_color", color);
            s_shader->SetUniformMat4("u_transform", projectionTransform);

            RenderCommand::DrawIndexed(Primitive::Triangles, s_quad->GetIndexBuffer()->GetCount(), 0);
            s_quad->Unbind();
            s_shader->Unbind();
        }
    }
}