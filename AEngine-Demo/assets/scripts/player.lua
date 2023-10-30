-- player.lua
dofile("assets/scripts/messaging.lua")

-- misc
local messageAgent

-- interact
local isHolding = false
local heldEntity

-- look
local lookSpeed = 5.0
local lookSensitivity = 0.0025
local pitch = 0.0
local yaw = 0.0

function OnStart()
	messageAgent = MessageService.CreateAgent(entity:GetTagComponent().ident)
	messageAgent:RegisterMessageHandler(
		MessageType.PICKUP,
		function (msg)
			isHolding = true
			print(msg.payload.tag)
			heldEntity = SceneManager.GetActiveScene():GetEntity(msg.payload.tag)
		end
	)
end

function OnFixedUpdate(dt)
	if(isHolding) then
		print(heldEntity:GetTransformComponent().translation.x)
		heldEntity:GetPhysicsBody():SetTranslation(entity:GetTransformComponent().translation)
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

	if (IsKeyDown(AEKey.W)) then
		moveVec = moveVec + AEMath.RotateVec(Vec3.new(0.0, 0.0, -1.0), entity:GetTransformComponent().orientation)
		hasMove = true
	end

	if (IsKeyDown(AEKey.S)) then
		moveVec = moveVec + AEMath.RotateVec(Vec3.new(0.0, 0.0, 1.0), entity:GetTransformComponent().orientation)
		hasMove = true
	end

	if (IsKeyDown(AEKey.A)) then
		moveVec = moveVec + AEMath.RotateVec(Vec3.new(-1.0, 0.0, 0.0), entity:GetTransformComponent().orientation)
		hasMove = true
	end

	if (IsKeyDown(AEKey.D)) then
		moveVec = moveVec + AEMath.RotateVec(Vec3.new(1.0, 0.0, 0.0), entity:GetTransformComponent().orientation)
		hasMove = true
	end

	if (GetKey(AEKey.E) == AEInput.Pressed) then
		if(isHolding == false) then
			messageAgent:SendMessageToCategory(
				AgentCategory.BOOK,
				MessageType.POSITION,
				Position_Data.new(entity:GetTransformComponent().translation)
			)
		end
	end

	if (GetKey(AEKey.E) == AEInput.Released) then
		if(isHolding) then
			print("Released")
			isHolding = false
			heldEntity = nil
		end
	end

	-- update translation
	if (hasMove) then
		entity:GetPlayerControllerComponent():Move(moveVec)
	end
end

function OnUpdate(dt)
	-- don't control player if using debug camera
	if (Scene.UsingDebugCamera()) then
		return
	end

	UpdateOrientation(dt)
	UpdateMovement(dt)
end
