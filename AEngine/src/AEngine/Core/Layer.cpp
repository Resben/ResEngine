#include "Layer.h"

namespace AEngine
{
	Layer::Layer(const std::string &ident)
		: m_ident(ident)
	{

	}

	void Layer::OnAttach()
	{

	}

	void Layer::OnDetach()
	{

	}

	void Layer::OnUpdate(TimeStep deltaTime)
	{

	}
    const std::string &Layer::Ident()
    {
		return m_ident;
    }
}
