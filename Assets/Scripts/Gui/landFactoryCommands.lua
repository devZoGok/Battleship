numGui = 2
res = graphics.resolution
Size = {x = 100, y = 100}

gui = {
	{
		pos = {x = res.x - 200, y = res.y - 200},
		size = Size,
		imagePath = '',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.LAND_FACTORY_TRAIN,
		trigger = 84,
		unitId = UnitId.TANK
	},
	{
		pos = {x = res.x - Size.x - 200, y = res.y - 200},
		size = Size,
		imagePath = '',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.LAND_FACTORY_TRAIN,
		trigger = 65,
		unitId = UnitId.ARTILLERY
	}
}
