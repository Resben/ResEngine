local hp = 1000

function OnStart()
	print("move.test -> OnStart")
end

function OnUpdate(dt, entity)
	transform = entity:GetTransform()
	render = entity:GetRenderable()

	speed = 5
	rotateStep = 45

	--xAxis movement
	if (PollKey(AEKey.UP)) then
		transform.translation = transform.translation + vec3.new(0, 0, speed * dt):Rotate(transform.orientation)
		-- entity:TranslateLocal(vec3.new(0, 0, speed * dt));
	end
	if (PollKey(AEKey.DOWN)) then
		entity:TranslateLocal(vec3.new(0, 0, -speed * dt));
	end
	if (PollKey(AEKey.U)) then
		entity:TranslateLocal(vec3.new(0, speed * dt, 0));
	end

	if (PollKey(AEKey.Y)) then
		entity:RotateLocal(Math.Radians(rotateStep) * dt, vec3.new(1, 0, 0));
	end
	
	--zAxis Movement
	if (PollKey(AEKey.LEFT)) then
		entity:RotateLocal(Math.Radians(rotateStep) * dt, vec3.new(0, 1, 0));
	end
	if (PollKey(AEKey.RIGHT)) then
		entity:RotateLocal(-Math.Radians(rotateStep) * dt, vec3.new(0, 1, 0));
	end

	if (PollKey(AEKey.SPACE)) then
		hp = hp - 1
	end
	
	if (hp <= 0) then
		render.active = false
	end

	if (PollKey(AEKey.ESCAPE)) then
		app:Terminate()
	end
end

function OnDestroy()
	print("move.test -> OnDestroy")
end
