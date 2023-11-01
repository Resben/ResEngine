-- book.lua
dofile("assets/scripts/messaging.lua")

local messageAgent
local position
local bookHomePosition = Vec3.new(573, 8, 52.5)
local isHome = true
local isHeld = false

local State = {
    HOME = 0,
    HELD = 1,
    MISSING = 2
}

local fsm = FSM.new({
    -- States
	FSMState.new(
		"Home",
        { State.HELD, State.MISSING },	--transitions to
		function(dt) -- OnUpdate
            -- print ui component
            print("Home")
            return State.HOME
		end,
		function() -- OnEntry
            elapsedTime = 0
		end
	),

    FSMState.new(
        "Held",
        { State.HOME, State.MISSING },
        function(dt)
            -- print love ui
            print("Held")
            return State.HELD
        end,
        function()
            elapsedTime = 0
        end
    ),

    FSMState.new(
        "Missing",
        { State.HOME, State.HELD },
        function(dt)
            -- print missing ui
            print("Missing")
            return State.MISSING
        end,
        function()
            elapsedTime = 0
        end
    )},

    State.HOME
)


function OnStart()
    -- set the entity to the home position
    entity:GetPhysicsBody():SetTranslation(bookHomePosition)
    -- initialize fsm
    fsm:Init()

    -- initialize message agent
	messageAgent = MessageService.CreateAgent(entity:GetTagComponent().ident)
	messageAgent:AddToCategory(AgentCategory.BOOK)

    -- requests from the player and teacher to be picked up
	messageAgent:RegisterMessageHandler(
		MessageType.POSITION,

        -- msg contains the requesters position
		function (msg)
            position.y = 0;
            msg.payload.pos.y = 0;

            -- if the player is close enough, send a pickup message
			if (AEMath.Length(position - msg.payload.pos) < 10.0) then
                isHeld = true
				messageAgent:SendMessageToAgent(
					msg.sender,
					MessageType.PICKUP,
                    Pickup_Data.new(entity:GetTagComponent().tag)
				)
			end
		end
	)

    messageAgent:RegisterMessageHandler(
        MessageType.DROPPED,
        function (msg)
            isHeld = false
        end
    )

    messageAgent:RegisterMessageHandler(
        MessageType.HOME,
        function(msg)
            if (isHome) then
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
    -- get the position every frame and send message to player with position
	position = entity:GetTransformComponent().translation
    messageAgent:SendMessageToCategory(
        AgentCategory.PLAYER,
        MessageType.INTERACTABLE,
        Position_Data.new(position)
    )

    -- Detect is home
    if (math.abs(position.x - bookHomePosition.x) < 1.0) and
        (math.abs(position.z - bookHomePosition.z) < 1.0)
    then
        isHome = true
    else
        isHome = false
    end

    -- detect the state of the book
    if (isHeld) then
        fsm:GoToState(State.HELD)
    elseif (isHome) then
        fsm:GoToState(State.HOME)
    else
        fsm:GoToState(State.MISSING)
    end

    -- update the fsm
    fsm:OnUpdate(dt)
end

function OnDestroy()
	messageAgent:Destroy()
end
