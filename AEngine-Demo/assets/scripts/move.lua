local hp = 10

function OnStart()
	print("move.test -> OnStart")
end

function OnUpdate(dt, entt)
	transform = entt:GetTransformComponent()
	render = entt:GetRenderableComponent()

	speed = 5
	rotateStep = 45

	--xAxis movement
	if (GetKey(AEKey.UP)) then
		entt:TranslateLocal(Vec3.new(0, 0, speed * dt));
	end
	if (GetKey(AEKey.DOWN)) then
		entt:TranslateLocal(Vec3.new(0, 0, -speed * dt));
	end
	if (GetKey(AEKey.U)) then
		entt:TranslateLocal(Vec3.new(0, speed * dt, 0));
	end

	if (GetKey(AEKey.Y)) then
		entt:RotateLocal(Math.Radians(rotateStep) * dt, Vec3.new(1, 0, 0));
	end

	--zAxis Movement
	if (GetKey(AEKey.LEFT)) then
		entt:RotateLocal(Math.Radians(rotateStep) * dt, Vec3.new(0, 1, 0));
	end
	if (GetKey(AEKey.RIGHT)) then
		entt:RotateLocal(-Math.Radians(rotateStep) * dt, Vec3.new(0, 1, 0));
	end

	if (GetKeyNoRepeat(AEKey.B)) then
		hp = hp - 1
	end

	if (hp <= 0) then
		render.active = false
	end
end

function OnDestroy()
	print("move.test -> OnDestroy")
end
