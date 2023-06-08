#pragma once
#include "AEngine/Core/Types.h"
#include "AEngine/Render/Shader.h"
#include "AEngine/Render/Texture.h"
#include "AEngine/Render/VertexArray.h"
#include "AEngine/Core/TimeStep.h"

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
		void Render(const Math::mat4& transform, const Shader& shader, const Math::mat4& projectionView, TimeStep dt) const;

		void SetDUDV(SharedPtr<Texture> texture);
		void SetNormal(SharedPtr<Texture> texture);

		Texture* GetDUDV();
		Texture* GetNormal();

		float GetMoveFactor(TimeStep) const;

	private:
		SharedPtr<VertexArray> m_vertexArray;
		SharedPtr<Texture> m_dudv;
		SharedPtr<Texture> m_normal;
	};
}
