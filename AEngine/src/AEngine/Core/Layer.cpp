/**
 * \file
 * \author Christien Alden (34119981)
*/
#include "Layer.h"

namespace AEngine
{
	Layer::Layer(const std::string &ident)
		: m_identifier(ident)
	{

	}

	void Layer::OnAttach()
	{

	}

	void Layer::OnUpdate(TimeStep frameTime)
	{

	}

	void Layer::OnDetach()
	{

	}

	const std::string& Layer::GetIdentifier()
	{
		return m_identifier;
	}
}
