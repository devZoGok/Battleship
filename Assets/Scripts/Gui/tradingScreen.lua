res = graphics.resolution
Pos = {x = 10, y = 10}
Size = {x = res.x * .8, y = res.y * .8}
margin = {top = 100, left = 100, right = 100, bottom = 100}

resTrayPos = {x = Pos.x + margin.left, y = Pos.y + margin.top}
iconSize = {x = 25, y = 25}
textboxSize = {x = 200, y = 25}

buttonSize = {x = 100, y = 20}

ResourceTrayGuiId = {ICON = 0, TEXTBOX = 1, INCREMENT = 2, DECREMENT = 3}
IconPath = {WEALTH = 'Icons/wealth.png', RESEARCH = 'Icons/research.png', REFINEDS = 'Icons/refineds.png', TIME = 'Icons/time.png'}
function createResourceTrayGui(cpuPlayer, lineId, guiId, imgPath)
	height = margin.top + lineId * iconSize.y

	if guiId == ResourceTrayGuiId.ICON then
		guiElPos = {x = Pos.x + (cpuPlayer and margin.left or Size.x -(margin.right + 3 * iconSize.x + textboxSize.x)), y = height}

		return {
			guiType = GuiType.GUI_RECTANGLE,
			pos = guiElPos,
			zIndex = .8,
			size = iconSize,
			imagePath = imgPath
		}
	elseif guiId == ResourceTrayGuiId.TEXTBOX then
		guiElPos = {x = Pos.x + (cpuPlayer and margin.left + iconSize.x or Size.x -(margin.right + 2 * iconSize.x + textboxSize.x)), y = height}

		return {
			pos = guiElPos,
			size = textboxSize,
			guiType = GuiType.TEXTBOX,
		}
	elseif guiId == ResourceTrayGuiId.INCREMENT then
		guiElPos = {x = Pos.x + (cpuPlayer and margin.left + iconSize.x + textboxSize.x or Size.x -(margin.right + 2 * iconSize.x)), y = height}

		return {
			guiType = GuiType.BUTTON,
			buttonType = ButtonType.ACTIVE_GAME_STATE,
			name = '+',
			imagePath = '',
			guiScreen = 'tradingScreen.lua',
			pos = guiElPos,
			size = iconSize,
			trigger = 10
		}
	elseif guiId == ResourceTrayGuiId.DECREMENT then
		guiElPos = {x = Pos.x + (cpuPlayer and margin.left + 2 * iconSize.x + textboxSize.x or Size.x -(margin.right + iconSize.x)), y = height}

		return {
			guiType = GuiType.BUTTON,
			buttonType = ButtonType.ACTIVE_GAME_STATE,
			name = '-',
			imagePath = '',
			guiScreen = 'tradingScreen.lua',
			pos = guiElPos,
			size = iconSize,
			trigger = 10
		}
	end
end

gui = {
	{
		guiType = GuiType.TEXT,
		name = 'P0',
		text = 'P0',
		pos = {x = Pos.x + margin.left, y = Pos.y + margin.top},
		zIndex = 0,
		scale = 1,
		font = 'batang.ttf',
		fontFirstChar = 0,
		fontLastChar = 256,
		color = {x = 1, y = 1, z = 1, w = 1}
	},
	{
		guiType = GuiType.TEXT,
		name = 'You',
		text = 'You',
		pos = {x = Pos.x + margin.left, y = Pos.y + margin.top},
		zIndex = 0,
		scale = 1,
		font = 'batang.ttf',
		fontFirstChar = 0,
		fontLastChar = 256,
		color = {x = 1, y = 1, z = 1, w = 1}
	},
	createResourceTrayGui(true, 0, ResourceTrayGuiId.ICON, IconPath.WEALTH),
	createResourceTrayGui(true, 0, ResourceTrayGuiId.TEXTBOX, IconPath.RESEARCH),
	createResourceTrayGui(true, 0, ResourceTrayGuiId.INCREMENT, IconPath.REFINEDS),
	createResourceTrayGui(true, 0, ResourceTrayGuiId.DECREMENT, IconPath.TIME),
	createResourceTrayGui(false, 0, ResourceTrayGuiId.ICON, IconPath.WEALTH),
	createResourceTrayGui(false, 0, ResourceTrayGuiId.TEXTBOX, IconPath.RESEARCH),
	createResourceTrayGui(false, 0, ResourceTrayGuiId.INCREMENT, IconPath.REFINEDS),
	createResourceTrayGui(false, 0, ResourceTrayGuiId.DECREMENT, IconPath.TIME),
	createResourceTrayGui(true, 1, ResourceTrayGuiId.ICON, IconPath.WEALTH),
	createResourceTrayGui(true, 1, ResourceTrayGuiId.TEXTBOX, IconPath.RESEARCH),
	createResourceTrayGui(true, 1, ResourceTrayGuiId.INCREMENT, IconPath.REFINEDS),
	createResourceTrayGui(true, 1, ResourceTrayGuiId.DECREMENT, IconPath.TIME),
	createResourceTrayGui(false, 1, ResourceTrayGuiId.ICON, IconPath.WEALTH),
	createResourceTrayGui(false, 1, ResourceTrayGuiId.TEXTBOX, IconPath.RESEARCH),
	createResourceTrayGui(false, 1, ResourceTrayGuiId.INCREMENT, IconPath.REFINEDS),
	createResourceTrayGui(false, 1, ResourceTrayGuiId.DECREMENT, IconPath.TIME),
	createResourceTrayGui(true, 2, ResourceTrayGuiId.ICON, IconPath.WEALTH),
	createResourceTrayGui(true, 2, ResourceTrayGuiId.TEXTBOX, IconPath.RESEARCH),
	createResourceTrayGui(true, 2, ResourceTrayGuiId.INCREMENT, IconPath.REFINEDS),
	createResourceTrayGui(true, 2, ResourceTrayGuiId.DECREMENT, IconPath.TIME),
	createResourceTrayGui(false, 2, ResourceTrayGuiId.ICON, IconPath.WEALTH),
	createResourceTrayGui(false, 2, ResourceTrayGuiId.TEXTBOX, IconPath.RESEARCH),
	createResourceTrayGui(false, 2, ResourceTrayGuiId.INCREMENT, IconPath.REFINEDS),
	createResourceTrayGui(false, 2, ResourceTrayGuiId.DECREMENT, IconPath.TIME),
	createResourceTrayGui(true, 3, ResourceTrayGuiId.ICON, IconPath.WEALTH),
	createResourceTrayGui(true, 3, ResourceTrayGuiId.TEXTBOX, IconPath.RESEARCH),
	createResourceTrayGui(true, 3, ResourceTrayGuiId.INCREMENT, IconPath.REFINEDS),
	createResourceTrayGui(true, 3, ResourceTrayGuiId.DECREMENT, IconPath.TIME),
	createResourceTrayGui(false, 3, ResourceTrayGuiId.ICON, IconPath.WEALTH),
	createResourceTrayGui(false, 3, ResourceTrayGuiId.TEXTBOX, IconPath.RESEARCH),
	createResourceTrayGui(false, 3, ResourceTrayGuiId.INCREMENT, IconPath.REFINEDS),
	createResourceTrayGui(false, 3, ResourceTrayGuiId.DECREMENT, IconPath.TIME),
	{
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.PLAYER_TRADE,
		name = 'Back',
		imagePath = '',
		guiScreen = 'tradingHub.lua',
		pos = {x = Pos.x + Size.x - 2 * buttonSize.x - margin.right, y = Pos.y + Size.y - buttonSize.y - margin.bottom},
		size = buttonSize,
		trigger = 10
	},
	{
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.ACTIVE_GAME_STATE,
		name = 'Offer',
		imagePath = '',
		guiScreen = 'tradingHub.lua',
		pos = {x = Pos.x + Size.x - buttonSize.x - margin.right, y = Pos.y + Size.y - buttonSize.y - margin.bottom},
		size = buttonSize,
		trigger = 10
	},
	{
		pos = {x = Pos.x + margin.left, y = Pos.y + Size.y - textboxSize.y - margin.bottom},
		size = textboxSize,
		guiType = GuiType.TEXTBOX,
	}
}
