#pragma once
#include "AEngine/Core/Types.h"
#include "AEngine/Resource/Asset.h"

namespace AEngine
{
	class Script : public Asset
	{
	public:
		Script(const std::string& ident, const std::string& fname);
		const std::string& GetData() const;
		static SharedPtr<Script> Create(const std::string& ident, const std::string& fname);

	private:
		std::string m_data;
	};
}