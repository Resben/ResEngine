-- projectile.lua
local elapsedTime = 0.0
local lifetime = 5.0

function OnStart()
	-- do nothing
end

function OnFixedUpdate(dt)
	elapsedTime = elapsedTime + dt
	if (elapsedTime > lifetime) then
		entity:Destroy()
	end
end

function OnDestroy()
	-- do nothing
end
