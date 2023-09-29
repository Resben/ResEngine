local open = false

-- Probably better to have game states
function OnUpdate(dt)
    render = entity:GetCanvasRendererComponent();

    -- Open exit screen
	if (GetKeyNoRepeat(AEKey.ESCAPE)) then
        render.active = true;
        open = true;
	end

    -- Quit the program
    if (GetKeyNoRepeat(AEKey.ENTER)) then
        if (open) then
            Application.Terminate();
        end
    end

    -- Close exit screen
    if (GetKeyNoRepeat(AEKey.BACKSPACE)) then
        if (open) then
            render.active = false;
            open = false;
        end
    end
end