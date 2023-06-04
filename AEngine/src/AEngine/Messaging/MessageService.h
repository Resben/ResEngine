/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once
#include "AEngine/Core/Types.h"

namespace AEngine
{
	class MessageAgent;

		/**
		 * \class MessageService
		 * \brief A static class that provides a way to create MessageAgents.
		*/
	class MessageService
	{
	public:
			/**
			 * \brief Creates a MessageAgent with the given identifier.
			 * \param[in] identifier The identifier of the MessageAgent.
			 * \return The created MessageAgent.
			 * \throw std::runtime_error If the identifier is already in use.
			*/
		static MessageAgent CreateAgent(Uint16 identifier);
	};
}
