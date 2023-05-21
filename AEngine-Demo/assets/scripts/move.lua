local speed = 5.0
local stateTimer = 0.0
local function targetDistance()
	local camPos = Scene.GetDebugCamera():GetPosition()
	local targetVec = camPos - entity:GetTransformComponent().translation
	return Math.Length(targetVec)
end

local function counter()
	local count = 0
	return function()
		count = count + 1
		return count
	end
end

local idleState = FSMState.new("idle", { 1, 2 },
	function(dt)
		-- increment state timer
		stateTimer = stateTimer + dt

		-- if the target is close enough, switch to seek state
		if (targetDistance() < 30.0) then
			return 1
		end

		-- continue idling
		entity:RotateLocal(Math.Radians(30) * dt, Vec3.new(0.0, 1.0, 0.0))
		return 0
	end,
	function()
		print(entity:GetTagComponent().tag .. " is entering idle state")
		stateTimer = 0.0
	end
)

local seekState = FSMState.new("seek", { 0 },
	function(dt)
		stateTimer = stateTimer + dt

		-- if the target is far enough, switch to idle state
		if (targetDistance() >= 60.0) then
			return 0
		end

		-- continue seeking
		local camPos = Scene.GetDebugCamera():GetPosition()
		entity:GetTransformComponent():LookAt(camPos)
		entity:TranslateLocal(Vec3.new(0.0, 0.0, speed * dt))
		return 1
	end,
	function()
		print(entity:GetTagComponent().tag .. " is entering seek state")
		stateTimer = 0.0
	end
)

-- local wanderState = FSMState.new("wander", { 1 },
-- 	function(dt)

-- )

local fsm = FSM.new({ idleState, seekState }, 1)

function OnStart()
	fsm:Init()
end

function OnUpdate(dt, entt)
	fsm:OnUpdate(dt)
end

function OnDestroy()
	print("move.test -> OnDestroy")
end
