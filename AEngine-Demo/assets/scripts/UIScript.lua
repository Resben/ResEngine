function OnStart()


end

-- Probably better to have game states
function OnUpdate(dt)
    render = entity:GetCanvasRendererComponent();

	if (GetKeyNoRepeat(AEKey.P)) then
		if(render.active == true) then
            render.active = false
        else
            render.active = true;
        end
	end
end