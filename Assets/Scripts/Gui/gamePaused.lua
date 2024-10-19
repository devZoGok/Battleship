Size = {x = 150, y = 50}
numGui = 5
gui = {
	{
		pos = {x = 100, y = 100, z = 0},
		size = Size,
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.RESUME,
		name = 'Resume'
	},
	{
		pos = {x = 100, y = 100 + 1 * (Size.y + 10), z = 0},
		size = Size,
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.CONSOLE,
		name = 'Console'
	},
	{
		pos = {x = 100, y = 100 + 2 * (Size.y + 10), z = 0},
		size = Size,
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.OPTIONS,
		name = 'Options'
	},
	{
		pos = {x = 100, y = 100 + 3 * (Size.y + 10), z = 0},
		size = Size,
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.MAIN_MENU,
		name = 'Main menu'
	},
	{
		pos = {x = 100, y = 100 + 4 * (Size.y + 10), z = 0},
		size = Size,
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.EXIT,
		name = 'Exit'
	}
}
