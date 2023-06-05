
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

Damage_Data = {}
function Damage_Data.new(amount)
	local instance = {}
	instance.amount = amount
	return instance
end

Spotted_Data = {}
function Spotted_Data.new(targetPos, spotterPos)
	local instance = {}
	instance.targetPos = targetPos
	instance.spotterPos = spotterPos
	return instance
end
