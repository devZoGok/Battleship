Size = {x = 150, y = 40}

numGui = 3
gui = {
	{
		pos = {x = 110, y = 40},
		size = Size,
		guiType = GuiType.BUTTON,
		name = 'New map',
		buttonType = ButtonType.NEW_MAP
	},
	{
		pos = {x = 110, y = 90},
		size = Size,
		guiType = GuiType.BUTTON,
		name = 'Load map',
		buttonType = ButtonType.LOAD_MAP
	},
	{
		pos = {x = 110, y = 140},
		size = Size,
		guiType = GuiType.BUTTON,
		name = 'Back',
		buttonType = ButtonType.BACK,
		screen = 'mainMenu.lua'
	}
}
