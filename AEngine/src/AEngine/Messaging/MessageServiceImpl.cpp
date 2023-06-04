/**
 * \file
 * \author Christien Alden (34119981)
*/
#include "MessageServiceImpl.h"
#include "AEngine/Core/Logger.h"
#include <stdexcept>

namespace AEngine
{
	MessageServiceImpl &MessageServiceImpl::Instance()
	{
		static MessageServiceImpl instance;
		return instance;
	}

//--------------------------------------------------------------------------------
// Agent Creation and Modification
//--------------------------------------------------------------------------------
	MessageAgent MessageServiceImpl::CreateAgent(Agent identifier)
	{
		if (!AgentExists(identifier))
		{
			throw std::runtime_error("Agent already exists");
		}

		m_agents.insert(identifier);
		return MessageAgent(identifier, *this);
	}

	void MessageServiceImpl::DestroyAgent(Agent identifier)
	{
		// remove agent from all groups
		for (auto &category : m_categories)
		{
			category.second.erase(identifier);
		}

		// remove agent's message handlers
		m_messageHandlers.erase(identifier);

		// remove agent's mailbox
		m_mailboxes.erase(identifier);

		// remove agent from registered agents
		m_agents.erase(identifier);
	}

	void MessageServiceImpl::AddAgentToCategory(Agent agent, AgentCategory category)
	{
		if (!AgentExists(agent))
		{
			return;
		}

		m_categories[category].insert(agent);
	}

	void MessageServiceImpl::AddAgentToCategory(Agent agent, AgentCategorySet categories)
	{
		// allow exception to propogate if agent does not exist
		for (auto group : categories)
		{
			AddAgentToCategory(agent, group);
		}
	}

	void MessageServiceImpl::RemoveAgentFromCategory(Agent agent, AgentCategory category)
	{
		if (!AgentExists(agent))
		{
			return;
		}

		m_categories[category].erase(agent);
	}

	void MessageServiceImpl::RemoveAgentFromCategory(Agent agent, AgentCategorySet categories)
	{
		// allow exception to propogate if agent does not exist
		for (auto group : categories)
		{
			RemoveAgentFromCategory(agent, group);
		}
	}

	void MessageServiceImpl::RegisterMessageHandler(Agent agent, MessageType type, MessageCallback callback)
	{
		if (!AgentExists(agent))
		{
			return;
		}

		// currently overwrites any existing handler
		m_messageHandlers[agent][type] = callback;
	}

	void MessageServiceImpl::UnregisterMessageHandler(Agent agent, MessageType type)
	{
		if (!AgentExists(agent))
		{
			return;
		}

		m_messageHandlers[agent].erase(type);
	}

//--------------------------------------------------------------------------------
// Message Dispatch and Sending
//--------------------------------------------------------------------------------
	void MessageServiceImpl::DispatchMessages()
	{
		// iterate through mailboxes
		using Mailbox = std::pair<const Agent, std::queue<Message>>;
		for (Mailbox &mailbox : m_mailboxes)
		{
			// get agent identifier and message queue
			Agent current = mailbox.first;
			MessageQueue &queue = mailbox.second;

			// iterate through messages
			while (!queue.empty())
			{
				// get handler for message type
				MessageType type = queue.front().messageType;
				MessageCallback callback = m_messageHandlers[current][type];

				// check that handler exists
				if (callback)
				{
					callback(queue.front());
				}

				// remove from queue
				queue.pop();
			}
		}
	}

	void MessageServiceImpl::SendMessageToAllAgents(Agent from, MessageType type, MessageData data)
	{
		for (auto agent : m_agents)
		{
			SendMessageToAgent(from, agent, type, data);
		}
	}

	void MessageServiceImpl::SendMessageToAgent(Agent from, Agent to, MessageType type, MessageData data)
	{
		if (!AgentExists(from))
		{
			return;
		}

		// do not send message to self
		if (from == to)
		{
			return;
		}

		// generate message
		Message message;
		message.sender = from;
		message.receiver = to;
		message.messageType = type;
		data = data;

		// add message to receiver's mailbox
		AddToMailbox(to, message);
	}

	void MessageServiceImpl::SendMessageToAgent(Agent from, const AgentSet &to, MessageType type, MessageData data)
	{
		for (auto &agent : to)
		{
			SendMessageToAgent(from, agent, type, data);
		}
	}

	void MessageServiceImpl::SendMessageToCategory(Agent from, AgentCategory to, MessageType type, MessageData data)
	{
		// check that category exists
		if (m_categories.find(to) == m_categories.end())
		{
			return;
		}

		for (auto & agent : m_categories.at(to))
		{
			SendMessageToAgent(from, agent, type, data);
		}
	}

	void MessageServiceImpl::SendMessageToCategory(Agent from, const AgentCategorySet &to, MessageType type, MessageData data)
	{
		for (auto &category : to)
		{
			SendMessageToCategory(from, category, type, data);
		}
	}

//--------------------------------------------------------------------------------
// Agent Introspection
//--------------------------------------------------------------------------------
	const MessageServiceImpl::AgentCategorySet MessageServiceImpl::GetRegisteredCategories(Agent agent) const
	{
		if (!AgentExists(agent))
		{
			return AgentCategorySet{};
		}

		AgentCategorySet categories;
		for (auto &group : m_categories)
		{
			if (group.second.find(agent) != group.second.end())
			{
				categories.insert(group.first);
			}
		}

		return categories;
	}

	const MessageServiceImpl::MessageTypeSet MessageServiceImpl::GetRegisteredMessageTypes(Agent agent) const
	{
		if (!AgentExists(agent))
		{
			return MessageTypeSet{};
		}

		MessageTypeSet types;
		for (auto &handler : m_messageHandlers.at(agent))
		{
			types.insert(handler.first);
		}

		return types;
	}

//--------------------------------------------------------------------------------
// Private
//--------------------------------------------------------------------------------
	void MessageServiceImpl::AddToMailbox(Agent agent, Message message)
	{
		// check that receiving agent exists
		if (m_agents.find(agent) == m_agents.end())
		{
			return;
		}

		m_mailboxes[agent].push(message);
	}

	bool AEngine::MessageServiceImpl::AgentExists(Agent agent) const
	{
		return {
			m_agents.find(agent) != m_agents.end()
		};
	}
}
