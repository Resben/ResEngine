local stateTimer = 0.0
local idleState = FSMState.new("idle", { 1 },
	function(deltaTime)
		stateTimer = stateTimer + deltaTime
		if (stateTimer > 2.0) then
			return 1
		end

		return 0
	end,
	function()
		print("Entering idle state")
		stateTimer = 0.0
	end,
	function()
		print("Exiting idle state")
	end
)

local wanderState = FSMState.new("wander", { 0 },
	function(dt)
		stateTimer = stateTimer + dt
		if (stateTimer > 2.0) then
			return 0
		end

		return 1
	end,
	function()
		print("Entering wander state")
		stateTimer = 0.0
	end,
	function()
		print("Exiting wander state")
	end
)

local fsm = FSM.new({ idleState, wanderState }, 0)

function OnStart()
	fsm:Init()
end

function OnUpdate(dt, entt)
	fsm:OnUpdate(dt)
end

function OnDestroy()
	print("move.test -> OnDestroy")
end
