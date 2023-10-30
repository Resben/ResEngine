-- bdi.lua
local bdiAgent
local bdiComp

function OnStart()
	bdiComp = entity:GetBDIComponent()
	bdiAgent = bdiComp:GetAgent()

	-- bdiAgent:AddBelief("there_is_a_god")
	bdiAgent:AddBelief("i_wont_get_fat")

	bdiAgent:AddDesire(
		"to_live_forever",
		"BELIEF(there_is_a_god)",
		0.5
	)

	bdiAgent:AddDesire(
		"eat_lots_of_food",
		"BELIEF(i_wont_get_fat)",
		0.2
	)

	bdiAgent:AddIntention(
		"thrive",
		[[
			AND(
				DESIRE(to_live_forever),
				DESIRE(eat_lots_of_food)
			)
		]],
		function(str)
			-- do nothing
		end
	)
end

function OnFixedUpdate(dt)
	bdiAgent:OnUpdate()
end

function OnUpdate(dt)
	if (GetKey(AEKey.SPACE) == AEInput.Pressed) then
		bdiAgent:AddBelief("there_is_a_god")
	end
end

function OnDestroy()
	-- do nothing
end
