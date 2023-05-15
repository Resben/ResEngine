
function OnStart()
	print("move.test -> OnStart")
end

function OnUpdate(dt, entity)
	transform = entity:GetTransform()
	render = entity:GetRenderable()

	speed = 5

	--xAxis movement
	if (PollKey(AEKey.UP)) then
		transform.translation.x = transform.translation.x + speed * dt
	end
	if (PollKey(AEKey.DOWN)) then
		transform.translation.x = transform.translation.x - speed * dt
	end
	
	--zAxis Movement
	if (PollKey(AEKey.LEFT)) then
		transform.translation.z = transform.translation.z + speed * dt
	end
	if (PollKey(AEKey.RIGHT)) then
		transform.translation.z = transform.translation.z - speed * dt
	end

	if (PollKey(AEKey.ESCAPE)) then
		app:Terminate()
	end
end

function OnDestroy()
	print("move.test -> OnDestroy")
end	