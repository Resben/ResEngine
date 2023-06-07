#pragma once

#include "AEngine/Render/Shader.h"
#include "AEngine/Render/Mesh.h"
#include "AEngine/Render/Texture.h"

namespace AEngine
{
	/**
	 * @brief 
	 */
	class Water
	{
	public:
		/**
		 * @brief 
		 */
		Water();

		~Water();

		void SetMesh(Mesh *mesh);

		void SetShader(Shader* shader);

		void SetDUDV(Texture* texture);

		void SetNormal(Texture* texture);

		void SetModelMatrix(Math::mat4& model);

		Mesh* GetMesh();
		Shader* GetShader();
		Texture* GetDUDV();
		Texture* GetNormal();
		Math::mat4 GetModelMatrix();

		Mesh* GenerateMesh(Math::vec2 size);

	private:
		
		Mesh* mesh;
		Shader* shader;
		Texture* dudv;
		Texture* normal;
		Math::mat4 modelMatrix;
		float moveFactor;
		
	};
}
