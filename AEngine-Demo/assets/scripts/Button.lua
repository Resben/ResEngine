-- Button.lua
dofile("assets/scripts/messaging.lua")

-- State
local jealousyCounter = 1
local isPressed = false
local position


-- Emotions
local State = {
	Happy = 0,
	Jealous = 1,
	Depressed = 2
}

-- Animation
local startAnimation = false
local animationComponent
local animDuration = 0.0
local animationTimer = 0.0

-- FSM
local fsm = FSM.new({
	-- States
	FSMState.new(
		"Happy",
		{ State.Jealous },	--transitions to
		function(dt) -- OnUpdate
            -- broadcast the pressable affordance
            if not isPressed then
                messageAgent:SendMessageToCategory(
                    { AgentCategory.PLAYER, AgentCategory.TEACHER },
                    MessageType.PRESSABLE,
                    Position_Data.new(position)
                )
            end
			return State.Happy
		end,
		function() -- OnEntry
            elapsedTime = 0
            jealousyCounter = 0
            animationComponent:SetMaterialColor(
                animationComponent:GetMaterialString(
                    animationComponent:GetMeshMaterialCount() - 1
                ),
                Vec4.new(1.0, 1.0, 0.0, 1.0)
            )
            messageAgent:BroadcastMessage(
                MessageType.BUTTON_HAPPY,
                {}
            )
		end
	),

	FSMState.new(
		"Jealous",
		{ State.Happy, State.Depressed },
		function(dt) -- OnUpdate
            -- broadcast the pressable affordance
            if not isPressed then
                messageAgent:SendMessageToCategory(
                    { AgentCategory.PLAYER, AgentCategory.TEACHER },
                    MessageType.PRESSABLE,
                    Position_Data.new(position)
                )
            end
			return State.Jealous
		end,
		function() -- OnEntry
			elapsedTime = 0
            animationComponent:SetMaterialColor(
                animationComponent:GetMaterialString(
                    animationComponent:GetMeshMaterialCount() - 1
                ),
                Vec4.new(0.0, 0.1, 0.0, 1.0)
            )
            messageAgent:BroadcastMessage(
			 	MessageType.BUTTON_JEALOUS,
			 	{}
			)
		end
	),

	FSMState.new(
		"Depressed",
		{ State.Happy },
		function(dt) -- OnUpdate
            -- do nothing, just wait for the teacher to cheer you up
			return State.Depressed
		end,
		function() -- OnEntry
			elapsedTime = 0
            animationComponent:SetMaterialColor(
                animationComponent:GetMaterialString(
                    animationComponent:GetMeshMaterialCount() - 1
                ),
                Vec4.new(0.0, 0.0, 1.0, 1.0)
            )
            messageAgent:BroadcastMessage(
             	MessageType.BUTTON_DEPRESSED,
             	{}
            )
		end
	)},

	-- Initial State
	State.Happy
)

function OnStart()
	messageAgent = MessageService.CreateAgent(entity:GetTagComponent().ident)
	messageAgent:AddToCategory(AgentCategory.BUTTON)

    -- request to push the button
    messageAgent:RegisterMessageHandler(
        MessageType.PRESSED,
        function (msg)
            -- don't do anything if the button is already pressed
            if isPressed then
                return
            end

            -- return if the player is too far away
            if (AEMath.Length(position - msg.payload.pos) >= 15.0) then
                return 
            end

            -- check for depressed state
            if fsm:GetCurrentState() == State.Depressed then
                return
            end

            -- go to happy state and start animation
            fsm:GoToState(State.Happy)
            startAnimation = true
        end
    )

    messageAgent:RegisterMessageHandler(
        MessageType.BUTTON_CHEER_UP,
        function (msg)
            -- check that close enough
            if (AEMath.Length(position - msg.payload.pos) >= 15.0) then
                return 
            end
            
            if fsm:GetCurrentState() == State.Depressed then
                fsm:GoToState(State.Happy)
            end
        end
    )

    -- informational message from other agents
    messageAgent:RegisterMessageHandler(
        MessageType.BUTTON_PRESSED,
        function (msg)
            jealousyCounter = jealousyCounter + 1
            if jealousyCounter >= 6 then
                fsm:GoToState(State.Depressed)
            elseif jealousyCounter >= 3 then
                fsm:GoToState(State.Jealous)
            end
        end
    )

    -- get the animation component
    animationComponent = entity:GetAnimationComponent()
    animDuration = animationComponent:GetDuration()
    animationComponent:Pause()

    -- initialize the FSM
    fsm:Init()
end

function OnFixedUpdate(dt)
    position = entity:GetTransformComponent().translation
    fsm:OnUpdate(dt)

    -- here is where we set the button to pressed
    if startAnimation then
        animationComponent:Play();
        startAnimation = false
        animationTimer = 0
        isPressed = true
        messageAgent:SendMessageToCategory(
            AgentCategory.BUTTON,
            MessageType.BUTTON_PRESSED,
            {}
        )
    end

    if isPressed then
        animationTimer = animationTimer + dt
        if animationTimer >= animDuration then
            isPressed = false
            animationComponent:Pause()
        end
    end
end

function OnDestroy()
	messageAgent:Destroy()
end
