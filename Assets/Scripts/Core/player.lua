Player = {}

function Player:new(pl)
	player = pl or {}
	self.__index = self
	setmetatable(player, self)
	return player
end

function Player:foo1()
	print('foo 1')
	return true
end

function Player:foo2()
	print('foo 2')
	return true
end

function Player:foo3()
	print('foo 3')
	return true
end

function Player:foo4()
	print('foo 4')
	return true
end

function Player:update()
	executeBtNode(self.behaviour)
end

Player.behaviour = {
	type = BTNodeType.SEQUENCE,
	children = {
		{type = BTNodeType.FUNCTION, func = Player.foo1},
		{type = BTNodeType.FUNCTION, func = Player.foo2},
		{type = BTNodeType.SEQUENCE, children = {{type = BTNodeType.FUNCTION, func = Player.foo3}, {type = BTNodeType.FUNCTION, func = Player.foo4}}}
	}
}
