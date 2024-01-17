ProjectileClass = {GUIDED_MISSILE = 0, TORPEDO = 1}

projectiles = {
	projectileClass = {
		ProjectileClass.GUIDED_MISSILE,
		ProjectileClass.TORPEDO,
	},
	speed = {.5, .06},
	rayLength = {1.25, .75},
	pos = {{x = 0, y = 2.44, z = -3.5}, {x = 0, y = -.27, z = 4.6}},
	rot = {{w = .707, x = -.707, y = 0, z = 0}, {w = 1, x = 0, y = 0, z = 0}},
	damage = {450, 300},
	name = {
	    "Guided missile",
	    "Torpedo"
	},
	basePath = {
	    PATH .. "Models/GameObjects/Projectiles/Missiles/",
	    PATH .. "Models/GameObjects/Projectiles/Torpedos/",
	},
	meshPath = {
	    "guidedMissile.xml",
	    "torpedo.xml",
	},
	unitCornerPoints = {
		{
			{x = .592, y = -1.3475, z = -.2755},
			{x = -.592, y = -1.3475, z = -.2755},
			{x = -.592, y = -1.3475, z = .2755},
			{x = .592, y = -1.3475, z = .2755},
			{x = .592, y = 1.3475, z = -.2755},
			{x = -.592, y = 1.3475, z = -.2755},
			{x = -.592, y = 1.3475, z = .2755},
			{x = .592, y = 1.3475, z = .2755}
		},
		{
			{x = .1, y = -.796, z = -.1},
			{x = -.1, y = -.796, z = -.1},
			{x = -.1, y = -.796, z = .1},
			{x = .1, y = -.796, z = .1},
			{x = .1, y = .796, z = -.1},
			{x = -.1, y = .796, z = -.1},
			{x = -.1, y = .796, z = .1},
			{x = .1, y = .796, z = .1}
		}
	}
}
