#pragma once
#include <string>
#include "TimeStep.h"

namespace AEngine
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Default");
		virtual ~Layer() = default;

		virtual void onAttach();
		virtual void onDetach();
		virtual void onUpdate(TimeStep ts);

	private:
		std::string m_name;
	};
}
