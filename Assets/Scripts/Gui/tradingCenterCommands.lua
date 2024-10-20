res = graphics.resolution
Size = {x = 70, y = 70}
margin = 10
sz = {x = Size.x - margin, y = Size.y - margin}

gui = {
	{
		pos = {x = res.x - Size.x, y = res.y - Size.y, z = 0},
		size = sz,
		name = 'Buy ref',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.BUY_REFINEDS,
		trigger = 73,
	},
	{
		pos = {x = res.x - 2 * Size.x, y = res.y - Size.y, z = 0},
		size = sz,
		name = 'Sell ref',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.SELL_REFINEDS,
		trigger = 79,
	},
	{
		pos = {x = res.x - Size.x, y = res.y - 2 * Size.y, z = 0},
		size = sz,
		name = 'Buy rsch',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.BUY_RESEARCH,
		trigger = 75,
	},
	{
		pos = {x = res.x - 2 * Size.x, y = res.y - 2 * Size.y, z = 0},
		size = sz,
		name = 'Sell rsch',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.SELL_RESEARCH,
		trigger = 76,
	}
}
