Size = {x = 150, y = 40}

numGui = 6
gui = {
	{
		pos = {x = 110, y = 40},
		size = Size,
		guiType = GuiType.BUTTON,
		name = 'Controls',
		buttonType = ButtonType.CONTROLS_TAB
	},
	{
		pos = {x = 230, y = 40},
		size = Size,
		guiType = GuiType.BUTTON,
		name = 'Mouse',
		buttonType = ButtonType.MOUSE_TAB
	},
	{
		pos = {x = 350, y = 40},
		size = Size,
		guiType = GuiType.BUTTON,
		name = 'Video',
		buttonType = ButtonType.VIDEO_TAB
	},
	{
		pos = {x = 480, y = 40},
		size = Size,
		guiType = GuiType.BUTTON,
		name = 'Audio',
		buttonType = ButtonType.AUDIO_TAB
	},
	{
		pos = {x = 590, y = 40},
		size = Size,
		guiType = GuiType.BUTTON,
		name = 'Multiplayer',
		buttonType = ButtonType.MULTIPLAYER_TAB
	},
	{
		pos = {x = 110, y = 100},
		size = Size,
		guiType = GuiType.BUTTON,
		name = 'Back',
		screen = 'mainMenu.lua',
		buttonType = ButtonType.BACK
	}
}
