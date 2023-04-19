
function OnStart()
	print("Hello World.")
end

function OnUpdate(dt)
	--print("updating everyframe")
	
	if PollMouseButton(MouseButton["Left"]) then
		print("Left Mouse was pressed")
	end

	if(PollAlphaKey(AlphaKeyCode["K"])) then
		print("K was pressed")
	end

	if(PollNumKey(NumKeyCode["NUM1"])) then
		print("Number 1 pressed")
	end

	if(PollSpecialKey(SpecialKeyCode["SPACE"])) then
		print("Space was pressed")
	end
	
end

function OnDestroy()
	print("Death to all")
end	