
AgentCategory = {
	ENEMY = 0,
	PLAYER = 1
}

MessageType = {
	POSITION = 0,
	DAMAGE = 1,
	SPOTTED = 2
}

Position_Data = {}
function Position_Data.new(pos)
	local instance = {}
	instance.pos = pos
	return instance
end
