numGui = 2
res = graphics.resolution
Size = {x = 100, y = 100}

gui = {
	{
		pos = {x = res.x - 200, y = res.y - 200},
		size = Size,
		name = 'Tank',
		imagePath = '',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.LAND_FACTORY_TRAIN,
		trigger = 84,
		unitId = 1
	},
	{
		pos = {x = res.x - Size.x - 200, y = res.y - 200},
		size = Size,
		name = 'Artillery',
		imagePath = '',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.LAND_FACTORY_TRAIN,
		trigger = 65,
		unitId = 2
	}
}
