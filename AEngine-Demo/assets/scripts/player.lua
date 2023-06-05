
local lookSensitivity = 0.0025
local lookSpeed = 5.0
local pitch = 0.0
local yaw = 0.0

function OnStart()
	print("player.lua -> OnStart()")
end

function OnUpdate(dt)
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
		-- local moveNormalized = AEMath.Normalize(moveVec)
		-- local moveStep = moveNormalized * moveSpeed * dt
		-- if (GetKey(AEKey.LEFT_SHIFT)) then
		-- 	moveStep = moveStep * sprintMod
		-- end
		-- entity:GetTransformComponent().translation = entity:GetTransformComponent().translation + moveStep
	end
end
