numGui = 5
res = graphics.resolution
Size = {x = 100, y = 100}

gui = {
	{
		pos = {x = res.x - 200, y = res.y - 200},
		size = Size,
		name = 'Land factory',
		imagePath = '',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.BUILD,
		trigger = 75,
		structureId = 8
	},
	{
		pos = {x = res.x - Size.x - 200, y = res.y - 200},
		size = Size,
		name = 'Naval factory',
		imagePath = '',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.BUILD,
		trigger = 78,
		structureId = 9
	},
	{
		pos = {x = res.x - 2 * Size.x - 200, y = res.y - 200},
		size = {x = 100, y = 100},
		name = 'Market',
		imagePath = '',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.BUILD,
		trigger = 75,
		structureId = 10
	},
	{
		pos = {x = res.x - 3 * Size.x - 200, y = res.y - 200},
		size = Size,
		name = 'Lab',
		imagePath = '',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.BUILD,
		trigger = 84,
		structureId = 11
	},
	{
		pos = {x = res.x - 4 * Size.x - 200, y = res.y - 200},
		size = Size,
		name = 'Point defense',
		imagePath = '',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.BUILD,
		trigger = 80,
		structureId = 12
	}
}
