#pragma once
#include <list>
#include "Layer.h"
#include "Types.h"

namespace AEngine
{
	class LayerStack
	{
	public:
		using stack = std::list<UniquePtr<Layer>>;
			/**
			 * \brief Construct a new Layer Stack object
			*/
		LayerStack() = default;
			/**
			 * \brief Destroy the Layer Stack object
			*/
		~LayerStack();

			/**
			 * \brief Clear all layers, and call OnDetach() for each layer
			*/
		void Clear();
			/**
			 * \brief Remove layer by ident, and call OnDetach() for this layer
			 * \param[in] ident Layer ident
			 * \retval true if layer was removed
			 * \retval false if layer was not found
			*/
		bool RemoveLayer(const std::string& ident);
			/**
			 * \brief Push layer to stack, and call OnAttach() for this layer
			 * \param[in] layer Layer
			 * \note Layer will be moved and will be invalid after this function
			*/
		void PushLayer(UniquePtr<Layer> layer);
			/**
			 * \brief Get begin iterator
			 * \return begin iterator
			*/
		stack::const_iterator begin();
			/**
			 * \brief Get end iterator
			 * \return end iterator
			*/
		stack::const_iterator end();

	private:
		stack m_layers;
	};
}
