#include "WaterRenderer.h"

namespace AEngine
{
	WaterRenderer::WaterRenderer()
	{
	}

	WaterRenderer::~WaterRenderer()
	{
	}
	
	void WaterRenderer::Register(Water *water)
	{
	}
	
	void WaterRenderer::UnRegister(Water *water)
	{
	}
	
	void WaterRenderer::Render(WaterFrameBuffer *waterFrameBuffer)
	{
		//Lighting stuff - smoothing edges
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for(Water* water : waters)
		{
			Shader* shader = water->GetShader();
			//VertexArray* vArray = water->GetMesh()->GetVertexArray();
			//IndexBuffer* iBuffer = water->GetMesh()->GetIndexBuffer();

			shader->Bind();
			//vArray->Bind();
			//iBuffer->Bind();

			waterFrameBuffer->getReflectionTexture()->Bind();
			waterFrameBuffer->getRefractionTexture()->Bind();
			water->GetDUDV()->Bind();
			water->GetNormal()->Bind();
			//glActiveTexture(GL_TEXTURE0 + 4);
			//glBindTexture(GL_TEXTURE_2D, waterFrameBuffer->getRefractionDepthTexture());

			shader->SetUniformMat4("projection", camera->GetProjectionMatrix());
			shader->SetUniformMat4("view", camera->GetViewMatrix());
			shader->SetUniformMat4("model", water->GetModelMatrix());

			//glDrawElements(GL_TRIANGLES, iBuffer->GetCount(), GL_UNSIGNED_INT, 0);

			//glBindTexture(GL_TEXTURE_2D, 0);
			water->GetNormal()->Unbind();
			water->GetDUDV()->Unbind();
			waterFrameBuffer->getReflectionTexture()->Unbind();
			waterFrameBuffer->getRefractionTexture()->Unbind();


			//iBuffer->Unbind();
			//vArray->Unbind();
			shader->Unbind();
		}

		//glDisable(GL_BLEND);
	}
}