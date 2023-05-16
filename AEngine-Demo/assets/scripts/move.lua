local hp = 10

function OnStart()
	print("move.test -> OnStart")
end

function OnUpdate(dt, entity)
	transform = entity:GetTransform()
	render = entity:GetRenderable()

	speed = 5
	rotateStep = 45

	--xAxis movement
	if (GetKey(AEKey.UP)) then
		entity:TranslateLocal(vec3.new(0, 0, speed * dt));
	end
	if (GetKey(AEKey.DOWN)) then
		entity:TranslateLocal(vec3.new(0, 0, -speed * dt));
	end
	if (GetKey(AEKey.U)) then
		entity:TranslateLocal(vec3.new(0, speed * dt, 0));
	end

	if (GetKey(AEKey.Y)) then
		entity:RotateLocal(Math.Radians(rotateStep) * dt, vec3.new(1, 0, 0));
	end

	--zAxis Movement
	if (GetKey(AEKey.LEFT)) then
		entity:RotateLocal(Math.Radians(rotateStep) * dt, vec3.new(0, 1, 0));
	end
	if (GetKey(AEKey.RIGHT)) then
		entity:RotateLocal(-Math.Radians(rotateStep) * dt, vec3.new(0, 1, 0));
	end

	if (GetKeyNoRepeat(AEKey.SPACE)) then
		hp = hp - 1
	end

	if (hp <= 0) then
		render.active = false
	end

	if (GetKeyNoRepeat(AEKey.ESCAPE)) then
		app:Terminate()
	end
end

function OnDestroy()
	print("move.test -> OnDestroy")
end
