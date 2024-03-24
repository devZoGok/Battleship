res = graphics.resolution
Pos = {x = 10, y = 10}
Size = {x = res.x * .8, y = res.y * .8}
buttonSize = {x = 100, y = 20}
textboxSize = {x = 200, y = 25}
margin = {top = 10, left = 10, right = 10, bottom = 10}

height = Pos.y + margin.top + lineId * 30
warIcon = 'Icons/PlayerStates/war.png'
allianceIcon = 'Icons/PlayerStates/alliance.png'

gui = {
	{
		guiType = GuiType.TEXT,
		name = 'playerName',
		text = '',
		pos = {x = Pos.x + margin.left, y = height},
		zIndex = 0,
		scale = 1,
		font = 'batang.ttf',
		fontFirstChar = 0,
		fontLastChar = 256,
		color = {x = 1, y = 1, z = 1, w = 1}
	},
	--[[
	{
		guiType = GuiType.GUI_RECTANGLE,
		pos = {x = Pos.x + margin.left + 20, y = height},
		zIndex = .9,
		size = Size,
		color = {x = 0, y = 0, z = 0, w = .6}
	},
	{
		pos = {x = Pos.x + margin.left + 40, y = height},
		size = textboxSize,
		guiType = GuiType.LISTBOX,
		listboxType = ListboxType.TRADE_OFFERS,
		numMaxDisplay = 1,
		lines = {''}
	},
	]]--
	{
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.ACTIVE_GAME_STATE,
		name = 'Trade',
		guiScreen = 'tradingScreen.lua',
		pos = {x = Pos.x + Size.x - buttonSize.x - margin.right, y = height},
		size = buttonSize,
		trigger = 10
	}
}
