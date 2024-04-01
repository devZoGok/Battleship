res = graphics.resolution
Pos = {x = 10, y = 10}
Size = {x = res.x * .8, y = res.y * .8}
buttonSize = {x = 100, y = 20}
textboxSize = {x = 200, y = 25}
margin = {top = 10, left = 10, right = 10, bottom = 10}

gui = {
	{
		guiType = GuiType.GUI_RECTANGLE,
		pos = Pos,
		zIndex = .9,
		size = Size,
		color = {x = 0, y = 0, z = 0, w = .6}
	},
	{
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.ACTIVE_GAME_STATE,
		name = 'Back',
		guiScreen = 'activeGameState.lua',
		pos = {x = Pos.x + Size.x - buttonSize.x - margin.right, y = Pos.y + Size.y - buttonSize.y - margin.bottom},
		size = buttonSize,
		trigger = 10
	},
}
