/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once
#include "Types.h"

namespace AEngine
{
		/**
		 * \class Identifier
		 * \brief Generates unique identifiers
		 * \todo This will be improved to dynamically generate identifiers
		*/
	class Identifier
	{
	public:
			/**
			 * \brief Generates a unique identifier
			 * \return The generated unique identifier
			*/
		static Uint16 Generate();

	private:
			/**
			 * \brief The next identifier to be generated
			*/
		static Uint16 s_next;
	};
}
