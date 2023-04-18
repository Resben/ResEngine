
function OnStart()
	print("Hello World.")
end

function OnUpdate(dt)
	print("updating everyframe")
	local speed = 3
	x = speed * dt
	print(x)
end

function OnDestroy()
	print("Death to all")
end	