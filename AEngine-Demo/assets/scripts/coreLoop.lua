local row = 0

function OnStart()
	print("Core Loop -> OnStart")
end

function OnUpdate(dt)
	local scene = SceneManager.GetActiveScene();

	if (GetKeyNoRepeat(AEKey.ESCAPE)) then
		Application:Terminate()
	end

	if (GetKeyNoRepeat(AEKey.F7)) then
		SceneManager.SetActiveScene("Physics Test Scene")
	end

	if (GetKeyNoRepeat(AEKey.F8)) then
		local scenes = SceneManager.GetSceneIdents()
		print ("Number of scenes = " .. scenes:Size())
		for i = 1, scenes:Size(), 1 do
			print(scenes[i])
		end
	end

	if (GetKeyNoRepeat(AEKey.F9)) then
		if (Scene.UsingDebugCamera()) then
			Scene.UseDebugCamera(false)
		else
			Scene.UseDebugCamera(true)
		end
	end

	if (GetKeyNoRepeat(AEKey.EQUAL)) then
		local fov = Scene.GetDebugCamera():GetFov()
		Scene.GetDebugCamera():SetFov(fov + 5.0)
	end
	if (GetKeyNoRepeat(AEKey.MINUS)) then
		local fov = Scene.GetDebugCamera():GetFov()
		Scene.GetDebugCamera():SetFov(fov - 5.0)
	end

	if (GetMouseButton(AEMouse.LEFT)) then
		for i = 0, 9, 1 do
			local scene = SceneManager.GetActiveScene()
			local entt = scene:CreateEntity("fromLua" .. row .. i)
			local transformComp = entt:AddTransformComponent()
			transformComp.translation = Vec3.new(0.0 + (row * 10), 10.0, 0.0 + (i * 10))
			transformComp.scale = Vec3.new(2.0, 2.0, 2.0)

			local renderCompOther = scene:GetEntity("Enemy1"):GetRenderableComponent()
			local renderComp = entt:AddRenderableComponent()
			renderComp.active = true
			renderComp:SetModel("enemy.obj")
			renderComp:SetShader("simple.shader")
		end
		row = row + 1
	end
end

function OnDestroy()
	print("Core Loop -> OnDestroy")
end
