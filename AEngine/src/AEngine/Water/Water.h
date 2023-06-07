#pragma once
#include "AEngine/Core/Types.h"
#include "AEngine/Render/Shader.h"
#include "AEngine/Render/Texture.h"
#include "AEngine/Render/VertexArray.h"

namespace AEngine
{
	/**
	 * @brief 
	 */
	class Water
	{
	public:
		Water();
		Water(SharedPtr<Texture> dudv, SharedPtr<Texture> normal);

			// todo
		void Render(const Math::mat4& transform, const Shader& shader, const Math::mat4& projectionView) const;

		void SetDUDV(SharedPtr<Texture> texture);
		void SetNormal(SharedPtr<Texture> texture);
		void SetModelMatrix(Math::mat4& model);

		Texture* GetDUDV();
		Texture* GetNormal();

	private:
		SharedPtr<VertexArray> m_vertexArray;
		SharedPtr<Texture> m_dudv;
		SharedPtr<Texture> m_normal;

			// todo
		float moveFactor;
	};
}
