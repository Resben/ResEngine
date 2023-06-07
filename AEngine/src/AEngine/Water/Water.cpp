#include "Water.h"

namespace AEngine
{
	Water::Water()
	{

	}

	Water::~Water()
	{

	}

	void Water::Render(const Math::mat4 &projection, const Math::mat4 &view)
	{
	}

	void Water::SetMesh(Mesh *mesh)
	{
	}

	void Water::SetShader(Shader *shader)
	{
	}

	void Water::SetModelMatrix(Math::mat4 &model)
	{
	}

	Mesh *Water::GetMesh()
	{
		return nullptr;
	}

	Shader *Water::GetShader()
	{
		return nullptr;
	}

	Math::mat4 Water::GetModelMatrix()
	{
		return Math::mat4();
	}

	Mesh *Water::GenerateMesh(Math::vec2 size)
	{
		return nullptr;
	}
}

