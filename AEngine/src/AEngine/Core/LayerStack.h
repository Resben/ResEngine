#pragma once
#include <list>
#include "Layer.h"

namespace AEngine
{
	class LayerStack
	{
	public:
		using stackList = std::list<Layer*>;
		LayerStack() = default;
		~LayerStack();

		void Clear();
		void PushLayer(Layer* layer);
		stackList::const_iterator begin();
		stackList::const_iterator end();

	private:
		stackList m_layers;
	};
}
