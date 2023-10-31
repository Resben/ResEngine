AgentCategory = {
	TEACHER = 0,
	PLAYER = 1,
	BOOK = 2,
	BUTTON = 3,
	RUNTIME = 4
}

MessageType = {
	-- Informational
	POSITION = 0,
	TEXT = 1, 
	HOME = 2,

	-- Affordances, with UI elements
	INTERACTABLE = 3,
	PRESSABLE = 4,

	-- Affordances
	PICKUP = 5,
	PRESSED = 6,
	
	-- Button Informational
	BUTTON_HAPPY = 7,
	BUTTON_JEALOUS = 8,
	BUTTON_DEPRESSED = 9,

	-- Button Affordances
	BUTTON_PRESSED = 10,
	BUTTON_CHEER_UP = 11
}

Position_Data = {}
function Position_Data.new(pos)
	local instance = {}
    instance.pos = pos
	return instance
end

Pickup_Data = {}
function Pickup_Data.new(tag)
	local instance = {}
    instance.tag = tag
	return instance
end

Home_Data = {}
function Home_Data.new(isTrue, position)
	local instance = {}
	instance.isHome = isTrue
	instance.pos = position
	return instance
end