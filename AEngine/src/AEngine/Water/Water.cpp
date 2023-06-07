#include "Water.h"

namespace AEngine
{
	Water::Water()
	{
		//TODO
	}

	Water::~Water()
	{
		//TODO
	}

	void Water::SetMesh(Mesh *mesh)
	{
		this->mesh = mesh;
	}

	void Water::SetShader(Shader *shader)
	{
		this->shader = shader;
	}

	void Water::SetDUDV(Texture *texture)
	{
		this->dudv = texture;
	}

	void Water::SetNormal(Texture *texture)
	{
		this->normal = texture;
	}

	void Water::SetModelMatrix(Math::mat4 &model)
	{
		this->modelMatrix = model;
	}

	Mesh *Water::GetMesh()
	{
		return mesh;
	}

	Shader *Water::GetShader()
	{
		return shader;
	}

	Texture *Water::GetDUDV()
	{
		return dudv;
	}

	Texture *Water::GetNormal()
	{
		return normal;
	}

	Math::mat4 Water::GetModelMatrix()
	{
		return modelMatrix;
	}

	Mesh *Water::GenerateMesh(Math::vec2 size)
	{
		//TODO
		return nullptr;
	}
}

