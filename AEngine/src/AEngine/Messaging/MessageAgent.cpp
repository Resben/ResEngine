/**
 * \file
 * \author Christien Alden (34119981)
*/
#include "MessageAgent.h"
#include "AEngine/Core/Logger.h"
#include "MessageServiceImpl.h"
#include <stdexcept>

namespace AEngine
{
	MessageAgent::~MessageAgent()
	{
		try
		{
			m_service.DestroyAgent(m_identifier);
		}
		catch (std::runtime_error &e)
		{
			AE_LOG_ERROR("Failed to destroy agent: {}", e.what());
		}
	}

	void MessageAgent::AddToCategory(AgentCategory category)
	{
		try
		{
			m_service.AddAgentToCategory(m_identifier, category);
		}
		catch (std::runtime_error &e)
		{
			AE_LOG_ERROR("Failed to add agent to category: {}", e.what());
		}
	}

	void MessageAgent::RemoveFromCategory(AgentCategory category)
	{
		m_service.RemoveAgentFromCategory(m_identifier, category);
	}

	void MessageAgent::RegisterMessageHandler(MessageType type, MessageCallback callback)
	{
		m_service.RegisterMessageHandler(m_identifier, type, callback);
	}

	void MessageAgent::UnregisterMessageHandler(MessageType type)
	{
		m_service.UnregisterMessageHandler(m_identifier, type);
	}

	void MessageAgent::BroadcastMessage(MessageType type, MessageData payload)
	{
		m_service.SendMessageToAllAgents(m_identifier, type, payload);
	}

	void MessageAgent::SendMessageToAgent(Agent to, MessageType type, MessageData payload)
	{
		m_service.SendMessageToAgent(m_identifier, to, type, payload);
	}

	void MessageAgent::SendMessageToAgent(AgentSet to, MessageType type, MessageData payload)
	{
		m_service.SendMessageToAgent(m_identifier, to, type, payload);
	}

	void MessageAgent::SendMessageToCategory(AgentCategory to, MessageType type, MessageData payload)
	{
		m_service.SendMessageToCategory(m_identifier, to, type, payload);
	}

	void MessageAgent::SendMessageToCategory(AgentCategorySet to, MessageType type, MessageData payload)
	{
		m_service.SendMessageToCategory(m_identifier, to, type, payload);
	}

	const MessageAgent::AgentCategorySet MessageAgent::GetRegisteredCategories() const
	{
		return m_service.GetRegisteredMessageTypes(m_identifier);
	}

	const MessageAgent::MessageTypeSet MessageAgent::GetRegisteredMessageTypes() const
	{
		return m_service.GetRegisteredMessageTypes(m_identifier);
	}
}
