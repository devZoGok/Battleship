numGui = 5

Size = {x = 140, y = 20}
startX = 100
gui = {
	{
		pos = {x = startX, y = 400},
		size = Size,
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.EXPORT,
		name = 'Export'
	},
	{
		pos = {x = startX + Size.x + 10, y = 400},
		size = Size,
		guiType = GuiType.LISTBOX,
		listboxType = ListboxType.UNITS,
		vehicles = true,
		numMaxDisplay = 5
	},
	{
		pos = {x = startX + 2 * (Size.x + 10), y = 400},
		size = Size,
		guiType = GuiType.LISTBOX,
		listboxType = ListboxType.UNITS,
		vehicles = false,
		numMaxDisplay = 5
	},
	{
		pos = {x = startX + 3 * (Size.x + 10), y = 400},
		size = Size,
		guiType = GuiType.LISTBOX,
		listboxType = ListboxType.SKYBOX_TEXTURES,
		numMaxDisplay = 5
	},
	{
		pos = {x = startX + 4 * (Size.x + 10), y = 400},
		size = Size,
		guiType = GuiType.LISTBOX,
		listboxType = ListboxType.LAND_TEXTURES,
		numMaxDisplay = 5
	}
}