#pragma once
#include <string>
#include "TimeStep.h"

namespace AEngine
{
	class Layer
	{
	public:
		explicit Layer(const std::string& ident);
		virtual ~Layer() = default;

			/**
			 * \brief Called when layer is attached to layer stack
			*/
		virtual void OnAttach();
			/**
			 * \brief Called when layer is detached from layer stack
			*/
		virtual void OnDetach();
			/**
			 * \brief Called every frame
			 * \param[in] deltaTime of current frame
			*/
		virtual void OnUpdate(TimeStep deltaTime);

			/**
			 * \brief Gets ident of layer
			 * \return Debug ident of later
			*/
		const std::string& Ident();

	private:
		std::string m_ident;
	};
}
