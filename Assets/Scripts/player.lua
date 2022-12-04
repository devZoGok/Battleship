Player = {}

function Player:new(pl)
	player = pl or {}
	self.__index = self
	setmetatable(player, self)
	return player
end

function Player:update()
	print("id = " .. self.id)
end
