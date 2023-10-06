-- modify these to change the behaviour of the agaent
local rotationDegreesPerSecond = 60.0
local speed = 5.0

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

local State = {
	LAST = -1,
	IDLE = 0,
	WANDER = 1,
	TURN = 2,
    MOVE = 3
}

----------------------------------------------------------------------------------------------------
local fsm = FSM.new({
	FSMState.new("idle",
		{ State.WANDER, State.TURN, State.MOVE },

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
		{ State.TURN, State.IDLE, State.MOVE },

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
		{ State.TURN, State.IDLE, State.WANDER },
		-- on update
		function(dt)

			stateTimer = stateTimer + dt

			if(atDestination) then

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
				if(waypoints:Size() > 0) then

					local direction = Vec3.new(waypoints[currentWaypoint], waypoints[currentWaypoint + 1], waypoints[currentWaypoint + 2]) - entity:GetTransformComponent().translation

					if(direction.x < 0.05 and direction.z < 0.05) then
						currentWaypoint = currentWaypoint + 3
						if(currentWaypoint >= waypoints:Size()) then
							print("Destination reached here")
							atDestination = true
						end
					end

					entity:GetPlayerControllerComponent():Move(direction)
				else
					print("No waypoints found")
					atDestination = true
				end
			end

			return State.MOVE
		end,

		-- on enter
		function()
			grid = SceneManager.GetActiveScene():GetEntity("AI_Grid"):GetNavigationGridComponent()
			print(entity:GetTagComponent().tag .. " is entering move state")
			atDestination = false
			currentWaypoint = 1
			stateTimer = 0.0
			if(atLocationA) then
				waypoints = grid:GetWaypoints(Vec3.new(0.0, 0.0, 0.0), Vec3.new(540.0, 0.0, 44.0))
				atLocationA = false
			else
				waypoints = grid:GetWaypoints(Vec3.new(0.0, 0.0, 0.0), Vec3.new(0.0, 0.0, 0.0))
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
				entity:RotateLocal(math.rad(rotationDegreesPerSecond) * dt, Vec3.new(0.0, 1.0, 0.0))
			else
				entity:RotateLocal(math.rad(-rotationDegreesPerSecond) * dt, Vec3.new(0.0, 1.0, 0.0))
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
	)},

	-- initial state
	State.IDLE
);

----------------------------------------------------------------------------------------------------
function OnStart()
	fsm:Init()
end

function OnFixedUpdate(dt)
	fsm:OnUpdate(dt)
end
