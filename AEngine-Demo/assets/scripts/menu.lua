-- Probably better to have game states
function OnUpdate(dt)
    render = entity:GetCanvasRendererComponent();

	if (GetKey(AEKey.Q) == AEInput.Pressed) then
        render.active = false;
	end
end