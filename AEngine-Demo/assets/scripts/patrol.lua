--[[
author:
	Christien Alden (34119981)

purpose:
	Provides a basic patrol behaviour to entities; they will start in a wander state until they
	see the player, at which point they will seek the player. If the player gets too far away,
	they will return to the idle state for a short period of time before returning to the wander
	state.
--]]

-- modify these to change the behaviour of the agaent
local idleTime = 10.0
local rotationDegreesPerSecond = 30.0
local seekDistanceStart = 40.0
local seekDistanceStop = 50.0
local targetEntity = "Player"

----------------------------------------------------------------------------------------------------
local function DistanceToEntity(target)
	local targetPos = SceneManager.GetActiveScene():GetEntity(target):GetTransformComponent().translation
	local targetVec = targetPos - entity:GetTransformComponent().translation
	return AEMath.Length(targetVec)
end

-- internal state variables
local stateTimer
local turnDir
local turnTime
local wanderTime

local State = {
	LAST = -1,
	IDLE = 0,
	SEEK = 1,
	WANDER = 2,
	TURN = 3
}

local fsm = FSM.new({
	FSMState.new("idle",
		{ State.SEEK, State.WANDER },

		-- on update
		function(dt)
			-- increment state timer
			stateTimer = stateTimer + dt

			-- if the target is close enough, switch to seek state
			if (DistanceToEntity(targetEntity) < seekDistanceStart) then
				return State.SEEK
			end

			-- if the state timer is greater than 10 seconds, switch to wander state
			if (stateTimer >= idleTime) then
				return State.WANDER
			end

			return State.IDLE
		end,

		-- on enter
		function()
			stateTimer = 0
			print(entity:GetTagComponent().tag .. " is entering idle state")
		end
	),

	FSMState.new("seek",
		{ State.IDLE },

		-- on update
		function(dt)
			stateTimer = stateTimer + dt

			-- if the target is far enough, switch to idle state
			if (DistanceToEntity(targetEntity) >= seekDistanceStop) then
				return State.IDLE
			end

			-- continue seeking
			local targetPos = SceneManager.GetActiveScene():GetEntity("Player"):GetTransformComponent().translation
			entity:GetTransformComponent():LookAt(targetPos)
			entity:GetPlayerControllerComponent():Move(AEMath.Normalize(targetPos - entity:GetTransformComponent().translation))
			return State.SEEK
		end,

		-- on enter
		function()
			print(entity:GetTagComponent().tag .. " is entering seek state")
			stateTimer = 0.0
		end
	),

	FSMState.new("wander",
		{ State.SEEK, State.TURN },

		-- on update
		function(dt)
			stateTimer = stateTimer + dt

			-- rotate 90 degrees every 5 seconds
			if (stateTimer > wanderTime) then
				return State.TURN
			end

			-- wander a little
			local direction = AEMath.RotateVec(Vec3.new(0.0, 0.0, -1.0), entity:GetTransformComponent().orientation)
			entity:GetPlayerControllerComponent():Move(direction)

			-- if the target is close enough, switch to seek state
			if (DistanceToEntity(targetEntity) < seekDistanceStart) then
				return State.SEEK
			end

			return State.WANDER
		end,

		-- on enter
		function()
			print(entity:GetTagComponent().tag .. " is entering wander state")
			stateTimer = 0.0
			wanderTime = math.random(1, 5)
		end
	),

	FSMState.new("turn",
		{ State.LAST },

		-- on update
		function(dt)
			stateTimer = stateTimer + dt
			if (stateTimer >= turnTime) then
				return State.LAST
			end

			if (turnDir > 0.5) then
				entity:RotateLocal(math.rad(rotationDegreesPerSecond) * dt, Vec3.new(0.0, 1.0, 0.0))
			else
				entity:RotateLocal(math.rad(-rotationDegreesPerSecond) * dt, Vec3.new(0.0, 1.0, 0.0))
			end

			return State.TURN
		end,

		-- on enter
		function()
			print(entity:GetTagComponent().tag .. " is entering turn state")
			stateTimer = 0.0
			turnDir = math.random(0, 1)
			turnTime = math.random(1, 5)
		end
	)},

	-- initial state
	State.WANDER
);

----------------------------------------------------------------------------------------------------
function OnStart()
	fsm:Init()
end

function OnFixedUpdate(dt)
	fsm:OnUpdate(dt)
end

function OnDestroy()
	print("move.test -> OnDestroy")
end
