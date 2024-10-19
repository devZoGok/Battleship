numGui = 6
gui = {
	{
		pos = {x = 110, y = 40, z = 0},
		size = {x = 100, y = 20},
		guiType = GuiType.TEXTBOX
	},
	{
		pos = {x = 110, y = 70, z = 0},
		size = {x = 100, y = 20},
		guiType = GuiType.TEXTBOX
	},
	{
		pos = {x = 110, y = 100, z = 0},
		size = {x = 100, y = 20},
		guiType = GuiType.TEXTBOX
	},
	{
		pos = {x = 100, y = 400, z = 0},
		size = {x = 150, y = 20},
		guiType = GuiType.BUTTON,
		name = 'Ok',
		buttonType = ButtonType.OK
	},
	{
		pos = {x = 260, y = 400, z = 0},
		size = {x = 150, y = 20},
		guiType = GuiType.BUTTON,
		name = 'Restore defaults',
		buttonType = ButtonType.DEFAULTS
	},
	{
		pos = {x = 420, y = 400, z = 0},
		size = {x = 150, y = 20},
		guiType = GuiType.BUTTON,
		name = 'Back',
		screen = 'options.lua',
		buttonType = ButtonType.BACK
	}
}
