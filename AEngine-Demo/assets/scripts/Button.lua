-- Button.lua
dofile("assets/scripts/messaging.lua")

-- State
local isPressed = false
local position

-- Emotions
local State = {
	Happy = 0,
	Jealous = 1,
	Depressed = 2
}

-- FCM
local fcm
local Concept = {
    Happiness = 0,
    Jealousy = 1,
    Pressed = 2,
    OtherPressed = 3
}

-- BDI
local bdi

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
		{ State.Jealous, State.Depressed },	--transitions to
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
            animationComponent:SetMaterialColor(
                animationComponent:GetMaterialString(
                    animationComponent:GetMeshMaterialCount() - 1
                ),
                Vec4.new(1.0, 1.0, 0.0, 1.0)
            )
            -- messageAgent:BroadcastMessage(
            --     MessageType.BUTTON_HAPPY,
            --     {}
            -- )
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
            print ("button is jealous")
			elapsedTime = 0
            animationComponent:SetMaterialColor(
                animationComponent:GetMaterialString(
                    animationComponent:GetMeshMaterialCount() - 1
                ),
                Vec4.new(0.0, 0.1, 0.0, 1.0)
            )
            -- messageAgent:BroadcastMessage(
			--  	MessageType.BUTTON_JEALOUS,
			--  	{}
			-- )
		end
	),

	FSMState.new(
		"Depressed",
		{ State.Happy, State.Jealous },
		function(dt) -- OnUpdate
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

            -- broadcast depressed message (for teacher)
            messageAgent:BroadcastMessage(
                MessageType.BUTTON_DEPRESSED,
                {}
            )
		end
	)},

	-- Initial State
	State.Happy
)


function SetupFCM()
    -- register the FCM
    fcmComp = entity:GetFCMComponent()
    fcm = fcmComp:GetFCM()

    fcm:AddConcept(
        "Happiness",
        0.5,
        0.5,
        0.01,
        function(value)
            bdi:AddBelief("is_happy")
        end,
        function(value)
            bdi:RemoveBelief("is_happy")
        end
    )

    fcm:AddConcept(
        "Jealousy",
        0.5,
        0.6,
        0.005,
        function(value)
            bdi:AddBelief("is_jealous")
        end,
        function(value)
            bdi:RemoveBelief("is_jealous")
        end
    )

    fcm:AddConcept(
        "Pressed",
        0.0,
        0.5,
        0.0,
        nil,
        nil
    )

    fcm:AddConcept(
        "OtherPressed",
        0.0,
        0.5,
        0.0,
        nil,
        nil
    )

    fcm:AddEdge(
        Concept.Pressed,
        Concept.Happiness,
        0.10
    )

    fcm:AddEdge(
        Concept.OtherPressed,
        Concept.Jealousy,
        0.10
    )

    fcm:Init()
end

function SetupBDI()
    bdiComp = entity:GetBDIComponent()
    bdi = bdiComp:GetAgent()

    -- add desires
    bdi:AddDesire(
        "show_happiness",
        "BELIEF(is_happy)",
        0.7
    )

    bdi:AddDesire(
        "show_jealousy",
        "BELIEF(is_jealous)",
        0.8
    )


    bdi:AddIntention(
        "become_jealous",
        "DESIRE(show_jealousy)",
        function(str)
            fsm:GoToState(State.Jealous)
        end
    )

    bdi:AddIntention(
        "become_happy",
        "DESIRE(show_happiness)",
        function(str)
            fsm:GoToState(State.Happy)
        end
    )

    bdi:AddIntention(
        "show_sadness",
        "AND( NOT(BELIEF(is_happy)), NOT(BELIEF(is_jealous)) )",
        function(str)
            fsm:GoToState(State.Depressed)
        end
    )
end

function OnStart()
	messageAgent = MessageService.CreateAgent(entity:GetTagComponent().ident)
	messageAgent:AddToCategory(AgentCategory.BUTTON)

    SetupBDI()
    SetupFCM()

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
            -- if fsm:GetCurrentState() == State.Depressed then
                -- return
            -- end

            -- go to happy state and start animation
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

            -- if depressed, start the animation (make happy!)
            if fsm:GetCurrentState() == State.Depressed then
                startAnimation = true
            end
        end
    )

    -- informational message from other agents
    messageAgent:RegisterMessageHandler(
        MessageType.BUTTON_PRESSED,
        function (msg)
            fcm:SetConceptValue(Concept.OtherPressed, 1.0)
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
    fcm:OnUpdate(dt)
    bdi:OnUpdate()

    -- reset the FCM values of the 'events'
    fcm:SetConceptValue(Concept.Pressed, 0.0)
    fcm:SetConceptValue(Concept.OtherPressed, 0.0)

    -- here is where we set the button to pressed
    if startAnimation then
        animationComponent:Play();
        startAnimation = false
        animationTimer = 0
        isPressed = true
        fcm:SetConceptValue(Concept.Pressed, 1.0)
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
