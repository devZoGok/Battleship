res = graphics.resolution
Size = {x = 70, y = 70}
sz = 210

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
	{
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.ACTIVE_GAME_STATE,
		name = 'Trade',
		imagePath = '',
		pos = {x = res.x - Size.x, y = 200},
		size = Size,
		trigger = 10
	},
	{
		guiType = GuiType.GUI_RECTANGLE,
		pos = {x = res.x - sz, y = res.y - sz},
		zIndex = 0,
		size = {x = sz, y = sz},
		color = {x = .6, y = .6, z = .6, w = .6}
	},
	{
		guiType = GuiType.GUI_RECTANGLE,
		pos = {x = 0, y = res.y - sz},
		zIndex = 0,
		size = {x = sz, y = sz},
		color = {x = .6, y = .6, z = .6, w = .6}
	},
	{
		guiType = GuiType.GUI_RECTANGLE,
		pos = {x = sz, y = res.y - .75 * sz},
		zIndex = 0,
		size = {x = res.x - 2 * sz, y = sz * .75},
		color = {x = .6, y = .6, z = .6, w = .6}
	},
}
