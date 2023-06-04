--[[
author:
	Christien Alden (34119981)

purpose:
	Provides a basic patrol behaviour to entities; they will start in a wander state until they
	see the player, at which point they will seek the player. If the player gets too far away,
	they will return to the idle state for a short period of time before returning to the wander
	state.
--]]

-- load the messaging 'header'
dofile("assets/scripts/messaging.lua")

-- modify these to change the behaviour of the agaent
local idleTime = 10.0
local rotationDegreesPerSecond = 30.0
local seekDistanceStart = 40.0
local seekDistanceStop = 50.0
local attackRange = 7.50

----------------------------------------------------------------------------------------------------
-- internal state variables
local messageAgent
local stateTimer
local turnDir
local turnTime
local wanderTime
local targetPosition
local entityTag

local function GetDistanceToPlayer()
	local targetVec = targetPosition - entity:GetTransformComponent().translation
	return AEMath.Length(targetVec)
end

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
			if (GetDistanceToPlayer() < seekDistanceStart) then
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
			print(entityTag .. " is entering idle state")
		end
	),

	FSMState.new("seek",
		{ State.IDLE },

		-- on update
		function(dt)
			stateTimer = stateTimer + dt

			-- if the target is far enough, switch to idle state
			if (GetDistanceToPlayer() >= seekDistanceStop) then
				return State.IDLE
			end

			if (GetDistanceToPlayer() <= attackRange) then
				messageAgent:SendMessageToCategory(
					AgentCategory.PLAYER,
					MessageType.DAMAGE,
					Damage_Data.new(1)
				)
			end

			-- continue seeking
			entity:GetTransformComponent():LookAt(targetPosition)
			entity:GetPlayerControllerComponent():Move(AEMath.Normalize(targetPosition - entity:GetTransformComponent().translation))
			return State.SEEK
		end,

		-- on enter
		function()
			print(entityTag .. " is entering seek state")
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
			if (GetDistanceToPlayer() < seekDistanceStart) then
				messageAgent:SendMessageToCategory(
					AgentCategory.PLAYER,
					MessageType.SPOTTED,
					{}
				)
				return State.SEEK
			end

			return State.WANDER
		end,

		-- on enter
		function()
			print(entityTag .. " is entering wander state")
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
			print(entityTag .. " is entering turn state")
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
	entityTag = entity:GetTagComponent().tag

	fsm:Init()
	messageAgent = MessageService.CreateAgent(entity:GetTagComponent().ident)
	messageAgent:AddToCategory(AgentCategory.ENEMY)
	messageAgent:RegisterHandler(
		MessageType.POSITION,
		function(msg)
			targetPosition = msg.data.pos
		end
	)
end

function OnFixedUpdate(dt)
	fsm:OnUpdate(dt)
end

function OnDestroy()
	print(entityTag .. " has been destroyed")
end
