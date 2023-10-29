#pragma once
#include "AEngine/Core/Types.h"
#include <string>

namespace AEngine
{
	class Predicate
	{
	public:
		enum class Operator
		{
			And,
			Or,
			Not
		};

		enum class TermType
		{
			Belief,
			Desire
		};

		struct Expr
		{
			virtual ~Expr(){}
		};

		struct Term : Expr
		{
			TermType type;
			std::string value;
		};

		struct BinaryExpression : Expr
		{
			Operator op;
			SharedPtr<Expr> leftOperand;
			SharedPtr<Expr> rightOperand;
		};

		struct UnaryExpression : Expr
		{
			Operator op;
			SharedPtr<Expr> operand;
		};

	public:
		Predicate() = default;
			/**
			 * \brief Construct a predicate from a string
			 * \param str The string to parse
			 * \throws std::runtime_error if the string is invalid
			*/
		Predicate(const std::string& str);
			/**
			 * \brief Construct a predicate from a string
			 * \returns The pointer to the expression tree
			*/
		const Expr* GetExprTree() const;
			/**
			 * \brief Get the original string
			 * \returns The original string
			*/
		const std::string& GetString() const;

			/**
			 * \brief Check if a string is a valid predicate
			 * \param str The string to check
			 * \returns True if the string is valid, false otherwise
			 * \todo Hook this function into the parser
			*/
		static bool IsValid(const std::string& str);


	private:
		SharedPtr<Expr> m_exprTree;   ///< The parsed expression tree
		std::string m_originalStr;          ///< The original string

			/**
			 * \brief Parse a string into an expression tree
			 * \param str The string to parse
			 * \returns The pointer to the expression tree
			 * \throws std::runtime_error if the string is invalid
			 * \todo Implement this function
			*/
		static SharedPtr<Expr> ParseStringIntoExpression(std::string& str);
		static SharedPtr<Term> ParseTerm(std::string& str);
		static SharedPtr<BinaryExpression> ParseBinaryExpression(std::string &str);
		static SharedPtr<UnaryExpression> ParseUnaryExpression(std::string &str);
		static void StripWhitespace(std::string &str);
	};
};
