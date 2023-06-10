-- coreLoop
dofile("assets/scripts/messaging.lua")

-- internal
local messageAgent

function OnStart()
	messageAgent = MessageService.CreateAgent(entity:GetTagComponent().ident)
	messageAgent:AddToCategory(AgentCategory.RUNTIME)
	messageAgent:RegisterMessageHandler(
		MessageType.TEXT,
		function (msg)
			entity:GetTextComponent().text = msg.payload.text
		end
	)
end

function OnDestroy()
	messageAgent:Destroy()
end
