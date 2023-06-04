/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once
#include "AEngine/Core/Types.h"
#include "Message.h"
#include <functional>
#include <set>

namespace AEngine
{
	class MessageServiceImpl;

	class MessageAgent
	{
	public:
		using Agent = Uint16;
		using AgentSet = std::set<Agent>;
		using AgentCategory = Uint8;
		using AgentCategorySet = std::set<AgentCategory>;

		using MessageType = Uint8;
		using MessageTypeSet = std::set<MessageType>;
		using MessageData = void*;
		using MessageCallback = std::function<void(Message)>;

	public:
		~MessageAgent();
		void AddToCategory(AgentCategory category);
		void RemoveFromCategory(AgentCategory category);

		void RegisterMessageHandler(MessageType type, MessageCallback callback);
		void UnregisterMessageHandler(MessageType type);

		void BroadcastMessage(MessageType type, MessageData payload);
		void SendMessageToAgent(Agent to, MessageType type, MessageData payload);
		void SendMessageToAgent(AgentSet to, MessageType type, MessageData payload);
		void SendMessageToCategory(AgentCategory to, MessageType type, MessageData payload);
		void SendMessageToCategory(AgentCategorySet to, MessageType type, MessageData payload);

		const AgentCategorySet GetRegisteredCategories() const;
		const MessageTypeSet GetRegisteredMessageTypes() const;

	private:
		MessageAgent(Agent agent, MessageServiceImpl& service);
		Agent m_identifier;
		MessageServiceImpl &m_service;

		friend class MessageServiceImpl;
	};
}
