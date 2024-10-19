numGui = 3
gui = {
	{
		pos = {x = 100, y = 100, z = 0},
		size = {x = 300, y = 20},
		guiType = GuiType.LISTBOX,
		listboxType = ListboxType.CONSOLE,
		numMaxDisplay = 10,
		closable = false
	},
	{
		pos = {x = 100, y = 510, z = 0},
		size = {x = 270, y = 20},
		guiType = GuiType.TEXTBOX
	},
	{
		pos = {x = 380, y = 510, z = 0},
		size = {x = 20, y = 20},
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.CONSOLE_COMMAND_OK,
		name = 'OK',
		numDependencies = 2,
		dependencies = {
			{id = 0},
			{id = 1}
		}
	}
}
