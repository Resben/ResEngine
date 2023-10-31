dofile("assets/scripts/messaging.lua")

local startAnimation = false
local animationComponent
local animDuration = 0.0
local stateTimer = 0.0


function OnStart()
	messageAgent = MessageService.CreateAgent(entity:GetTagComponent().ident)
	messageAgent:AddToCategory(AgentCategory.BUTTON)
    messageAgent:RegisterMessageHandler(
        MessageType.PRESSED,
        function (msg)
            if (AEMath.Length(position - msg.payload.pos) < 15.0) then
                startAnimation = true
            end
        end
    )
end

function OnFixedUpdate(dt)

	position = entity:GetTransformComponent().translation
    messageAgent:SendMessageToCategory(
        AgentCategory.PLAYER,
        MessageType.PRESSABLE,
        Position_Data.new(position)
    )

    if animationComponent == nil then
        animationComponent = entity:GetAnimationComponent()
        animDuration = animationComponent:GetDuration()
        animationComponent:Pause()
    end

    if startAnimation then
        animationComponent:Play();
        startAnimation = false
        stateTimer = 0
        playing = true
    end

    if playing then
        stateTimer = stateTimer + dt
        if stateTimer >= animDuration then
            playing = false
            animationComponent:Pause()
            animationComponent:SetMaterialColor(animationComponent:GetMaterialString(animationComponent:GetMeshMaterialCount() - 1), Vec4.new(0.0, 1.0, 0.0, 1.0))
        end
    end
end

function OnDestroy()
	messageAgent:Destroy()
end
