-- bdi.lua
local bdiAgent
local fcm

local Concepts = {
	Happy = 0,
	Stressed = 1,
	Relaxed = 2,
	Alcohol = 3
}

function OnStart()
	local bdiComp = entity:GetBDIComponent()
	bdiAgent = bdiComp:GetAgent()

	local fcmComp = entity:GetFCMComponent()
	fcm = fcmComp:GetFCM()

	-- Setup FCM
	fcm:AddNode(
		"happy",
		0.05,
		0.7,
		function(value)
			print("I am happy!")
		end,
		function(value)
			print("I am not happy anymore...")
		end
	)

	fcm:AddNode(
		"stressed",
		0.7,
		0.6,
		function(value)
			print("I am stressed!")
		end,
		function(value)
			print("I am not stressed anymore...")
		end
	)
	
	fcm:AddNode(
		"relaxed",
		0.0,
		0.70,
		function(value)
			print("I am relaxed!")
		end,
		function(value)
			print("I am not relaxed anymore...")
		end
	)

	fcm:AddNode(
		"alcohol",
		0.0,
		0.1,
		function(value)
			print("I am drunk!")
		end,
		function(value)
			print("Hungover...")
		end
	)

	fcm:AddEdge(
		Concepts.Stressed,
		Concepts.Happy,
		-0.80
	)

	fcm:AddEdge(
		Concepts.Relaxed,
		Concepts.Stressed,
		-0.10
	)

	fcm:AddEdge(
		Concepts.Relaxed,
		Concepts.Happy,
		0.25
	)

	fcm:AddEdge(
		Concepts.Stressed,
		Concepts.Relaxed,
		-0.10
	)
	
	fcm:AddEdge(
		Concepts.Alcohol,
		Concepts.Relaxed,
		0.25
	)

	fcm:AddEdge(
		Concepts.Alcohol,
		Concepts.Happy,
		0.25
	)
	
	fcm:AddEdge(
		Concepts.Alcohol,
		Concepts.Stressed,
		-0.25
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
	fcm:OnUpdate()
	if (IsKeyDown(AEKey.C)) then
		fcm:SetConceptValue(Concepts.Alcohol, fcm:GetConceptValue(Concepts.Alcohol) + 0.05)
	end
	if (IsKeyDown(AEKey.V)) then
		fcm:SetConceptValue(Concepts.Alcohol, fcm:GetConceptValue(Concepts.Alcohol) - 0.05)
	end
end

function OnUpdate(dt)
	-- do nothing
end

function OnDestroy()
	-- do nothing
end
