numGui = 9
gui = {
	{
		pos = {x = 210, y = 190, z = 0},
		size = {x = 360, y = 20},
		guiType = GuiType.LISTBOX,
		listboxType = ListboxType.RESOLUTION,
		numMaxDisplay = 5,
		numLines = 3,
		lines = {
			'640 x 480',
			'800 x 600',
			'1920 x 1080'
		}
	},
	{
		pos = {x = 110, y = 40, z = 0},
		guiType = GuiType.CHECKBOX
	},
	{
		pos = {x = 110, y = 70, z = 0},
		guiType = GuiType.CHECKBOX
	},
	{
		pos = {x = 110, y = 100, z = 0},
		guiType = GuiType.CHECKBOX
	},
	{
		pos = {x = 110, y = 130, z = 0},
		guiType = GuiType.CHECKBOX
	},
	{
		pos = {x = 110, y = 160, z = 0},
		guiType = GuiType.CHECKBOX
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
