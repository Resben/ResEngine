
function OnStart()
	print("Hello World.")
end

function OnUpdate(dt)
	--print("updating everyframe")
	
	if PollInput(MouseButton["Left"]) then
		print("Something was pressed")
	end
end

function OnDestroy()
	print("Death to all")
end	