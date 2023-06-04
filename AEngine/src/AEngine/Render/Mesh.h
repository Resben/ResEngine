/**
 * @file
 * @author Christien Alden (34119981)
**/
#pragma once
#include "AEngine/Core/Types.h"

namespace AEngine
{
		/**
		 * @class Mesh
		 * @brief Abstract mesh object
		**/
	class Mesh {
	public:
		virtual ~Mesh() = default;
			/**
			 * @brief Binds the Mesh to the rendering API
			 * @retval void
			**/
		virtual void Bind() const = 0;
			/**
			 * @brief Unbinds the Mesh to the rendenring API
			 * @retval void
			**/
		virtual void Unbind() const = 0;
			/**
			 * @brief Returns the total number of indices in index buffer
			**/
		virtual unsigned int GetIndexCount() const = 0;

			// Loading without bones
		static SharedPtr<Mesh> AEngine::Mesh::Create(float* vertices, unsigned int nverts, unsigned int* indices, unsigned int nindices);

			// Loading with bones
		static SharedPtr<Mesh> Create(
			float* vertices, unsigned int nverts, 
			unsigned int* indices, unsigned int nindices,
			int* boneIDs, float* boneWeights, int numBoneInfluence
			);
	};
}
