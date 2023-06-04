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
			/**
			 * \brief Add the agent to the given category.
			 * \param[in] category The category to add the agent to.
			*/
		void AddToCategory(AgentCategory category);
			/**
			 * \brief Remove the agent from the given category.
			 * \param[in] category The category to remove the agent from.
			*/
		void RemoveFromCategory(AgentCategory category);

			/**
			 * \brief Register a callback for the given message type.
			 * \param[in] type The message type to register the callback for.
			 * \param[in] callback The callback to register.
			*/
		void RegisterMessageHandler(MessageType type, MessageCallback callback);
			/**
			 * \brief Unregister the callback for the given message type.
			 * \param[in] type The message type to unregister the callback for.
			*/
		void UnregisterMessageHandler(MessageType type);

			/**
			 * \brief Broadcast a message to all agents.
			 * \param[in] type The type of the message.
			 * \param[in] payload The payload of the message.
			*/
		void BroadcastMessage(MessageType type, MessageData payload);
			/**
			 * \brief Send a message to the given agent.
			 * \param[in] to The agent to send the message to.
			 * \param[in] type The type of the message.
			 * \param[in] payload The payload of the message.
			*/
		void SendMessageToAgent(Agent to, MessageType type, MessageData payload);
			/**
			 * \brief Send a message to the given agents.
			 * \param[in] to The agents to send the message to.
			 * \param[in] type The type of the message.
			 * \param[in] payload The payload of the message.
			*/
		void SendMessageToAgent(AgentSet to, MessageType type, MessageData payload);
			/**
			 * \brief Send a message to the given category.
			 * \param[in] to The category to send the message to.
			 * \param[in] type The type of the message.
			 * \param[in] payload The payload of the message.
			*/
		void SendMessageToCategory(AgentCategory to, MessageType type, MessageData payload);
			/**
			 * \brief Send a message to the given categories.
			 * \param[in] to The categories to send the message to.
			 * \param[in] type The type of the message.
			 * \param[in] payload The payload of the message.
			*/
		void SendMessageToCategory(AgentCategorySet to, MessageType type, MessageData payload);

			/**
			 * \brief Get the set of categories the agent is in.
			 * \return The set of categories the agent is in.
			*/
		const AgentCategorySet GetRegisteredCategories() const;
			/**
			 * \brief Get the set of message types the agent is registered for.
			 * \return The set of message types the agent is registered for.
			*/
		const MessageTypeSet GetRegisteredMessageTypes() const;

	private:
			/**
			 * \param[in] agent The identifier of the new agent.
			 * \param[in] service The service the agent is registered with.
			*/
		MessageAgent(Agent agent, MessageServiceImpl& service);
			/**
			 * \brief The identifier of the agent.
			*/
		Agent m_identifier;
			/**
			 * \brief The service the agent is registered with.
			*/
		MessageServiceImpl &m_service;

		friend class MessageServiceImpl;
	};
}
