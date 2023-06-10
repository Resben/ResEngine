-- player.lua
dofile("assets/scripts/messaging.lua")

-- modify these to change the behaviour of the player
local maxHealth = 25
local lookSpeed = 5.0

-- internal
local health
local messageAgent
local lookSensitivity = 0.0025
local pitch = 0.0
local yaw = 0.0
local damageCooloff = 0.0
local supplies = 0
local kills = 0
local healCooloff = 0.0

function OnStart()
	health = maxHealth
	messageAgent = MessageService.CreateAgent(entity:GetTagComponent().ident)
	messageAgent:AddToCategory(AgentCategory.PLAYER)
	messageAgent:RegisterMessageHandler(
		MessageType.DAMAGE,
		function (msg)
			if (damageCooloff <= 0.1) then
				return
			end

			-- reset damage cooloff
			damageCooloff = 0.0

			-- reduce health and print message
			health = health - msg.payload.amount
			maxHealth = health

			-- check if dead
			if (health <= 0) then
				messageAgent:SendMessageToCategory(
					AgentCategory.RUNTIME,
					MessageType.TEXT,
					Text_Data.new("You died with " .. supplies .. " supplies and " .. kills .. " kills")
				)
				messageAgent:BroadcastMessage(
					MessageType.KILLED,
					{}
				)
				messageAgent:Destroy()
				entity:Destroy()
			end
		end
	)

	messageAgent:RegisterMessageHandler(
		MessageType.KILLED,
		function (msg)
			kills = kills + 1
		end
	)

	messageAgent:RegisterMessageHandler(
		MessageType.PICKUP,
		function (msg)
			supplies = supplies + 1
		end
	)
end

function OnFixedUpdate(dt)
	if health <= 0 then
		return
	end

	local position = entity:GetTransformComponent().translation

	messageAgent:BroadcastMessage(
		MessageType.POSITION,
		Position_Data.new(Vec3.new(position))
	)

	messageAgent:SendMessageToCategory(
		AgentCategory.RUNTIME,
		MessageType.TEXT,
		Text_Data.new("Health: " .. health .. " Supplies: " .. supplies .. " Kills: " .. kills)
	)

	-- reset damage cooloff
	if (position.y < -117.50) then
		-- check for drown damage
		if (damageCooloff > 0.1) then
			damageCooloff = 0.0
			health = health - 0.5
		end
	else
		if (healCooloff > 0.1) and (health < maxHealth) then
			healCooloff = 0.0
			health = health + 0.5
		end
	end
end

function OnDestroy()
	messageAgent:Destroy()
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

	if (GetMouseButtonNoRepeat(AEMouse.LEFT)) then
		messageAgent:SendMessageToCategory(
			AgentCategory.ENEMY,
			MessageType.AREA_DAMAGE,
			AreaDamage_Data.new(5, 10, Vec3.new(entity:GetTransformComponent().translation))
		)
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

	if (GetKeyNoRepeat(AEKey.SPACE)) then
		if (supplies ~= 0) and (maxHealth <= 20) then
			supplies = supplies - 1
			maxHealth = maxHealth + 5
		end
	end

	-- update translation
	if (hasMove) then
		entity:GetPlayerControllerComponent():Move(moveVec)
	end
end

function OnUpdate(dt)
	damageCooloff = damageCooloff + dt
	healCooloff = healCooloff + dt

	-- don't control player if using debug camera
	if (Scene.UsingDebugCamera()) then
		return
	end

	UpdateOrientation(dt)
	UpdateMovement(dt)
end
