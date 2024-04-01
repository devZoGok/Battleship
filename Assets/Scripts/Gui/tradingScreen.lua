res = graphics.resolution
Pos = {x = 10, y = 10}
Size = {x = res.x * .8, y = res.y * .8}
margin = {top = 100, left = 100, right = 100, bottom = 100}

resTrayPos = {x = Pos.x + margin.left, y = Pos.y + margin.top}
iconSize = {x = 25, y = 25}
textboxSize = {x = 200, y = 25}

buttonSize = {x = 100, y = 20}

ResourceTrayGuiId = {ICON = 0, TEXTBOX = 1, INCREMENT = 2, DECREMENT = 3}
IconPath = {WEALTH = 'Icons/Resources/wealth.jpg', RESEARCH = 'Icons/Resources/research.jpg', REFINEDS = 'Icons/Resources/refineds.jpg', TIME = 'Icons/Resources/time.jpg'}
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
			buttonType = ButtonType.RESOURCE_AMMOUNT,
			ammount = 10,
			name = '+',
			pos = guiElPos,
			size = iconSize,
			trigger = 10
		}
	elseif guiId == ResourceTrayGuiId.DECREMENT then
		guiElPos = {x = Pos.x + (cpuPlayer and margin.left + 2 * iconSize.x + textboxSize.x or Size.x -(margin.right + iconSize.x)), y = height}

		return {
			guiType = GuiType.BUTTON,
			buttonType = ButtonType.RESOURCE_AMMOUNT,
			ammount = -10,
			name = '-',
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
	createResourceTrayGui(true, 0, ResourceTrayGuiId.ICON, IconPath.REFINEDS),
	createResourceTrayGui(true, 0, ResourceTrayGuiId.TEXTBOX),
	createResourceTrayGui(true, 0, ResourceTrayGuiId.INCREMENT),
	createResourceTrayGui(true, 0, ResourceTrayGuiId.DECREMENT),
	createResourceTrayGui(false, 0, ResourceTrayGuiId.ICON, IconPath.REFINEDS),
	createResourceTrayGui(false, 0, ResourceTrayGuiId.TEXTBOX),
	createResourceTrayGui(false, 0, ResourceTrayGuiId.INCREMENT),
	createResourceTrayGui(false, 0, ResourceTrayGuiId.DECREMENT),
	createResourceTrayGui(true, 1, ResourceTrayGuiId.ICON, IconPath.WEALTH),
	createResourceTrayGui(true, 1, ResourceTrayGuiId.TEXTBOX),
	createResourceTrayGui(true, 1, ResourceTrayGuiId.INCREMENT),
	createResourceTrayGui(true, 1, ResourceTrayGuiId.DECREMENT),
	createResourceTrayGui(false, 1, ResourceTrayGuiId.ICON, IconPath.WEALTH),
	createResourceTrayGui(false, 1, ResourceTrayGuiId.TEXTBOX),
	createResourceTrayGui(false, 1, ResourceTrayGuiId.INCREMENT),
	createResourceTrayGui(false, 1, ResourceTrayGuiId.DECREMENT),
	createResourceTrayGui(true, 2, ResourceTrayGuiId.ICON, IconPath.RESEARCH),
	createResourceTrayGui(true, 2, ResourceTrayGuiId.TEXTBOX),
	createResourceTrayGui(true, 2, ResourceTrayGuiId.INCREMENT),
	createResourceTrayGui(true, 2, ResourceTrayGuiId.DECREMENT),
	createResourceTrayGui(false, 2, ResourceTrayGuiId.ICON, IconPath.RESEARCH),
	createResourceTrayGui(false, 2, ResourceTrayGuiId.TEXTBOX),
	createResourceTrayGui(false, 2, ResourceTrayGuiId.INCREMENT),
	createResourceTrayGui(false, 2, ResourceTrayGuiId.DECREMENT),
	createResourceTrayGui(true, 3, ResourceTrayGuiId.ICON, IconPath.TIME),
	createResourceTrayGui(true, 3, ResourceTrayGuiId.TEXTBOX),
	createResourceTrayGui(true, 3, ResourceTrayGuiId.INCREMENT),
	createResourceTrayGui(true, 3, ResourceTrayGuiId.DECREMENT),
	createResourceTrayGui(false, 3, ResourceTrayGuiId.ICON, IconPath.TIME),
	createResourceTrayGui(false, 3, ResourceTrayGuiId.TEXTBOX),
	createResourceTrayGui(false, 3, ResourceTrayGuiId.INCREMENT),
	createResourceTrayGui(false, 3, ResourceTrayGuiId.DECREMENT),
	{
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.PLAYER_TRADE,
		name = 'Back',
		guiScreen = 'tradingHub.lua',
		pos = {x = Pos.x + Size.x - 2 * buttonSize.x - margin.right, y = Pos.y + Size.y - buttonSize.y - margin.bottom},
		size = buttonSize,
		trigger = 10
	},
	{
		guiType = GuiType.BUTTON,
		buttonType = ButtonType.TRADE_OFFER,
		name = 'Offer',
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
