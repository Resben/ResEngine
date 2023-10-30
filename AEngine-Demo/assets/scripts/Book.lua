-- book.lua
dofile("assets/scripts/messaging.lua")

local messageAgent
local position

function OnStart()
	messageAgent = MessageService.CreateAgent(entity:GetTagComponent().ident)
	messageAgent:AddToCategory(AgentCategory.BOOK)
	messageAgent:RegisterMessageHandler(
		MessageType.POSITION,
		function (msg)
            print("book received position")
			if (AEMath.Length(position - msg.payload.pos) < 10.0) then
				messageAgent:SendMessageToAgent(
					msg.sender,
					MessageType.PICKUP,
                    Pickup_data.new(entity:GetTagComponent().tag)
				)
			end
		end
	)
end

function OnFixedUpdate(dt)
	position = entity:GetTransformComponent().translation
end

function OnDestroy()
	messageAgent:Destroy()
end
