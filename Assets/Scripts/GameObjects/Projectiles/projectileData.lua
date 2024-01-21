ProjectileClass = {CRUISE_MISSILE = 0, TORPEDO = 1}

projectiles = {
	projectileClass = {
		ProjectileClass.CRUISE_MISSILE,
		ProjectileClass.TORPEDO,
	},
	speed = {.5, .06},
	rayLength = {1.25, .75},
	damage = {450, 300},
	name = {
	    "Cruise missile",
	    "Torpedo"
	},
	basePath = {
	    PATH .. "Models/GameObjects/Projectiles/Missiles/",
	    PATH .. "Models/GameObjects/Projectiles/Torpedos/",
	},
	meshPath = {
	    "cruiseMissile.xml",
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
