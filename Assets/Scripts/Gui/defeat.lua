Size = {x = 150, y = 40}

gui = {
	{
		guiType = GuiType.TEXT,
		text = 'Defeat',
		name = 'D',
		pos = {x = 100, y = 100},
		zIndex = 0,
		scale = 1,
		font = 'batang.ttf',
		fontFirstChar = 0,
		fontLastChar = 256,
		color = {x = 1, y = 1, z = 1, w = 1}
	},
	{
		guiType = GuiType.GUI_RECTANGLE,
		pos = {x = 100, y = 200},
		zIndex = 1,
		size = {x = 600, y = 600},
		color = {x = 0, y = 0, z = 0, w = 0.1}
	},
	{
		pos = {x = 110, y = 350},
		size = Size,
		guiType = GuiType.BUTTON,
		name = 'Leave',
		buttonType = ButtonType.STATISTICS,
		trigger = -1,
		imagePath = ''
	}
}
