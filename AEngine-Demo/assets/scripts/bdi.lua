-- bdi.lua
local bdiAgent
local fcm

local Concepts = {
	Happy = 0,
	Stressed = 1,
	Relaxed = 2,
	Work = 3
}

function OnStart()
	local bdiComp = entity:GetBDIComponent()
	bdiAgent = bdiComp:GetAgent()

	local fcmComp = entity:GetFCMComponent()
	fcm = fcmComp:GetFCM()

	-- Setup FCM
	fcm:AddNode(
		"happy",
		0.6,
		0.5,
		function(value)
			print("I am happy!")
		end,
		function(value)
			print("I am not happy anymore...")
		end
	)

	fcm:AddNode(
		"stressed",
		0.05,
		0.8,
		function(value)
			print("I am stressed!")
		end,
		function(value)
			print("I am not stressed anymore...")
		end
	)

	fcm:AddNode(
		"relaxed",
		0.2,
		0.95,
		function(value)
			print("I am relaxed!")
		end,
		function(value)
			print("I am not relaxed anymore...")
		end
	)

	fcm:AddNode(
		"work",
		0.0, 
		0.5,
		function(value)
			print("I am working!")
		end,
		function(value)
			print("I am not working anymore...")
		end
	)

	fcm:AddEdge(
		Concepts.Work,
		Concepts.Happy,
		-0.05
	)

	fcm:AddEdge(
		Concepts.Work,
		Concepts.Stressed,
		0.05
	)

	fcm:AddEdge(
		Concepts.Stressed,
		Concepts.Happy,
		-0.1
	)

	fcm:AddEdge(
		Concepts.Stressed,
		Concepts.Relaxed,
		-0.10
	)

	fcm:AddEdge(
		Concepts.Relaxed,
		Concepts.Happy,
		0.1
	)

	fcm:Init()

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
	fcm:SetConceptValue(Concepts.Work, fcm:GetConceptValue(Concepts.Work) + 0.05)
	fcm:OnUpdate()
end

function OnUpdate(dt)
	if (GetKey(AEKey.SPACE) == AEInput.Pressed) then
		bdiAgent:AddBelief("there_is_a_god")
	end
end

function OnDestroy()
	-- do nothing
end
