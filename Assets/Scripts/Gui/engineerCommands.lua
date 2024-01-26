res = graphics.resolution
Size = {x = 100, y = 100}

gui = {
	{
		pos = {x = res.x - 200, y = res.y - 200},
		size = Size,
		imagePath = '',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.BUILD,
		trigger = 76,
		structureId = UnitId.LAND_FACTORY 
	},
	{
		pos = {x = res.x - Size.x - 200, y = res.y - 200},
		size = Size,
		imagePath = '',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.BUILD,
		trigger = 78,
		structureId = UnitId.NAVAL_FACTORY 
	},
	{
		pos = {x = res.x - 2 * Size.x - 200, y = res.y - 200},
		size = {x = 100, y = 100},
		imagePath = '',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.BUILD,
		trigger = 77,
		structureId = UnitId.MARKET
	},
	{
		pos = {x = res.x - 3 * Size.x - 200, y = res.y - 200},
		size = Size,
		imagePath = '',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.BUILD,
		trigger = 84,
		structureId = UnitId.LAB
	},
	{
		pos = {x = res.x - 4 * Size.x - 200, y = res.y - 200},
		size = Size,
		imagePath = '',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.BUILD,
		trigger = 68,
		structureId = UnitId.POINT_DEFENSE
	},
	{
		pos = {x = res.x - 5 * Size.x - 200, y = res.y - 200},
		size = Size,
		imagePath = '',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.BUILD,
		trigger = 70,
		structureId = UnitId.FORT 
	},
}
