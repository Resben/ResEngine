
function OnStart()
	print("move.test -> OnStart")
end

function OnUpdate(dt, entity)
	transform = entity:GetTransform()
	render = entity:GetRenderable()

	speed = 5

	--xAxis movement
	if(PollAlphaKey(SpecialKeyCode["UP"])) then
		transform.translation.x = transform.translation.x + speed * dt
	end
	if(PollSpecialKey(SpecialKeyCode["DOWN"])) then
		transform.translation.x = transform.translation.x - speed * dt
	end

	--zAxis Movement
	if(PollAlphaKey(SpecialKeyCode["LEFT"])) then
		transform.translation.z = transform.translation.z + speed * dt
	end
	if(PollSpecialKey(SpecialKeyCode["RIGHT"])) then
		transform.translation.z = transform.translation.z - speed * dt
	end
end

function OnDestroy()
	print("move.test -> OnDestroy")
end	