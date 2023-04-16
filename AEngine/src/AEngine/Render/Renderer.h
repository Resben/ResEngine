/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Interface to setup and submit jobs to renderer
 * @todo Change Renderer::Submit to accept RenderableComponent
**/
#pragma once
#include "AEngine/Math/Math.hpp"
#include "Model.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"

namespace AEngine
{
		/**
		 * @class Renderer
		 * @brief Interface to submit rendering jobs to graphics context
		 * @author Christien Alden (34119981)
		**/
	class Renderer
	{
	public:
		struct Light {
			Math::vec3 pos;
			Math::vec3 colour;
		};

			/**
			 * @retval Renderer* Instance of Renderer
			**/
		static Renderer* Instance();
			/**
			 * @brief Set the projection/view for the rendering job
			 * @param[in] projectionMat to render from
			 * @retval void
			**/
		void SetProjection(const Math::mat4& projectionMat);
			/**
			 * @brief Submit an 'object' to be rendered
			 * @param[in] model to render
			 * @param[in] transform to apply
			 * @retval void
			**/
		void Submit(const Model& model, const Shader& shader, const Math::mat4& transform);

	private:
			/// Matrix to render scene from
		Math::mat4 m_projectionView{ 1.0f };
		struct Light m_light;

		
		Renderer() = default;
		static Renderer* s_instance;
	};
}
