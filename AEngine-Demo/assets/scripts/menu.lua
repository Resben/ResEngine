-- Probably better to have game states
function OnUpdate(dt)
    render = entity:GetCanvasRendererComponent();

	if (GetKeyNoRepeat(AEKey.Q)) then
        render.active = false;
	end
end