/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once
#include <list>
#include "Layer.h"
#include "Types.h"

namespace AEngine
{
		/**
		 * \class LayerStack
		 * \brief Provides a way to manage and iterate through layers
		 * \details
		 * The layers are stored as a list of \ref UniquePtr<\ref Layer> and can be iterated through using begin() and end().
		 * It is necessary to add the Layers in the order you wish to iterate them as there is
		 * currently no way to update the ordering of the internal data-structure.
		*/
	class LayerStack
	{
	public:
		using List = std::list<UniquePtr<Layer>>;

			/**
			 * \brief Constructor
			*/
		LayerStack() = default;
			/**
			 * \brief Destructor
			*/
		~LayerStack();

			/**
			 * \brief Push layer to stack, and call Layer::OnAttach for this layer
			 * \param[in] layer to push to stack
			 * \note \p layer will be moved and will be invalid after this function
			*/
		void PushLayer(UniquePtr<Layer> layer);
			/**
			 * \brief Remove layer by identifier, and call Layer::OnDetach for this layer
			 * \param[in] identifier of Layer to remove
			 * \retval true if layer was removed
			 * \retval false if layer was not found
			*/
		bool RemoveLayer(const std::string& identifier);
			/**
			 * \brief Clear all layers, and call Layer::OnDetach for each layer
			*/
		void Clear();

			/**
			 * \brief Get begin iterator
			 * \return begin iterator to a \ref List
			*/
		List::const_iterator begin();
			/**
			 * \brief Get end iterator
			 * \return end iterator to a \ref List
			*/
		List::const_iterator end();

	private:
			/**
			 * \brief A list containing the layers in the order they were pushed
			*/
		List m_layers;
	};
}
