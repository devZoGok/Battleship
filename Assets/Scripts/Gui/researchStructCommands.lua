res = graphics.resolution
Size = {x = 70, y = 70}
margin = 10
sz = {x = Size.x - margin, y = Size.y - margin}

gui = {
	--[[
	{
		pos = {x = res.x - Size.x, y = res.y - Size.y},
		size = sz,
		name = "Research",
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.RESEARCH,
		techId = TechId.APT,
		trigger = 82,
	},
	]]--
	{
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.ACTIVE_GAME_STATE,
		name = 'Tech',
		guiScreen = 'techTree.lua',
		pos = {x = res.x - 2 * Size.x, y = res.y - Size.y, z = 0},
		size = sz,
		trigger = 85
	},
}
