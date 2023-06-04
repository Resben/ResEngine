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
local viewConeAngleDegrees = 60.0

----------------------------------------------------------------------------------------------------
-- internal state variables
local messageAgent
local stateTimer
local turnDir
local turnTime
local wanderTime
local targetPosition
local entityTag
local trackPosition
local messageCooldown = 0.0

local function GetVectorToPlayer()
	return targetPosition - entity:GetTransformComponent().translation
end

local function CalculateAngleBetweenVectors(v1, v2)
	local dot = AEMath.Dot(v1, v2)
	return math.acos(dot / (AEMath.Length(v1) * AEMath.Length(v2)))
end

local State = {
	LAST = -1,
	IDLE = 0,
	SEEK = 1,
	WANDER = 2,
	TURN = 3,
	TRACK = 4
}

local fsm = FSM.new({
	FSMState.new("idle",
		{ State.SEEK, State.WANDER },

		-- on update
		function(dt)
			-- increment state timer
			stateTimer = stateTimer + dt

			-- if the target is close enough, switch to seek state
			if (AEMath.Length(GetVectorToPlayer()) < seekDistanceStart) then
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
			local targetVec = GetVectorToPlayer()
			if (AEMath.Length(targetVec) >= seekDistanceStop) then
				return State.IDLE
			end

			if (AEMath.Length(targetVec) <= attackRange) then
				messageAgent:SendMessageToCategory(
					AgentCategory.PLAYER,
					MessageType.DAMAGE,
					Damage_Data.new(1)
				)
			end

			if (messageCooldown >= 5.0) then
				messageAgent:SendMessageToCategory(
					AgentCategory.ENEMY,
					MessageType.SPOTTED,
					Spotted_Data.new(Vec3.new(targetPosition))
				)
				messageCooldown = 0.0
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

			-- if the player is spotted, switch to seek state
			local targetVec = GetVectorToPlayer()
			local targetDist = AEMath.Length(targetVec)
			local actualAngle = CalculateAngleBetweenVectors(targetVec, -entity:GetTransformComponent():LocalZ())
			local maxAngle = math.rad(viewConeAngleDegrees) / 2.0

			if ((targetDist <= seekDistanceStart) and (actualAngle <= maxAngle)) then
				messageAgent:SendMessageToCategory(
					AgentCategory.ENEMY,
					MessageType.SPOTTED,
					Spotted_Data.new(Vec3.new(targetPosition))
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
	),

	FSMState.new("track",
		{ State.WANDER, State.SEEK },

		function(dt)
			stateTimer = stateTimer + dt

			-- if the player is spotted, switch to seek state
			local playerVec = GetVectorToPlayer()
			local playerDist = AEMath.Length(playerVec)
			local actualAngle = CalculateAngleBetweenVectors(playerVec, -entity:GetTransformComponent():LocalZ())
			local maxAngle = math.rad(viewConeAngleDegrees) / 2.0

			if ((playerDist <= seekDistanceStart) and (actualAngle <= maxAngle)) then
				messageAgent:SendMessageToCategory(
					AgentCategory.ENEMY,
					MessageType.SPOTTED,
					Spotted_Data.new(Vec3.new(targetPosition))
				)
				return State.SEEK
			end

			-- if the player has not been seen, continue to the last position known
			local positionVec = trackPosition - entity:GetTransformComponent().translation
			if (AEMath.Length(positionVec) <= 5.0) then
				return State.WANDER
			end

			entity:GetTransformComponent():LookAt(trackPosition)
			entity:GetPlayerControllerComponent():Move(AEMath.Normalize(positionVec))
			return State.TRACK
		end,

		-- on enter
		function()
			print(entityTag .. " is entering track state")
			stateTimer = 0.0
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

	messageAgent:RegisterHandler(
		MessageType.SPOTTED,
		function(msg)
			-- drop the message if the enemy is seeking or idle
			local currentState = fsm:GetCurrentState()
			if (currentState == State.SEEK or currentState == State.IDLE) then
				return
			end

			-- debug
			print(entityTag .. " has received a track message from " .. entity:GetScene():GetEntity(msg.sender):GetTagComponent().tag)

			-- record the position and switch to track state
			trackPosition = msg.data.pos
			fsm:GoToState(State.TRACK, true)
		end
	)
end

function OnFixedUpdate(dt)
	messageCooldown = messageCooldown + dt
	fsm:OnUpdate(dt)
end

function OnDestroy()
	print(entityTag .. " has been destroyed")
end
