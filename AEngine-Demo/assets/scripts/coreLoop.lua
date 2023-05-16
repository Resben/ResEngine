
function OnStart()
	print("Core Loop -> OnStart")
end

function OnUpdate(dt, entity)
	local scene = SceneManager.GetActiveScene();

	if (GetKeyNoRepeat(AEKey.ESCAPE)) then
		Application:Terminate()
	end

	if (GetKeyNoRepeat(AEKey.F7)) then
		SceneManager.SetActiveScene("PhysicsTest")
	end

	if (GetKeyNoRepeat(AEKey.F8)) then
		local scenes = SceneManager.GetSceneIdents()
		print ("Number of scenes = " .. scenes:Size())
		for i = 1, scenes:Size(), 1 do
			print(scenes[i])
		end
	end

	if (GetKeyNoRepeat(AEKey.F9)) then
		if (scene:UsingDebugCamera()) then
			scene:UseDebugCamera(false)
		else
			scene:UseDebugCamera(true)
		end
	end


end

function OnDestroy()
	print("Core Loop -> OnDestroy")
end
