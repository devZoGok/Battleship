ProjectileId = {CRUISE_MISSILE = 0, TORPEDO = 1}
ProjectileClass = {CRUISE_MISSILE = 0, TORPEDO = 1}

projectiles = {
	{
		projectileClass = ProjectileClass.CRUISE_MISSILE,
		size = {x = 1.2, y = 2.7, z = .54},
		speed = .5,
		rotAngle = .1,
		explosion = {damage = 100, radius = 20, sfx = PATH .. 'Sounds/SFX/Explosions/explosion02.ogg'},
		directHitDamage = 450,
		rayLength = 1.25,
		name = "Cruise missile",
	    basePath = PATH .. "Models/GameObjects/Projectiles/Missiles/",
		meshPath = "cruiseMissile.xml",
	},
	{
		projectileClass = ProjectileClass.TORPEDO,
		size = {x = .2, y = 1.6, z = .2},
		speed = .06,
		explosion = {damage = 20, radius = 3, sfx = PATH .. 'Sounds/Units/Submarines/torpedo.ogg'},
		directHitDamage = 450,
		rayLength = .75,
		name = "Torpedo",
	    basePath = PATH .. "Models/GameObjects/Projectiles/Torpedos/",
		meshPath = "torpedo.xml",
	}
}
