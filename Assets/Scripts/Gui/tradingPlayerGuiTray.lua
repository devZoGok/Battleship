res = graphics.resolution

buttonSize = {x = 100, y = 20}
textboxSize = {x = 200, y = 25}

height = HubPos.y + HubMargin.top + lineId * 30
warIcon = 'Icons/PlayerStates/war.png'
allianceIcon = 'Icons/PlayerStates/alliance.png'

gui = {
	{
		guiType = GuiType.TEXT,
		name = 'playerName',
		text = '',
		pos = {x = HubPos.x + HubMargin.left, y = height + 10, z = .11},
		scale = {x = .2, y = .2},
		font = 'batang.ttf',
		fontFirstChar = 0,
		fontLastChar = 256,
		color = {x = 1, y = 1, z = 1, w = 1}
	},
	--[[
	{
		guiType = GuiType.GUI_RECTANGLE,
		pos = {x = Pos.x + margin.left + 20, y = height},
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
		buttonType = ButtonType.TRADING_SCREEN,
		name = 'Trade',
		guiScreen = 'tradingScreen.lua',
		pos = {x = HubPos.x + HubSize.x - buttonSize.x - HubMargin.right, y = height, z = .11},
		size = buttonSize,
		trigger = 10
	}
}
