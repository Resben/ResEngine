
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
		transform.translation = transform.translation + Math.RotateVec(vec3.new(0, 0, speed * dt), transform.rotation)
	end
	if (PollKey(AEKey.DOWN)) then
		transform.translation = transform.translation + Math.RotateVec(vec3.new(0, 0, -(speed * dt)), transform.rotation)
	end
	
	--zAxis Movement
	if (PollKey(AEKey.LEFT)) then
		transform.rotation = Math.Rotate(transform.rotation, Math.Radians(rotateStep) * dt, vec3.new(0, 1, 0));
	end
	if (PollKey(AEKey.RIGHT)) then
		transform.rotation = Math.Rotate(transform.rotation, -Math.Radians(rotateStep) * dt, vec3.new(0, 1, 0));
	end

	if (PollKey(AEKey.ESCAPE)) then
		app:Terminate()
	end
end

function OnDestroy()
	print("move.test -> OnDestroy")
end
