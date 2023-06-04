/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once
#include "MessageAgent.h"
#include "Message.h"
#include "AEngine/Core/Types.h"
#include "AEngine/Core/TimeStep.h"
#include <sol/sol.hpp>
#include <functional>
#include <map>
#include <queue>
#include <set>

namespace AEngine
{
	class MessageServiceImpl
	{
	public:
		using Agent = Uint16;
		using AgentSet = std::set<Agent>;
		using AgentCategory = Uint8;
		using AgentCategorySet = std::set<AgentCategory>;

		using MessageType = Uint8;
		using MessageTypeSet = std::set<MessageType>;
		using MessageData = sol::table;
		using MessageCallback = std::function<void(Message)>;

	public:
			/**
			 * \brief Get the singleton instance of the MessageServiceImpl.
			 * \return The singleton instance of the MessageServiceImpl.
			*/
		static MessageServiceImpl& Instance();

			/**
			 * \brief Create an agent with the given identifier.
			 * \param[in] identifier The identifier of the agent.
			 * \return The agent.
			 * \throw std::runtime_error If the agent already exists.
			*/
		MessageAgent CreateAgent(Agent identifier);
			/**
			 * \brief Destroy the given agent.
			 * \param[in] identifier The agent to destroy.
			 * \throw std::runtime_error If the agent does not exist.
			*/
		void DestroyAgent(Agent identifier);

			/**
			 * \brief Add the given agent to the given category.
			 * \param[in] agent The agent to add.
			 * \param[in] category The category to add the agent to.
			 * \throw std::runtime_error If the agent does not exist.
			*/
		void AddAgentToCategory(Agent agent, AgentCategory category);
			/**
			 * \brief Add the given agent to the given categories.
			 * \param[in] agent The agent to add.
			 * \param[in] categories The categories to add the agent to.
			*/
		void AddAgentToCategory(Agent agent, AgentCategorySet categories);

			/**
			 * \brief Remove the given agent from the given category.
			 * \param[in] agent The agent to remove.
			 * \param[in] category The category to remove the agent from.
			 * \throw std::runtime_error If the agent does not exist.
			*/
		void RemoveAgentFromCategory(Agent agent, AgentCategory category);
			/**
			 * \brief Remove the given agent from the given categories.
			 * \param[in] agent The agent to remove.
			 * \param[in] categories The categories to remove the agent from.
			 * \throw std::runtime_error If the agent does not exist.
			*/
		void RemoveAgentFromCategory(Agent agent, AgentCategorySet categories);

			/**
			 * \brief Register a message handler for the given agent and message type.
			 * \param[in] agent The agent to register the message handler for.
			 * \param[in] type The message type to register the message handler for.
			 * \param[in] callback The message handler to register.
			*/
		void RegisterMessageHandler(Agent agent, MessageType type, MessageCallback callback);
			/**
			 * \brief Unregister a message handler for the given agent and message type.
			 * \param[in] agent The agent to unregister the message handler for.
			*/
		void UnregisterMessageHandler(Agent agent, MessageType type);

			/**
			 * \brief Delivers all messages in the mailboxes of each agent.
			 * \details
			 * This will iterate through each agent and deliver all messages in their mailbox. \n
			 * If the agent has no messages in their mailbox, then nothing will happen. \n
			 * If the agent has messages in their mailbox, then the message will be delivered to the agent's message handler. \n
			 * If the agent has no message handler for the message type, then the message will be ignored.
			*/
		void DispatchMessages();

			/**
			 * \brief Send a message from the given agent to all agents.
			 * \param[in] from The agent sending the message.
			 * \param[in] type The type of the message.
			 * \param[in] data The data of the message.
			 * \throw std::runtime_error If the sender does not exist.
			*/
		void SendMessageToAllAgents(Agent from, MessageType type, MessageData data);
			/**
			 * \brief Send a message from the given agent to the given agent.
			 * \param[in] from The agent sending the message.
			 * \param[in] to The agent receiving the message.
			 * \param[in] type The type of the message.
			 * \param[in] data The data of the message.
			 * \throw std::runtime_error If either sender does not exist.
			 * \note If the receiver does not exist, then the message will be ignored.
			*/
		void SendMessageToAgent(Agent from, Agent to, MessageType type, MessageData data);
			/**
			 * \brief Send a message from the given agent to the given agents.
			 * \param[in] from The agent sending the message.
			 * \param[in] to The agents receiving the message.
			 * \param[in] type The type of the message.
			 * \param[in] data The data of the message.
			*/
		void SendMessageToAgent(Agent from, const AgentSet &to, MessageType type, MessageData data);

			/**
			 * \brief Send a message from the given agent to all agents in the given category.
			 * \param[in] from The agent sending the message.
			 * \param[in] to The category receiving the message.
			 * \param[in] type The type of the message.
			 * \param[in] data The data of the message.
			*/
		void SendMessageToCategory(Agent from, AgentCategory to, MessageType type, const MessageData data);
			/**
			 * \brief Send a message from the given agent to all agents in the given categories.
			 * \param[in] from The agent sending the message.
			 * \param[in] to The categories receiving the message.
			 * \param[in] type The type of the message.
			 * \param[in] data The data of the message.
			*/
		void SendMessageToCategory(Agent from, const AgentCategorySet &to, MessageType type, const MessageData data);

		const AgentCategorySet GetRegisteredCategories(Agent agent) const;
		const MessageTypeSet GetRegisteredMessageTypes(Agent agent) const;

	private:
		using AgentMessageHandlers = std::map<MessageType, MessageCallback>;
		using MessageQueue = std::queue<Message>;

	private:
		MessageServiceImpl() = default;
			/**
			 * \brief Holds the message handlers for each agent.
			*/
		std::map<Agent, AgentMessageHandlers> m_messageHandlers;
			/**
			 * \brief Holds the mailboxes for each agent.
			*/
		std::map<Agent, MessageQueue> m_mailboxes;
			/**
			 * \brief Holds the categories that each agent is registered to.
			*/
		std::map<AgentCategory, AgentSet> m_categories;
			/**
			 * \brief Holds the agents that are currently registered
			*/
		AgentSet m_agents;

			/**
			 * \brief Add a message to the mailbox of the given agent.
			 * \param[in] agent The agent to add the message to.
			 * \param[in] message The message to add.
			 * \note If the agent does not exist, then the message will be dropped.
			*/
		void AddToMailbox(Agent agent, Message &message);
			/**
			 * \brief Checks if the given agent exists.
			 * \param[in] agent The agent to check.
			 * \retval True The agent exists.
			 * \retval False The agent does not exist.
			*/
		bool AgentExists(Agent agent) const;
	};
}
