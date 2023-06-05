/**
 * \file
 * \author Christien Alden (34119981)
 * \brief Contains core definitions/alises for the Messaging system.
*/
#pragma once
#include "AEngine/Core/Types.h"
#include <sol/sol.hpp>
#include <functional>
#include <set>

namespace AEngine
{
	struct Message;

	using Agent = Uint16;
	using AgentSet = std::set<Agent>;
	using AgentCategory = Uint8;
	using AgentCategorySet = std::set<AgentCategory>;

	using MessageType = Uint8;
	using MessageTypeSet = std::set<MessageType>;
	using MessageData = sol::table;
	using MessageCallback = std::function<void(Message)>;

		/**
		 * \struct Message
		 * \brief Used in MessagingService to send messages between agents.
		*/
	struct Message
	{
		Agent sender;
		Agent receiver;
		MessageType type;
		MessageData payload;
	};
}
