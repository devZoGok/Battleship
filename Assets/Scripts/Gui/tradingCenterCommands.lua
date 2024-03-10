res = graphics.resolution
Size = {x = 100, y = 100}

gui = {
	{
		pos = {x = res.x - 200, y = res.y - 200},
		size = Size,
		name = 'Buy ref',
		imagePath = '',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.BUY_REFINEDS,
		trigger = 73,
	},
	{
		pos = {x = res.x - Size.x - 200, y = res.y - 200},
		size = Size,
		imagePath = '',
		name = 'Sell ref',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.SELL_REFINEDS,
		trigger = 79,
	},
	{
		pos = {x = res.x - 200, y = res.y - 400},
		size = Size,
		imagePath = '',
		name = 'Buy rsch',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.BUY_RESEARCH,
		trigger = 75,
	},
	{
		pos = {x = res.x - Size.x - 200, y = res.y - 400},
		size = Size,
		imagePath = '',
		name = 'Sell rsch',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.SELL_RESEARCH,
		trigger = 76,
	}
}
