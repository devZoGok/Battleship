numGui = 6
gui = {
	{
		pos = {x = 110, y = 40},
		size = {x = 300, y = 10},
		minVal = 0,
		maxVal = 2,
		guiType = GuiType.SLIDER
	},
	{
		pos = {x = 110, y = 80},
		size = {x = 300, y = 10},
		guiType = GuiType.TEXTBOX
	},
	{
		pos = {x = 110, y = 110},
		guiType = GuiType.CHECKBOX
	},
	{
		pos = {x = 100, y = 400},
		size = {x = 150, y = 20},
		guiType = GuiType.BUTTON,
		name = 'Ok',
		buttonType = ButtonType.OK
	},
	{
		pos = {x = 260, y = 400},
		size = {x = 150, y = 20},
		guiType = GuiType.BUTTON,
		name = 'Restore defaults',
		buttonType = ButtonType.DEFAULTS
	},
	{
		pos = {x = 420, y = 400},
		size = {x = 150, y = 20},
		guiType = GuiType.BUTTON,
		name = 'Back',
		screen = 'options.lua',
		buttonType = ButtonType.BACK
	}
}