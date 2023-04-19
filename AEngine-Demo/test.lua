
function OnStart()
	--print("Hello World.")

	test = GetVector()
	test.x = 4
	print(test.x)
	print(test.y)
	print(test.z)

	test2 = vec3.new(4, 4, 4)
	print(test2.x)
	print(test2.y)
	print(test2.z)
end

function OnUpdate(dt, entity)
	transform = entity:GetTransform()

	speed = 5

	--xAxis movement
	if(PollAlphaKey(AlphaKeyCode["X"])) then
		transform.translation.x = transform.translation.x + speed * dt
	end
	if(PollSpecialKey(SpecialKeyCode["RIGHT_SHIFT"])) then
		transform.translation.x = transform.translation.x - speed * dt
	end

	--yAxis Movement
	if(PollAlphaKey(AlphaKeyCode["Y"])) then
		transform.translation.y = transform.translation.y + speed * dt
	end
	if(PollSpecialKey(SpecialKeyCode["RIGHT_CONTROL"])) then
		transform.translation.y = transform.translation.y - speed * dt
	end

	--zAxis Movement
	if(PollAlphaKey(AlphaKeyCode["Z"])) then
		transform.translation.z = transform.translation.z + speed * dt
	end
	if(PollSpecialKey(SpecialKeyCode["RIGHT_ALT"])) then
		transform.translation.z = transform.translation.z - speed * dt
	end

	--print(translation.x)
end

function OnDestroy()
	print("Death to all")
end	