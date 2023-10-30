-- modify these to change the behaviour of the agaent
local turnStateRotationDegreesPerSecond = 60.0
local moveStateRotationDegreesPerSecond = 120.0
local min_speed = 20.0  -- Minimum speed
local max_speed = 50.0
local deceleration_distance = 10.0  -- Start decelerating when closer than this distance
local acceleration_distance = 20.0  -- Start accelerating when farther than this distance
local moveRotateFlag = false

local initialForward = Vec3.new(0.0, 0.0, 1.0)

----------------------------------------------------------------------------------------------------
-- internal state variables
local stateTimer
local turnDir
local turnTime
local wanderTime
local atDestination

--testing
local atLocationA = true

local grid
local waypoints
local currentWaypoint

local messageAgent
local bookMoved = false
local bookHome

local State = {
	LAST = -1,
	IDLE = 0,
	WANDER = 1,
	TURN = 2,
    MOVE = 3,
	CHECK = 4
}

function TraverseAStar(dt)
	if waypoints:Size() > 0 then

		local direction = Vec3.new(waypoints[currentWaypoint], waypoints[currentWaypoint + 1], waypoints[currentWaypoint + 2]) - entity:GetTransformComponent().translation
		local distance = math.sqrt(direction.x * direction.x + direction.z * direction.z)
		local speed = entity:GetPlayerControllerComponent():GetSpeed()

		-- Deceleration
		if distance < deceleration_distance then
			local required_deceleration = (speed ^ 2) / (2 * distance)
			newSpeed = speed - required_deceleration * dt
			if newSpeed < min_speed then
				newSpeed = min_speed
			end
		end

		-- Acceleration
		if distance > acceleration_distance then
			local acceleration_factor = 5.0
			newSpeed = speed + acceleration_factor * dt

			if newSpeed > max_speed then
				newSpeed = max_speed
			end
		end

		entity:GetPlayerControllerComponent():SetSpeed(newSpeed)

		local normCurrentForward = AEMath.Normalize(AEMath.RotateVec(initialForward, entity:GetTransformComponent().orientation))
		local normDirection = AEMath.Normalize(direction)
		local dot = AEMath.Dot(normCurrentForward, normDirection)
		local cross = AEMath.Cross(normCurrentForward, normDirection)

		if dot < 0.99 then
			if cross.y < 0 then
				entity:RotateLocal(math.rad(-moveStateRotationDegreesPerSecond) * dt, Vec3.new(0.0, 1.0, 0.0))
			else
				entity:RotateLocal(math.rad(moveStateRotationDegreesPerSecond) * dt, Vec3.new(0.0, 1.0, 0.0))
			end
		end

		if distance < 0.5 then
			currentWaypoint = currentWaypoint + 3
			moveRotateFlag = true
			if currentWaypoint >= waypoints:Size() then
				atDestination = true
			end
		end

		entity:GetPlayerControllerComponent():Move(direction)
	else
		print("No waypoints found")
		atDestination = true
	end
end

----------------------------------------------------------------------------------------------------
local fsm = FSM.new({
	FSMState.new("idle",
		{ State.WANDER, State.TURN, State.MOVE, State.CHECK },

		-- on update
		function(dt)
			stateTimer = stateTimer + dt

            local whatToDo = math.random(0, 3)

			-- if something switch to wander
			if (whatToDo == 0) then
				return State.WANDER
			end

			-- if something switch to turn
			if (whatToDo == 1) then
				return State.TURN
			end

            -- if something switch to move
            if (whatToDo == 2) then
                return State.MOVE
            end

			return State.IDLE
		end,

		-- on enter
		function()
			print(entity:GetTagComponent().tag .. " is entering idle state")
			entity:GetAnimationComponent():SetAnimation("NPC.gltf/walk")
			stateTimer = 0.0
		end
	),

	FSMState.new("wander",
		{ State.TURN, State.IDLE, State.MOVE, State.CHECK },

		-- on update
		function(dt)
			stateTimer = stateTimer + dt

            local whatToDo = math.random(0, 3)

            if(stateTimer >= wanderTime) then
                -- if something switch to turn
                if (whatToDo == 0) then
                    return State.TURN
                end

                -- if something switch to idle
                if (whatToDo == 1) then
                    return State.IDLE
                end

                -- if somethign switch to move
                if (whatToDo == 2) then
                    return State.MOVE
                end
            end

			-- wander a little
			--local direction = AEMath.RotateVec(Vec3.new(0.0, 0.0, -1.0), entity:GetTransformComponent().orientation)
			--entity:GetPlayerControllerComponent():Move(direction)

			return State.WANDER
		end,

		-- on enter
		function()
			print(entity:GetTagComponent().tag .. " is entering wander state")
			entity:GetAnimationComponent():SetAnimation("NPC.gltf/walk")
			stateTimer = 0.0
			wanderTime = math.random(1, 1)
		end
	),

	FSMState.new("move",
		{ State.TURN, State.IDLE, State.WANDER, State.CHECK },
		-- on update
		function(dt)

			stateTimer = stateTimer + dt

			if atDestination then

				local whatToDo = math.random(0, 3)

				-- if something switch to turn
				if (whatToDo == 0) then
					return State.TURN
				end

				-- if something switch to idle
				if (whatToDo == 1) then
					return State.IDLE
				end

				-- if somethign switch to wander
				if (whatToDo == 2) then
					return State.WANDER
				end
				
			else
				TraverseAStar(dt)
			end

			return State.MOVE
		end,

		-- on enter
		function()
			grid = SceneManager.GetActiveScene():GetEntity("AI_Grid"):GetNavigationGridComponent()
			print(entity:GetTagComponent().tag .. " is entering move state")
			moveRotateFlag = false
			atDestination = false
			currentWaypoint = 1
			stateTimer = 0.0
			if(atLocationA) then
				waypoints = grid:GetWaypoints(entity:GetTransformComponent().translation, Vec3.new(543.5, 0.0, 41.5), true)
				atLocationA = false
			else
				waypoints = grid:GetWaypoints(entity:GetTransformComponent().translation, Vec3.new(7.5, 0.0, 0.0), true)
				atLocationA = true
			end

			entity:GetAnimationComponent():SetAnimation("NPC.gltf/walk")
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
				entity:RotateLocal(math.rad(turnStateRotationDegreesPerSecond) * dt, Vec3.new(0.0, 1.0, 0.0))
			else
				entity:RotateLocal(math.rad(-turnStateRotationDegreesPerSecond) * dt, Vec3.new(0.0, 1.0, 0.0))
			end

			return State.TURN
		end,

		-- on enter
		function()
			print(entity:GetTagComponent().tag .. " is entering turn state")
			entity:GetAnimationComponent():SetAnimation("NPC.gltf/walk")
			stateTimer = 0.0
			turnDir = math.random(0, 1)
			turnTime = math.random(1, 2)
		end
	),

	FSMState.new("check",
		{ State.TURN, State.IDLE, State.WANDER, State.MOVE },

		-- on update
		function(dt)
			if(atDestination) then
				
			else
				TraverseAStar(dt)
			end
		end,

		-- on enter
		function()
			waypoints = grid:GetWaypoints(entity:GetTransformComponent().translation, Vec3.new(584, 8, 50.5), true)
		end
	)},

	-- initial state
	State.IDLE
);

----------------------------------------------------------------------------------------------------
function OnStart()
	messageAgent = MessageService.CreateAgent(entity:GetTagComponent().ident)
	messageAgent:AddToCategory(AgentCategory.TEACHER)
	fsm:Init()
end

function OnFixedUpdate(dt)
	fsm:OnUpdate(dt)
end
