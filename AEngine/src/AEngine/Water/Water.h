#pragma once

#include "AEngine/Render/Shader.h"
#include "AEngine/Render/Mesh.h"

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

		void Render(const Math::mat4& projection, const Math::mat4& view);

		void SetMesh(Mesh *mesh);

		void SetShader(Shader* shader);

		void SetModelMatrix(Math::mat4& model);

		Mesh* GetMesh();
		Shader* GetShader();
		Math::mat4 GetModelMatrix();

		Mesh* GenerateMesh(Math::vec2 size);

	private:
		
		Mesh* mesh;
		Shader* shader;
		Math::mat4 model;
		
		//TODO if i get to it
		//Texture* dudv;
		//Texture* normal;
		
	};
}
