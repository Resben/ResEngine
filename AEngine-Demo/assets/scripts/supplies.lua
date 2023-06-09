-- supplies.lua
local messageAgent
local position

function OnStart()
	messageAgent = MessageService.CreateAgent(entity:GetTagComponent().ident)
	messageAgent:AddToCategory(AgentCategory.SUPPLIES)
	messageAgent:RegisterMessageHandler(
		MessageType.POSITION,
		function (msg)
			if (AEMath.Length(position - msg.payload.pos) < 5.0) then
				messageAgent:SendMessageToAgent(
					msg.sender,
					MessageType.PICKUP,
					{}
				)
				messageAgent:Destroy()
				entity:Destroy()
			end
		end
	)
end

function OnFixedUpdate(dt)
	position = entity:GetTransformComponent().translation
	-- do nothing!!
end

function OnDestroy()
	messageAgent:Destroy()
end
