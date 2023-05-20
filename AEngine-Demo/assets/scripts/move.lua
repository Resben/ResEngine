local hp = 10
local timer = 0.0
local speed = 0.5

function OnStart()
	print("move.test -> OnStart")
end

function OnUpdate(dt, entt)
	timer = timer + dt
	if (timer > 2.5 ) then
		print("Speed up!")
		speed = speed + 0.5
		timer = timer - 2.5
	end

	transform = entt:GetTransformComponent()
	render = entt:GetRenderableComponent()

	local pos = SceneManager.GetActiveScene():GetDebugCamera():GetPosition()
	transform:LookAt(pos)
	entt:TranslateLocal(Vec3.new(0, 0, speed * dt));

	if (GetKeyNoRepeat(AEKey.B)) then
		hp = hp - 1
	end

	if (hp <= 0) then
		render.active = false
	end
end

function OnDestroy()
	print("move.test -> OnDestroy")
end
