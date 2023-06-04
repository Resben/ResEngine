dofile("assets/scripts/messaging.lua")

-- modify these to change the behaviour of the player
local health = 25
local lookSpeed = 5.0

-- internal
local messageAgent
local lookSensitivity = 0.0025
local pitch = 0.0
local yaw = 0.0
local damageCooloff = 0.0

function OnStart()
	print("player.lua -> OnStart()")
	messageAgent = MessageService.CreateAgent(entity:GetTagComponent().ident)
	messageAgent:AddToCategory(AgentCategory.PLAYER)
	messageAgent:RegisterHandler(
		MessageType.SPOTTED,
		function(msg)
			-- print(entity:GetTagComponent().tag .. " has been spotted by " .. SceneManager.GetActiveScene():GetEntity(msg.sender):GetTagComponent().tag)
			print(entity:GetTagComponent().tag .. " has been spotted by " .. entity:GetScene():GetEntity(msg.sender):GetTagComponent().tag)
		end
	)

	messageAgent:RegisterHandler(
		MessageType.DAMAGE,
		function (msg)
			if (damageCooloff <= 0.1) then
				return
			end

			-- reset damage cooloff
			damageCooloff = 0.0

			-- reduce health and print message
			health = health - msg.data.amount
			-- print(entity:GetTagComponent().tag .. " has taken " .. msg.data.amount .. " damage from " .. SceneManager.GetActiveScene():GetEntity(msg.sender):GetTagComponent().tag)
			print(entity:GetTagComponent().tag .. " has taken " .. msg.data.amount .. " damage from " .. entity:GetScene():GetEntity(msg.sender):GetTagComponent().tag)

			-- check if dead
			if (health <= 0) then
				-- print(entity:GetTagComponent().tag .. " has been mauled to death by " .. SceneManager.GetActiveScene():GetEntity(msg.sender):GetTagComponent().tag .. "!")
				print(entity:GetTagComponent().tag .. " has been mauled to death by " .. entity:GetScene():GetEntity(msg.sender):GetTagComponent().tag .. "!")
				entity:Destroy()
				return
			end
		end
	)
end

function OnFixedUpdate(dt)
	messageAgent:SendMessageToCategory(
		AgentCategory.ENEMY,
		MessageType.POSITION,
		Position_Data.new(entity:GetTransformComponent().translation)
	)
end

----------------------------------------------------------------------------------------------------
local function UpdateOrientation(dt)
	local lookStep = lookSensitivity * lookSpeed
	local mouseDelta = GetMouseDelta()

	-- update pitch and yaw
	pitch = pitch - (lookStep * mouseDelta.y)
	yaw = yaw - (lookStep * mouseDelta.x)

	-- clamp pitch
	pitch = AEMath.Clamp(pitch, -89.0, 89.0)

	-- clamp yaw
	if (yaw >= 360.0) then
		yaw = 0
	elseif (yaw <= -360.0) then
		yaw = 0
	end

	-- generate orientation quaternion
	local orientation = Quat.new(1.0, 0.0, 0.0, 0.0)
	orientation = AEMath.Rotate(orientation, math.rad(yaw), Vec3.new(0.0, 1.0, 0.0))
	orientation = AEMath.Rotate(orientation, math.rad(pitch), Vec3.new(1.0, 0.0, 0.0))

	-- update orientation
	entity:GetTransformComponent().orientation = AEMath.Normalize(orientation)
end

local function UpdateMovement(dt)
	-- move entity
	local moveVec = Vec3.new(0.0, 0.0, 0.0)
	local hasMove = false

	if (GetKey(AEKey.W)) then
		moveVec = moveVec + AEMath.RotateVec(Vec3.new(0.0, 0.0, -1.0), entity:GetTransformComponent().orientation)
		hasMove = true
	end

	if (GetKey(AEKey.S)) then
		moveVec = moveVec + AEMath.RotateVec(Vec3.new(0.0, 0.0, 1.0), entity:GetTransformComponent().orientation)
		hasMove = true
	end

	if (GetKey(AEKey.A)) then
		moveVec = moveVec + AEMath.RotateVec(Vec3.new(-1.0, 0.0, 0.0), entity:GetTransformComponent().orientation)
		hasMove = true
	end

	if (GetKey(AEKey.D)) then
		moveVec = moveVec + AEMath.RotateVec(Vec3.new(1.0, 0.0, 0.0), entity:GetTransformComponent().orientation)
		hasMove = true
	end

	-- update translation
	if (hasMove) then
		entity:GetPlayerControllerComponent():Move(moveVec)
	end
end

function OnUpdate(dt)
	damageCooloff = damageCooloff + dt
	UpdateOrientation(dt)
	UpdateMovement(dt)
end
