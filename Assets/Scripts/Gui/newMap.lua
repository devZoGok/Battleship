Size = {x = 140, y = 40}

numGui = 5
gui = {
	{
		pos = {x = 110, y = 150},
		size = Size,
		guiType = GuiType.TEXTBOX
	},
	{
		pos = {x = 110, y = 190},
		size = Size,
		guiType = GuiType.TEXTBOX
	},
	{
		pos = {x = 110, y = 220},
		size = Size,
		guiType = GuiType.TEXTBOX
	},
	{
		pos = {x = 420, y = 400},
		size = {x = 150, y = 50},
		guiType = GuiType.BUTTON,
		name = 'Ok',
		buttonType = ButtonType.NEW_MAP_OK,
		numDependencies = 3,
		dependencies = {
			{id = 0},
			{id = 1},
			{id = 2}
		}
	},
	{
		pos = {x = 250, y = 400},
		size = {x = 150, y = 50},
		guiType = GuiType.BUTTON,
		name = 'Back',
		buttonType = ButtonType.BACK,
		screen = 'mapEditorMenu.lua'
	}
}
