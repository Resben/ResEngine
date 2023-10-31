-- book.lua
dofile("assets/scripts/messaging.lua")

local messageAgent
local position
local bookHomePosition = Vec3.new(573, 8, 52.5)

function OnStart()

    entity:GetPhysicsBody():SetTranslation(bookHomePosition)

	messageAgent = MessageService.CreateAgent(entity:GetTagComponent().ident)
	messageAgent:AddToCategory(AgentCategory.BOOK)
	messageAgent:RegisterMessageHandler(
		MessageType.POSITION,
		function (msg)
            position.y = 0;
            msg.payload.pos.y = 0;
			if (AEMath.Length(position - msg.payload.pos) < 10.0) then
				messageAgent:SendMessageToAgent(
					msg.sender,
					MessageType.PICKUP,
                    Pickup_Data.new(entity:GetTagComponent().tag)
				)
			end
		end
	)

    messageAgent:RegisterMessageHandler(
        MessageType.HOME,
        function(msg)
            if(position.x == bookHomePosition.x) then
                messageAgent:SendMessageToAgent(
                    msg.sender,
                    MessageType.HOME,
                    Home_Data.new(true, position)
                )
            else
                messageAgent:SendMessageToAgent(
                    msg.sender,
                    MessageType.HOME,
                    Home_Data.new(false, position)
                )
            end
        end
    )
end

function OnFixedUpdate(dt)
	position = entity:GetTransformComponent().translation
    messageAgent:SendMessageToCategory(
        AgentCategory.PLAYER,
        MessageType.INTERACTABLE,
        Position_Data.new(position)
    )
end

function OnDestroy()
	messageAgent:Destroy()
end
