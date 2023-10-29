#include "Predicate.h"
#include <regex>
#include <stdexcept>
#include <string>

namespace {
	constexpr char* g_termRegex = "^[a-z0-9_]+$";
}

namespace AEngine
{
	Predicate::Predicate(const std::string& str)
	{
		// remove whitespace
		m_originalStr = str;
		StripWhitespace(m_originalStr);

		// parse string into expression tree
		// create a copy of the stripped string
		std::string copy = m_originalStr;
		m_exprTree = ParseStringIntoExpression(copy);
		if (!m_exprTree)
		{
			throw std::runtime_error{ "Invalid predicate: " + str };
		}
	}

	const Predicate::Expr* Predicate::GetExprTree() const
	{
		return m_exprTree.get();
	}

	const std::string& Predicate::GetString() const
	{
		return m_originalStr;
	}

	SharedPtr<Predicate::Expr> Predicate::ParseStringIntoExpression(std::string& str)
	{
		// check if the string is empty
		if (str.empty())
		{
			throw std::runtime_error{ "Empty expression: " + str };
		}

		// check is the string starts with a term (base case)
		if (str.compare(0, 6, "BELIEF") == 0 || str.compare(0, 6, "DESIRE") == 0)
		{
			// parse term
			return ParseTerm(str);
		}
		else if (str.compare(0, 3, "AND") == 0 || str.compare(0, 2, "OR") == 0)
		{
			// parse binary expression
			return ParseBinaryExpression(str);
		}
		else if(str.compare(0, 3, "NOT") == 0)
		{
			// parse unary expression
			return ParseUnaryExpression(str);
		}
		else
		{
			throw std::runtime_error{ "Invalid expression: " + str };
		}
	}


	SharedPtr<Predicate::Term> Predicate::ParseTerm(std::string& str)
	{
		Term term;
		size_t typeLength = 0;

		if (str.compare(0, 6, "BELIEF") == 0)
		{
			term.type = TermType::Belief;
			typeLength = 6;
		}
		else if (str.compare(0, 6, "DESIRE") == 0)
		{
			term.type = TermType::Desire;
			typeLength = 6;
		}
		else
		{
			throw std::runtime_error{ "Expected BELIEF or DESIRE: " + str };
		}

		// check for '(' which should be the typeLength'th character
		size_t openParen = str.find_first_of('(');
		if (openParen != typeLength)
		{
			throw std::runtime_error{ "Expected '(': " + str };
		}

		// find_first_of index of matching ')'
		// we can look for the npos because we know that a desire can not
		// contain another expression
		size_t closeParen = str.find_first_of(')');
		if (closeParen == std::string::npos)
		{
			throw std::runtime_error{ "Expected ')': " + str };
		}

		// calculate the length of the term
		size_t numChars = closeParen - openParen - 1;

		// get value and set remove the term from the string
		term.value = str.substr(openParen + 1, numChars);
		std::regex pattern(g_termRegex);
		if (!std::regex_search(term.value, pattern))
		{
			throw std::runtime_error{ "Invalid <value>: " + str };
		}

		// remove the term from the string
		str = str.substr(closeParen + 1);
		return std::make_shared<Term>(term);

	}

	SharedPtr<Predicate::UnaryExpression> Predicate::ParseUnaryExpression(std::string& str)
	{
		// expected format: <op>(<expr>)
		// where <op> is one of: NOT
		// and <expr> is another expression

		UnaryExpression expr;
		size_t operatorLength = 0;

		// get the operator
		if (str.compare(0, 3, "NOT") == 0)
		{
			expr.op = Operator::Not;
			operatorLength = 3;
		}
		else
		{
			throw std::runtime_error{ "Expected NOT: " + str };
		}

		// check for '(' which should be the operatorLength'th character
		size_t index = str.find_first_of('(');
		if (index != operatorLength)
		{
			throw std::runtime_error{ "Expected '(': " + str };
		}

		// break the string from the first '(' to the end
		std::string subStr = str.substr(index + 1);
		expr.operand = ParseStringIntoExpression(subStr);

		// check for ')', this in the updated substr
		// this should be the first character
		if (subStr[0] != ')')
		{
			throw std::runtime_error{ "Expected ')': " + str };
		}

		// remove the expression from the string
		str = subStr.substr(1);
		return std::make_shared<UnaryExpression>(expr);
	}

	SharedPtr<Predicate::BinaryExpression> Predicate::ParseBinaryExpression(std::string& str)
	{
		// expetced format: <op>(<expr>, <expr>)
		// where <op> is one of: AND, OR
		// and <expr> is another expression

		BinaryExpression expr;
		size_t operatorLength = 0;

		// get the operator
		if (str.compare(0, 3, "AND") == 0)
		{
			expr.op = Operator::And;
			operatorLength = 3;
		}
		else if (str.compare(0, 2, "OR") == 0)
		{
			expr.op = Operator::Or;
			operatorLength = 2;
		}
		else
		{
			throw std::runtime_error{ "Expected AND or OR: " + str };
		}

		// check for '(' which should be the operatorLength'th character
		size_t index = str.find_first_of('(');
		if (index != operatorLength)
		{
			throw std::runtime_error{ "Expected '(': " + str };
		}

		// break the string from the first '(' to the end
		std::string subStr = str.substr(index + 1);
		expr.leftOperand = ParseStringIntoExpression(subStr);

		// check for ',' which should be the first character
		if (subStr[0] != ',')
		{
			throw std::runtime_error{ "Expected ',': " + str };
		}

		// break the string from the first ',' to the end
		subStr = subStr.substr(1);
		expr.rightOperand = ParseStringIntoExpression(subStr);

		// check for ')', this in the updated substr
		// this should be the first character
		if (subStr[0] != ')')
		{
			throw std::runtime_error{ "Expected ')': " + str };
		}

		// remove the expression from the string
		str = subStr.substr(1);
		return std::make_shared<BinaryExpression>(expr);
	}


	bool Predicate::IsValid(const std::string& str)
	{
		try
		{
			std::string copy = str;
			StripWhitespace(copy);
			ParseStringIntoExpression(copy);
			return true;
		} catch (const std::runtime_error& e)
		{
			return false;
		}
	}

	void Predicate::StripWhitespace(std::string& str)
	{
		str.erase(
			std::remove_if(
				str.begin(),
				str.end(),
				[](char c) { return std::isspace(c); }
			),
			str.end()
		);
	}
}
