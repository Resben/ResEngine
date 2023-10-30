AgentCategory = {
	TEACHER = 0,
	PLAYER = 1,
	BOOK = 2,
	RUNTIME = 3
}

MessageType = {
	POSITION = 0,
	PICKUP = 1,
    INTERACTABLE = 2,
	TEXT = 3,
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