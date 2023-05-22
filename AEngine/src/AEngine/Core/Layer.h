/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once
#include <string>
#include "TimeStep.h"

namespace AEngine
{
		/**
		 * \class Layer
		 * \brief Base class for layers
		 * \details
		 * Layers are used to separate different parts of the application, for example
		 * the UI and the game world; Layers are managed by the LayerStack class.
		*/
	class Layer
	{
	public:
			/**
			 * \brief Constructor
			 * \param[in] identifier for the layer
			 * \note \p identifier is used  by the LayerStack class as a unique identifier
			 * \see LayerStack
			*/
		explicit Layer(const std::string& identifier);
			/**
			 * \brief Destructor
			*/
		virtual ~Layer() = default;

			/**
			 * \brief Contains logic that is run when layer is attached to layer stack
			*/
		virtual void OnAttach();
			/**
			 * \brief Contains logic that is run when layer is updated; usually every frame
			 * \param[in] frameTime this update
			*/
		virtual void OnUpdate(TimeStep frameTime);
			/**
			 * \brief Contains logic that is run when layer is detached from layer stack
			*/
		virtual void OnDetach();

			/**
			 * \brief Gets identifier for layer
			 * \return String identifier for layer
			*/
		const std::string& GetIdentifier();

	private:
			/**
			 * \brief String identifier for layer
			*/
		const std::string m_identifier;
	};
}
