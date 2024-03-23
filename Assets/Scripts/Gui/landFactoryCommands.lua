res = graphics.resolution
Size = {x = 70, y = 70}
margin = 10
sz = {x = Size.x - margin, y = Size.y - margin}

gui = {
	{
		pos = {x = res.x + .5 * margin - Size.x, y = res.y - 3 * Size.y},
		size = sz,
		imagePath = '',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.LAND_FACTORY_TRAIN,
		trigger = 87
	},
	{
		pos = {x = res.x + .5 * margin - 2 * Size.x, y = res.y - 3 * Size.y},
		size = sz,
		imagePath = '',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.LAND_FACTORY_TRAIN,
		trigger = 84
	},
	{
		pos = {x = res.x + .5 * margin - 3 * Size.x, y = res.y - 3 * Size.y},
		size = sz,
		imagePath = '',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.LAND_FACTORY_TRAIN,
		trigger = 65
	}
}
