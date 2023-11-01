-- modify these to change the behaviour of the agaent
local turnStateRotationDegreesPerSecond = 60.0
local moveStateRotationDegreesPerSecond = 120.0
local min_speed = 40.0  -- Minimum speed
local max_speed = 80.0
local deceleration_distance = 10.0  -- Start decelerating when closer than this distance
local acceleration_distance = 20.0  -- Start accelerating when farther than this distance
local moveRotateFlag = false

local initialForward = Vec3.new(0.0, 0.0, 1.0)

----------------------------------------------------------------------------------------------------
-- internal state variables
local stateTimer
local atDestination
local flag = false

-- book states
local checked = false
local wasBookedThere = false
local bookPosition
local bookHomePosition = Vec3.new(573, 8, 52.5)
local holdingBook = false
local book

-- AStar
local teacherHomePosition = Vec3.new(533, 16, 40)
local grid
local waypoints
local currentWaypoint

--
local messageAgent
local bookMoved = false
local bookHome

local State = {
	LAST = -1,
	IDLE = 0,
	GO_HOME = 1,
	CHECK_ON_BOOK = 2,
	GET_BOOK = 3
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
	FSMState.new("Idle",
		{ State.CHECK_ON_BOOK }, -- PRESS_BUTTON, CLOSE_DOOR

		-- on update
		function(dt)
			stateTimer = stateTimer + dt

			if stateTimer >= 15.0 then
				return State.CHECK_ON_BOOK
			end

			return State.IDLE
		end,

		-- on enter
		function()
			stateTimer = 0.0
		end
	),

	FSMState.new("Go Home",
		{ State.IDLE, State.CHECK_ON_BOOK }, -- PRESS_BUTTON, CLOSE_DOOR

		-- on update
		function(dt)
			if(atDestination) then
				return State.IDLE
			else
				TraverseAStar(dt)
			end

			return State.GO_HOME
		end,

		-- on start
		function()
			moveRotateFlag = false
			atDestination = false
			currentWaypoint = 1
			waypoints = grid:GetWaypoints(entity:GetTransformComponent().translation, teacherHomePosition, true)
		end
	),

	FSMState.new("Check On Book",
		{ State.GO_HOME, State.GET_BOOK }, -- PRESS_BUTTON, CLOSE_DOOR

		-- on update
		function(dt)
			if(atDestination) then

				-- Request book position
				if(flag == false) then
					messageAgent:SendMessageToCategory(
						AgentCategory.BOOK,
						MessageType.HOME,
						{}
					)

					flag = true
				end

				-- Position was received
				if checked then
					-- If book is home
					if wasBookedThere then
						return State.GO_HOME
					else
						return State.GET_BOOK
					end
				end
			else
				TraverseAStar(dt)
			end

			return State.CHECK_ON_BOOK
		end,

		-- on enter
		function()
			wasBookedThere = false
			checked = false
			moveRotateFlag = false
			atDestination = false
			currentWaypoint = 1
			flag = false
			waypoints = grid:GetWaypoints(entity:GetTransformComponent().translation, bookHomePosition, true)
		end
	),

	FSMState.new("Get Book",
		{ State.GO_HOME }, -- PRESS_BUTTON, CLOSE_DOOR
		-- on update
		function(dt)

			if atDestination then
				-- get waypoints only once
				if(flag == false) then
					waypoints = grid:GetWaypoints(entity:GetTransformComponent().translation, bookHomePosition, true)
					moveRotateFlag = false
					atDestination = false
					currentWaypoint = 1
					flag = true
					holdingBook = true

				else

					book:GetPhysicsBody():SetTranslation(bookHomePosition)
					book:GetPhysicsBody():SetLinearMomentum(Vec3.new(0, 0, 0))
					book:GetPhysicsBody():SetAngularMomentum(Vec3.new(0, 0, 0))
					holdingBook = false

					-- go home after returning book
					return State.GO_HOME
				end
			else
				TraverseAStar(dt)

				if holdingBook then
					book:GetPhysicsBody():SetTranslation(entity:GetPlayerControllerComponent():GetUnitDirection() * 5.0)
				end
			end

			return State.GET_BOOK
		end,

		-- on enter
		function()
			moveRotateFlag = false
			atDestination = false
			currentWaypoint = 1
			flag = false
			holdingBook = false
			waypoints = grid:GetWaypoints(entity:GetTransformComponent().translation, bookPosition, true)
		end
	)},

	-- initial state
	State.IDLE
);

----------------------------------------------------------------------------------------------------
function OnStart()
	messageAgent = MessageService.CreateAgent(entity:GetTagComponent().ident)
	messageAgent:AddToCategory(AgentCategory.TEACHER)
	messageAgent:RegisterMessageHandler(
		MessageType.HOME,
		function(msg)
			checked = true
			wasBookedThere = msg.payload.isHome
			bookPosition = msg.payload.pos
		end
	)
	fsm:Init()
end

function OnFixedUpdate(dt)

	if grid == nil then
		grid = SceneManager.GetActiveScene():GetEntity("AI_Grid"):GetNavigationGridComponent()
	end

	if book == nil then
		book = SceneManager.GetActiveScene():GetEntity("Book")
	end

	fsm:OnUpdate(dt)
end