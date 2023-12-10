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

Player.behaviour = {
	type = BTNodeType.SEQUENCE,
	children = {
		{type = BTNodeType.FUNCTION, func = Player.foo3}
	}
}
