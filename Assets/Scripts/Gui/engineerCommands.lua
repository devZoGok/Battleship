numGui = 1
res = graphics.resolution

gui = {
	{
		pos = {x = res.x - 200, y = res.y - 200},
		size = {x = 100, y = 100},
		name = 'Point defense',
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.BUILD,
		trigger = 66,
		structureId = 8
	}
}
