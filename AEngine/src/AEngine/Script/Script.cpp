#include "Script.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace AEngine
{
	Script::Script(const std::string& ident, const std::string& fname)
		: Asset(ident, fname), m_data()
	{
		std::ifstream file(fname);
		if (file.is_open())
		{
			std::stringstream buffer;
			buffer << file.rdbuf();
			m_data = buffer.str();
		}
		else
		{
			throw std::invalid_argument("Failed to open file: " + fname);
		}
	}

	const std::string& Script::GetData() const
	{
		return m_data;
	}

	SharedPtr<Script> Script::Create(const std::string& ident, const std::string& fname)
	{
		return SharedPtr<Script>(new Script(ident, fname));
	}
}