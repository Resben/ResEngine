#pragma once

class Asset
{
public:
	Asset(const std::string& ident, const std::string& path)
		: m_ident(ident), m_path(path) {}

	const std::string& GetIdent() const { return m_ident; }
	const std::string& GetPath() const { return m_path; }

private:
	std::string m_ident;
	std::string m_path;
};
