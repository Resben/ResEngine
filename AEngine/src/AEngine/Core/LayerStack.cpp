#include "LayerStack.h"

namespace AEngine
{
	using stackList = std::list<std::unique_ptr<Layer>>;

	LayerStack::~LayerStack()
	{
		Clear();
	}

	void LayerStack::Clear()
	{
		for (auto it = m_layers.begin(); it != m_layers.end(); ++it)
		{
			(*it)->OnDetach();
		}
		m_layers.clear();
	}

	void LayerStack::PushLayer(std::unique_ptr<Layer> layer)
	{
		m_layers.push_back(std::move(layer));
		m_layers.back()->OnAttach();
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