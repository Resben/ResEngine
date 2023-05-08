#pragma once
#include <list>
#include <memory>
#include "Layer.h"

namespace AEngine
{
	class LayerStack
	{
	public:
		using stackList = std::list<std::unique_ptr<Layer>>;
		LayerStack() = default;
		~LayerStack();

		void Clear();
		void PushLayer(std::unique_ptr<Layer> layer);
		stackList::const_iterator begin();
		stackList::const_iterator end();

	private:
		stackList m_layers;
	};
}
