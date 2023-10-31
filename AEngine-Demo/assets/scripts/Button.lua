dofile("assets/scripts/messaging.lua")

function OnStart()
	messageAgent = MessageService.CreateAgent(entity:GetTagComponent().ident)
	messageAgent:AddToCategory(AgentCategory.BUTTON)
end

function OnFixedUpdate(dt)
	position = entity:GetTransformComponent().translation
    messageAgent:SendMessageToCategory(
        AgentCategory.PLAYER,
        MessageType.PRESSABLE,
        Position_Data.new(position)
    )
end

function OnDestroy()
	messageAgent:Destroy()
end
