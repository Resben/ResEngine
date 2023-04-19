#include "LayerStack.h"

namespace AEngine
{
	using stackList = std::list<Layer*>;

	LayerStack::~LayerStack()
	{
		Clear();
	}

	void LayerStack::Clear()
	{
		for (Layer* layer : m_layers)
		{
			layer->OnDetach();
		}
		m_layers.clear();
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_layers.push_back(layer);
		layer->OnAttach();
	}

	stackList::const_iterator LayerStack::begin()
	{
		return m_layers.begin();
	}

	stackList::const_iterator LayerStack::end()
	{
		return m_layers.end();
	}
}