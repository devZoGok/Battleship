res = graphics.resolution
Pos = {x = 10, y = 10}
Size = {x = res.x * .8, y = res.y * .8}
buttonSize = {x = 100, y = 20}
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
		guiType = GuiType.TEXT,
		name = 'P0',
		text = 'P0',
		pos = {x = Pos.x + margin.left, y = Pos.y + margin.top},
		zIndex = 0,
		scale = 1,
		font = 'batang.ttf',
		fontFirstChar = 0,
		fontLastChar = 256,
		color = {x = 1, y = 1, z = 1, w = 1}
	},
	{
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.ACTIVE_GAME_STATE,
		name = 'Trade',
		imagePath = '',
		guiScreen = 'tradingScreen.lua',
		pos = {x = Pos.x + Size.x - buttonSize.x - margin.right, y = Pos.y + buttonSize.y + margin.top},
		size = buttonSize,
		trigger = 10
	},
	{
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.ACTIVE_GAME_STATE,
		name = 'Back',
		imagePath = '',
		guiScreen = 'activeGameState.lua',
		pos = {x = Pos.x + Size.x - buttonSize.x - margin.right, y = Pos.y + Size.y - buttonSize.y - margin.bottom},
		size = buttonSize,
		trigger = 10
	},
}
