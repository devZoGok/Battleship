numGui = 9
gui = {
	{
		pos = {x = 210, y = 190},
		size = {x = 360, y = 20},
		guiType = GuiType.LISTBOX,
		numMaxDisplay = 5
	},
	{
		pos = {x = 110, y = 40},
		buttonType = ButtonType.CHECKBOX
	},
	{
		pos = {x = 110, y = 70},
		buttonType = ButtonType.CHECKBOX
	},
	{
		pos = {x = 110, y = 100},
		buttonType = ButtonType.CHECKBOX
	},
	{
		pos = {x = 110, y = 130},
		buttonType = ButtonType.CHECKBOX
	},
	{
		pos = {x = 110, y = 160},
		buttonType = ButtonType.CHECKBOX
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
