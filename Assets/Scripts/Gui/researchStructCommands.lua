res = graphics.resolution
Size = {x = 100, y = 100}

gui = {
	{
		pos = {x = res.x - 200, y = res.y - 200},
		size = Size,
		name = "Research",
		imagePath = '',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.RESEARCH,
		techId = TechId.APT,
		trigger = 82,
	},
	{
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.ACTIVE_GAME_STATE,
		name = 'Tech',
		imagePath = '',
		pos = {x = res.x - Size.x, y = res.y - Size.y},
		size = Size,
		trigger = 85
	},
}
