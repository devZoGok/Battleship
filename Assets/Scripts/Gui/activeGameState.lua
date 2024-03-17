Size = {x = 100, y = 100}

gui = {
	{
		guiType = GuiType.TEXT,
		name = 'refineds',
		text = '',
		pos = {x = 0, y = 100},
		zIndex = 0,
		scale = 1,
		font = 'batang.ttf',
		fontFirstChar = 0,
		fontLastChar = 256,
		color = {x = 1, y = 1, z = 1, w = 1}
	},
	{
		guiType = GuiType.TEXT,
		name = 'wealth',
		text = '',
		pos = {x = 0, y = 200},
		zIndex = 0,
		scale = 1,
		font = 'batang.ttf',
		fontFirstChar = 0,
		fontLastChar = 256,
		color = {x = 1, y = 1, z = 1, w = 1}
	},
	{
		guiType = GuiType.TEXT,
		name = 'research',
		text = '',
		pos = {x = 0, y = 300},
		zIndex = 0,
		scale = 1,
		font = 'batang.ttf',
		fontFirstChar = 0,
		fontLastChar = 256,
		color = {x = 1, y = 1, z = 1, w = 1}
	},
	{
		guiType = GuiType.TEXT,
		name = 'depth',
		text = '',
		pos = {x = 0, y = 400},
		zIndex = 0,
		scale = 1,
		font = 'batang.ttf',
		fontFirstChar = 0,
		fontLastChar = 256,
		color = {x = 1, y = 1, z = 1, w = 1}
	},
	--[[
	]]--
	{
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.ACTIVE_GAME_STATE,
		name = 'Tech',
		imagePath = '',
		pos = {x = 600, y = 800},
		size = Size,
		trigger = 10
	},
	{
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.ACTIVE_GAME_STATE,
		name = 'Trade',
		imagePath = '',
		pos = {x = 300, y = 800},
		size = Size,
		trigger = 10
	}
}
