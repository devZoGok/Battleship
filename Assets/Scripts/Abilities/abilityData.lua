AbilityType = {
	SPEED = 0,
	HEALTH = 1,
	LINE_OF_SIGHT = 1,
	MAX_TURN_ANGLE = 2,
	DIRECT_HIT_DAMAGE = 3,
	EXPLOSION_RADIUS = 4,
	EXPLOSION_DAMAGE = 5,
	CAPACITY = 6,
	LOAD_RATE = 7,
	LOAD_SPEED = 8,
	DRAW_RATE = 9,
	DRAW_SPEED = 10,
	HACK_RANGE = 11,
	UNIT_UNLOCK = 12
}

abilities = {
	{type = AbilityType.SPEED, gameObjType = 0, ammount = .4, gameObjIds = {UnitId.WAR_MECH}},
	{type = AbilityType.UNIT_UNLOCK, gameObjType = 0, gameObjIds = {UnitId.WAR_MECH}},
}
