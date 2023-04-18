#include "Layer.h"

namespace AEngine
{
	Layer::Layer(const std::string &name)
		: m_name(name)
	{

	}

	Layer::~Layer()
	{
		OnDetach();
	}

	void Layer::OnAttach()
	{

	}

	void Layer::OnDetach()
	{

	}

	void Layer::OnUpdate(TimeStep ts)
	{

	}
}
