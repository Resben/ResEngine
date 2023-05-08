
function OnStart()
	print("move.test -> OnStart")
	test1 = vec2.new(1.5, 1.5)
	test1 = test1 * vec2.new(1.5, 2.5)
	print(test1.x .. " " .. test1.y)
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