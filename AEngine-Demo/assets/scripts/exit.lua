local open = false

-- Probably better to have game states
function OnUpdate(dt)
    render = entity:GetCanvasRendererComponent();

    -- Open exit screen
	if (GetKey(AEKey.ESCAPE) == AEInput.Pressed) then
        render.active = true;
        open = true;
	end

    -- Quit the program
    if (GetKey(AEKey.ENTER) == AEInput.Pressed) then
        if (open) then
            Application.Terminate();
        end
    end

    -- Close exit screen
    if (GetKey(AEKey.BACKSPACE) == AEInput.Pressed) then
        if (open) then
            render.active = false;
            open = false;
        end
    end
end