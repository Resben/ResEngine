local stateTimer
local wanderTime
local UI

local State = {
    NEUTRAL = 0,
	HAPPY = 1,
	ANGRY = 2,
}

----------------------------------------------------------------------------------------------------
local fsm = FSM.new({

	FSMState.new("neutral",
		{ State.ANGRY, State.HAPPY },

		-- on update
		function(dt)
			stateTimer = stateTimer + dt

            if(stateTimer >= wanderTime) then

                local rand = math.random(1, 2)

                if(rand == 1) then
                    return State.HAPPY
                else
                    return State.ANGRY
                end
            end

			return State.NEUTRAL
		end,

		-- on enter
		function()
			print(entity:GetTagComponent().tag .. " is entering neutral state")
			stateTimer = 0.0
			wanderTime = math.random(1, 5)
		end
	),

	FSMState.new("happy",
		{ State.ANGRY, State.NEUTRAL },

		-- on update
		function(dt)
			stateTimer = stateTimer + dt

            UI:GetRectTransformComponent().translation = entity:GetTransformComponent().translation + Vec3.new(0, 5, 0)

            if(stateTimer >= wanderTime) then

                local rand = math.random(1, 2)

                if(rand == 1) then
                    UI:GetCanvasRendererComponent().active = false
                    return State.NEUTRAL
                else
                    return State.ANGRY
                end
            end

			return State.HAPPY
		end,

		-- on enter
		function()
            UI = SceneManager.GetActiveScene():GetEntity("Teacher_Emote")
            UI:GetCanvasRendererComponent().active = true
            UI:GetPanelComponent():SetTexture("happy.png")
			print(entity:GetTagComponent().tag .. " is entering happy state")
			stateTimer = 0.0
			wanderTime = math.random(1, 5)
		end
	),

	FSMState.new("angry",
        { State.HAPPY, State.NEUTRAL },
		-- on update
		function(dt)
			stateTimer = stateTimer + dt

            UI:GetRectTransformComponent().translation = entity:GetTransformComponent().translation + Vec3.new(0, 5, 0)

            if(stateTimer >= wanderTime) then

                local rand = math.random(1, 2)

                if(rand == 1) then
                    return State.HAPPY
                else
                    UI:GetCanvasRendererComponent().active = false
                    return State.NEUTRAL
                end
            end

			return State.ANGRY
		end,

		-- on enter
		function()
            UI = SceneManager.GetActiveScene():GetEntity("Teacher_Emote")
            UI:GetCanvasRendererComponent().active = true
            UI:GetPanelComponent():SetTexture("angry.png")
			print(entity:GetTagComponent().tag .. " is entering angry state")
			stateTimer = 0.0
			wanderTime = math.random(1, 5)
		end
	)},

	-- initial state
	State.NEUTRAL
);

----------------------------------------------------------------------------------------------------
function OnStart()
	fsm:Init()
end

function OnFixedUpdate(dt)
	fsm:OnUpdate(dt)
end