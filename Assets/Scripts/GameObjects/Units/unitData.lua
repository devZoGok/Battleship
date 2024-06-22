--TYPE = {ATTACK = 0, BUILD = 1, MOVE = 2, GARRISON = 3, EJECT = 4, PATROL = 5, LAUNCH = 6};
UnitId = {
	WAR_MECH = 0,
	TANK = 1,
	ARTILLERY = 2,
	ENGINEER = 3,
	SCOUT_TRANSPORT = 4,
	HOVER_TRANSPORT = 5,
	RESOURCE_ROVER = 6,
	TACTICAL_CRUISER = 7,
	DEFENSIVE_CRUISER = 8,
	CHEAP_CRUISER = 9,
	TRANSPORT_CRUISER = 10,
	HEAVY_CARRIER = 11,
	CHAMPION_CARRIER = 12,
	MISSILE_SUBMARINE = 13,
	STEALTH_SUBMARINE = 14,
	LAND_FACTORY = 15,
	NAVAL_FACTORY = 16,
	TRADE_CENTER = 17,
	LAB = 18,
	POINT_DEFENSE = 19,
	EXTRACTOR = 20,
	REFINERY = 21,
	FORT = 22
}
WeaponClass = {HITSCAN = 0, SHELL = 1, TORPEDO = 2, CRUISE_MISSILE = 3, HACK = 4}
UnitClass = {
	WAR_MECH = 0,
	TANK = 1,
	ARTILLERY = 2,
	ENGINEER = 3,
	TRANSPORT = 4,
	RESOURCE_ROVER = 5,
	CRUISER = 6,
	CARRIER = 7,
	SUBMARINE = 8,
	LAND_FACTORY = 9,
	NAVAL_FACTORY = 10,
	TRADE_CENTER = 11,
	LAB = 12,
	POINT_DEFENSE = 13,
	EXTRACTOR = 14,
	REFINERY = 15,
	FORT = 16
}
UnitType = {UNDERWATER = 0, SEA_LEVEL = 1, HOVER = 2, LAND = 3, AIR = 4}

ArmorType = {CAST = 0, COMBINED = 1, MECHANIC = 2, SHELL = 3, STEEL = 4}
vfxPrefix = "Models/VFX/"
gameObjPrefix = "Models/GameObjects/"
vehiclePrefix = gameObjPrefix .. "Units/Vehicles/"
structurePrefix = gameObjPrefix .. "Units/Structures/"

explosionVfx = {
	vfx = true,
	duration = 1,
	offset = 100,
	mesh = {
		numParticles = 1,
		texture = PATH .. 'Textures/Explosion/explosion07.png',
		lowLife = 3,
		highLife = 3,
		size = {x = 10, y = 10},
	}
}
explosionSfx = {
	vfx = false,
	offset = 100,
	path = PATH .. 'Sounds/SFX/Explosions/explosion00.ogg',
	duration = 2500
}


units = {
	{
		weapons = {
			{
				type = WeaponClass.HITSCAN,
				rateOfFire = 100,
				maxRange = 10,
				damage = 50,
				fireFx = {
					{
						vfx = true,
						duration = 50,
						mesh = {
							path = PATH .. vfxPrefix .. 'muzzleFlash.xml',
							color = {x = 1, y = 1, z = 0, a = 1},
						},
						pos = {x = 0.214, y = 2.742, z = 1.54},
						rot = {w = 1, x = 0, y = 0, z = 0},
						--scale = .5
					},
					{
						vfx = false,
						duration = 500,
						path = PATH .. 'Sounds/Units/WarMechs/fire.ogg'
					},
				},
				unitHitFx = {
					{
						vfx = false,
						duration = 500,
						path = PATH .. 'Sounds/Units/WarMechs/unitHit.ogg'
					},
				},
				landHitFx = {
					{
						vfx = false,
						duration = 500,
						path = PATH .. 'Sounds/Units/WarMechs/landHit.ogg'
					},
				},
				waterHitFx = {
					{
						vfx = false,
						duration = 500,
						path = PATH .. 'Sounds/Units/WarMechs/waterHit.ogg'
					},
				},
			}
		},
		unitClass = UnitClass.WAR_MECH,
		unitType = UnitType.LAND,
		armor = {ArmorType.MECHANIC},
		isVehicle = true,
		health = 500,
		buildTime = 1000,
		cost = 500,
		size = {x = 2.9, y = 8.22, z = 2.42},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 20,
		name = 'War mech',
		basePath = PATH .. vehiclePrefix .. 'WarMechs/',
		meshPath = 'warMech.xml',
		selectionSfx = PATH .. 'Sounds/Units/WarMechs/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		speed = .1,
		destinationOffset = .1,
		anglePrecision = .1,
		maxTurnAngle = .1,
		garrisonCategory = 1
	},
	{
		weapons = {
			{
				type = WeaponClass.HITSCAN, 
				rateOfFire = 1000, 
				damage = 200, 
				maxRange = 25,
				fireFx = {
					{
						vfx = true,
						duration = 50,
						mesh = {
							path = PATH .. vfxPrefix .. 'muzzleFlash.xml',
							color = {x = 1, y = 1, z = 0, a = 1},
						},
						parent = 'Turret',
						pos = {x = 0, y = .43, z = 5.7},
						rot = {w = 1, x = 0, y = 0, z = 0},
						--scale = .5
					},
					{
						vfx = false,
						duration = 300,
						path = PATH .. 'Sounds/Units/Tanks/attack.ogg', 
					}
				},
				landHitFx = {explosionVfx, explosionSfx},
				unitHitFx = {explosionVfx, explosionSfx}
			}
		},
		unitClass = UnitClass.TANK,
		unitType = UnitType.LAND,
		armor = {ArmorType.STEEL},
		isVehicle = true,
		health = 500,
		buildTime = 1000,
		cost = 500,
		size = {x = 2.9, y = 8.22, z = 2.42},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 25,
		name = 'Tank',
		basePath = PATH .. vehiclePrefix .. 'Tanks/',
		meshPath = 'tank2.xml',
		colorNodes = {'Antenna_S', 'Antenna_L', 'Hatch'},
		selectionSfx = PATH .. 'Sounds/Units/Tanks/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		speed = .1,
		destinationOffset = .1,
		anglePrecision = .1,
		maxTurnAngle = .1,
		garrisonCategory = 2
	},
	{
		weapons = {
			{
				type = WeaponClass.HITSCAN, 
				rateOfFire = 2000, 
				damage = 5000, 
				maxRange = 30,
				fireFx = {
					{
						vfx = true,
						duration = 50,
						mesh = {
							path = PATH .. vfxPrefix .. 'muzzleFlash.xml',
							color = {x = 1, y = 1, z = 0, a = 1},
						},
						pos = {x = 0.5, y = 2.95, z = 1.95},
						rot = {w = 1, x = 0, y = 0, z = 0},
						--scale = .5
					},
					{
						vfx = false,
						duration = 300,
						path = PATH .. 'Sounds/Units/Tanks/attack.ogg', 
					}
				},
				landHitFx = {explosionVfx, explosionSfx},
				unitHitFx = {explosionVfx, explosionSfx}
			}
		},
		unitClass = UnitClass.ARTILLERY,
		unitType = UnitType.LAND,
		armor = {ArmorType.MECHANIC},
		isVehicle = true,
		health = 500,
		buildTime = 1000,
		cost = 500,
		size = {x = 2.9, y = 8.22, z = 2.42},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 30,
		name = 'Artillery',
		basePath = PATH .. vehiclePrefix .. 'Artillery/',
		meshPath = 'artillery.xml',
		selectionSfx = PATH .. 'Sounds/Units/Tanks/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		speed = .1,
		destinationOffset = .1,
		anglePrecision = .1,
		maxTurnAngle = .1,
		garrisonCategory = 2
	},
	{
		weapons = {
			{type = WeaponClass.HITSCAN, rateOfFire = 2000, fireSfx = PATH .. 'Sounds/Units/WarMechs/fire.ogg', damage = 20, maxRange = 8},
		},
		buildableUnits = {
			{id = UnitId.LAND_FACTORY, buildable = true},
			{id = UnitId.NAVAL_FACTORY, buildable = true},
			{id = UnitId.TRADE_CENTER, buildable = true},
			{id = UnitId.LAB, buildable = true},
			{id = UnitId.POINT_DEFENSE, buildable = true},
			{id = UnitId.FORT, buildable = true},
			{id = UnitId.EXTRACTOR, buildable = true},
		},
		unitClass = UnitClass.ENGINEER,
		unitType = UnitType.HOVER,
		armor = {ArmorType.MECHANIC},
		isVehicle = true,
		health = 500,
		hackRange = 30,
		buildTime = 1000,
		hackTime = 1000,
		cost = 500,
		size = {x = 1, y = 8, z = 13.45},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 20,
		name = 'Engineer',
		basePath = PATH .. vehiclePrefix .. 'Engineers/',
		meshPath = 'engineer.xml',
		guiScreen = 'engineerCommands.lua',
		selectionSfx = PATH .. 'Sounds/Units/Engineers/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		speed = .1,
		destinationOffset = .1,
		anglePrecision = .1,
		maxTurnAngle = .1,
		garrisonCategory = 3
	},
	{
		weapons = {{type = WeaponClass.HITSCAN, rateOfFire = 2000, fireSfx = PATH .. 'Sounds/Units/WarMechs/fire.ogg', damage = 50, maxRange = 20}},
		unitClass = UnitClass.TRANSPORT,
		unitType = UnitType.HOVER,
		isVehicle = true,
		health = 500,
		buildTime = 1000,
		cost = 500,
		size = {x = 2, y = 8.25, z = 2},
		garrisonCapacity = {2, 2, 2},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 5,
		name = 'Scout transport',
		basePath = PATH .. vehiclePrefix .. 'Transports/',
		meshPath = 'scoutTransport.xml',
		selectionSfx = PATH .. 'Sounds/Units/Transports/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		speed = .1,
		destinationOffset = .1,
		anglePrecision = .1,
		maxTurnAngle = .1,
		garrisonCategory = 3
	},
	{
		weapons = {{type = WeaponClass.HITSCAN, rateOfFire = 2000, fireSfx = PATH .. 'Sounds/Units/Tanks/attack.ogg', damage = 100, maxRange = 25}},
		unitClass = UnitClass.TRANSPORT,
		unitType = UnitType.HOVER,
		armor = {ArmorType.MECHANIC},
		isVehicle = true,
		health = 500,
		buildTime = 1000,
		cost = 500,
		garrisonCapacity = {2, 2, 2},
		size = {x = 2, y = 8.25, z = 2},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 5,
		name = 'Hover transport',
		basePath = PATH .. vehiclePrefix .. 'Transports/',
		meshPath = 'hoverTransport.xml',
		selectionSfx = PATH .. 'Sounds/Units/Transports/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		speed = .1,
		destinationOffset = .1,
		anglePrecision = .1,
		maxTurnAngle = .1,
		garrisonCategory = 3
	},
	{
		weapons = {},
		unitClass = UnitClass.RESOURCE_ROVER,
		unitType = UnitType.HOVER,
		armor = {ArmorType.CAST},
		capacity = 3000,
		loadRate = 1,
		loadSpeed = 5,
		isVehicle = true,
		health = 500,
		buildTime = 1000,
		cost = 500,
		size = {x = 2, y = 8.25, z = 2},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 5,
		name = 'Cargo ship',
		basePath = PATH .. vehiclePrefix .. 'Cargoships/',
		meshPath = 'cargoship.xml',
		selectionSfx = PATH .. 'Sounds/Units/Cargoships/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		speed = .1,
		destinationOffset = .1,
		anglePrecision = .1,
		maxTurnAngle = .1,
		garrisonCategory = 3
	},
	{
		weapons = {
			{
				type = WeaponClass.HITSCAN, 
				rateOfFire = 200, 
				damage = 300, 
				maxRange = 15,
				fireFx = {
					{
						vfx = true,
						duration = 50,
						mesh = {
							size = {x = .16, y = .16},
							color = {x = 1, y = 0, z = 0, a = 1},
						},
						pos = {x = 0., y = 2.8, z = -.1},
						rot = {w = 1, x = 0, y = 0, z = 0},
						--scale = .5
					},
					{
						vfx = false,
						duration = 50,
						path = PATH .. 'Sounds/Units/Cruisers/fire.ogg',
					}
				},
				hitFx = {}
			}
		},
		unitClass = UnitClass.CRUISER,
		unitType = UnitType.SEA_LEVEL,
		armor = {ArmorType.STEEL},
		isVehicle = true,
		health = 500,
		buildTime = 1000,
		cost = 500,
		size = {x = 2, y = 8.25, z = 2},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 5,
		name = 'Tactical cruiser',
		basePath = PATH .. vehiclePrefix .. 'Cruisers/',
		meshPath = 'tacticalCruiser.xml',
		selectionSfx = PATH .. 'Sounds/Units/Cruisers/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		speed = .1,
		destinationOffset = .1,
		anglePrecision = .1,
		maxTurnAngle = .1,
		garrisonCategory = 3
	},
	{
		weapons = {
			{
				type = WeaponClass.HITSCAN, 
				rateOfFire = 200, 
				fireFx = {
					{
						vfx = true,
						duration = 50,
						mesh = {
							size = {x = .16, y = .16},
							color = {x = 1, y = 0, z = 0, a = 1},
						},
						pos = {x = 0., y = 2.8, z = -.1},
						rot = {w = 1, x = 0, y = 0, z = 0},
						--scale = .5
					},
					{
						vfx = false,
						duration = 50,
						path = PATH .. 'Sounds/Units/Cruisers/fire.ogg',
					}
				},
				damage = 300, 
				maxRange = 15
			}
		},
		unitClass = UnitClass.CRUISER,
		unitType = UnitType.SEA_LEVEL,
		armor = {ArmorType.CAST},
		isVehicle = true,
		health = 500,
		buildTime = 1000,
		cost = 500,
		garrisonCapacity = {2, 2, 2},
		size = {x = 2, y = 8.25, z = 2},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 5,
		name = 'Defensive cruiser',
		basePath = PATH .. vehiclePrefix .. 'Cruisers/',
		meshPath = 'defensiveCruiser.xml',
		selectionSfx = PATH .. 'Sounds/Units/Cruisers/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		speed = .1,
		destinationOffset = .1,
		anglePrecision = .1,
		maxTurnAngle = .1,
		garrisonCategory = 3
	},
	{
		weapons = {
			{
				type = WeaponClass.HITSCAN,
				rateOfFire = 200, 
				damage = 300,
				maxRange = 15,
				fireFx = {
					{
						vfx = true,
						duration = 50,
						mesh = {
							size = {x = .16, y = .16},
							color = {x = 1, y = 0, z = 0, a = 1},
						},
						pos = {x = 0., y = 2.8, z = -.1},
						rot = {w = 1, x = 0, y = 0, z = 0},
						--scale = .5
					},
					{
						vfx = false,
						duration = 50,
						path = PATH .. 'Sounds/Units/Cruisers/fire.ogg',
					}
				},
			}
		},
		unitClass = UnitClass.CRUISER,
		unitType = UnitType.SEA_LEVEL,
		armor = {ArmorType.CAST},
		isVehicle = true,
		health = 500,
		buildTime = 1000,
		cost = 500,
		size = {x = 2, y = 8.25, z = 2},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 5,
		name = 'Cheap cruiser',
		basePath = PATH .. vehiclePrefix .. 'Cruisers/',
		meshPath = 'cheapCruiser.xml',
		selectionSfx = PATH .. 'Sounds/Units/Cruisers/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		speed = .1,
		destinationOffset = .1,
		anglePrecision = .1,
		maxTurnAngle = .1,
		garrisonCategory = 3
	},
	{
		weapons = {
			{
				type = WeaponClass.HITSCAN, 
				rateOfFire = 200, 
				fireSfx = PATH .. 'Sounds/Units/Cruisers/fire.ogg', 
				damage = 300,
				maxRange = 15,
				fireFx = {
					{
						vfx = true,
						duration = 50,
						mesh = {
							size = {x = .16, y = .16},
							color = {x = 1, y = 0, z = 0, a = 1},
						},
						pos = {x = 0., y = 2.8, z = -.1},
						rot = {w = 1, x = 0, y = 0, z = 0},
						--scale = .5
					},
					{
						vfx = false,
						duration = 50,
						path = PATH .. 'Sounds/Units/Cruisers/fire.ogg',
					}
				}
			}
		},
		unitClass = UnitClass.CRUISER,
		unitType = UnitType.SEA_LEVEL,
		armor = {ArmorType.STEEL, ArmorType.COMBINED},
		isVehicle = true,
		health = 500,
		buildTime = 1000,
		cost = 500,
		size = {x = 2, y = 8.25, z = 2},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 5,
		name = 'Transport cruiser',
		basePath = PATH .. vehiclePrefix .. 'Cruisers/',
		meshPath = 'transportCruiser.xml',
		selectionSfx = PATH .. 'Sounds/Units/Cruisers/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		speed = .1,
		destinationOffset = .1,
		anglePrecision = .1,
		maxTurnAngle = .1,
		garrisonCategory = 3
	},
	{
		weapons = {
			{
				type = WeaponClass.HITSCAN, 
				rateOfFire = 100, 
				damage = 80, 
				maxRange = 100,
				fireFx = {
					{
						vfx = true,
						duration = 50,
						mesh = {
							size = {x = .16, y = .16},
							color = {x = 1, y = 0, z = 0, a = 1},
						},
						pos = {x = 0., y = 2.8, z = -.1},
						rot = {w = 1, x = 0, y = 0, z = 0},
						--scale = .5
					},
					{
						vfx = false,
						duration = 50,
						path = PATH .. 'Sounds/Units/Cruisers/fire.ogg',
					}
				}
			}
		},
		unitClass = UnitClass.CARRIER,
		unitType = UnitType.SEA_LEVEL,
		isVehicle = true,
		health = 500,
		buildTime = 1000,
		cost = 500,
		size = {x = 2, y = 8.25, z = 2},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 5,
		name = 'Heavy carrier',
		basePath = PATH .. vehiclePrefix .. 'Carriers/',
		meshPath = 'heavyCarrier.xml',
		selectionSfx = PATH .. 'Sounds/Units/Carriers/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		speed = .1,
		destinationOffset = .1,
		anglePrecision = .1,
		maxTurnAngle = .1,
		garrisonCategory = 3
	},
	{
		weapons = {
			{
				type = WeaponClass.HITSCAN, 
				rateOfFire = 450, 
				damage = 500, 
				maxRange = 100,
				fireFx = {
					{
						vfx = true,
						duration = 50,
						mesh = {
							path = PATH .. vfxPrefix .. 'muzzleFlash.xml',
							color = {x = 1, y = 1, z = 0, a = 1},
						},
						pos = {x = 0, y = 3.17, z = -1.5},
						rot = {w = 1, x = 0, y = 0, z = 0},
						--scale = .5
					},
					{
						vfx = false,
						duration = 300,
						path = PATH .. 'Sounds/Units/Tanks/attack.ogg', 
					}
				},
				unitHitFx = {explosionVfx, explosionSfx},
				waterHitFx = {explosionVfx, explosionSfx}
			},
			{
				type = WeaponClass.CRUISE_MISSILE, 
				projectile = {id = ProjectileId.CRUISE_MISSILE, pos = {x = 0, y = 1, z = 0}, rot = {w = .707, x = -.707, y = 0, z = 0}},
				rateOfFire = 500, 
				maxRange = 30, 
				fireFx = {
					{
						vfx = false,
						duration = 2500,
						path = PATH .. 'Sounds/Units/Submarines/missile.ogg', 
					}
				},
			}
		},
		unitClass = UnitClass.CARRIER,
		unitType = UnitType.SEA_LEVEL,
		isVehicle = true,
		health = 500,
		buildTime = 1000,
		cost = 500,
		size = {x = 2, y = 8.25, z = 2},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 5,
		name = 'Champion carrier',
		basePath = PATH .. vehiclePrefix .. 'Carriers/',
		meshPath = 'championCarrier.xml',
		selectionSfx = PATH .. 'Sounds/Units/Carriers/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		speed = .1,
		destinationOffset = .1,
		anglePrecision = .1,
		maxTurnAngle = .1,
		garrisonCategory = 3
	},
	{
		weapons = {
			{
				type = WeaponClass.TORPEDO, 
				rateOfFire = 1000, 
				fireFx = {
					{
						vfx = false,
						duration = 2500,
						path = PATH .. 'Sounds/Units/Submarines/fire.ogg', 
					}
				},
				damage = 200, 
				maxRange = 20, 
				projectile = {id = ProjectileId.TORPEDO, pos = {x = 0, y = -.27, z = 4.6}, rot = {w = 1, x = 0, y = 0, z = 0}}
			},
			{
				type = WeaponClass.CRUISE_MISSILE, 
				rateOfFire = 500, 
				maxRange = 30, 
				fireFx = {
					{
						vfx = false,
						duration = 2500,
						path = PATH .. 'Sounds/Units/Submarines/missile.ogg', 
					}
				},
				projectile = {id = ProjectileId.CRUISE_MISSILE, pos = {x = 0, y = 2.44, z = -3.5}, rot = {w = .707, x = -.707, y = 0, z = 0}}
			},
		},
		unitClass = UnitClass.SUBMARINE,
		unitType = UnitType.UNDERWATER,
		isVehicle = true,
		health = 500,
		buildTime = 1000,
		cost = 500,
		size = {x = 2, y = 8.25, z = 2},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 5,
		name = 'Missile submarine',
		basePath = PATH .. vehiclePrefix .. 'Submarines/',
		meshPath = 'missileSubmarine.xml',
		selectionSfx = PATH .. 'Sounds/Units/Submarines/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		speed = .1,
		destinationOffset = .1,
		anglePrecision = .1,
		maxTurnAngle = .1,
		garrisonCategory = 3
	},
	{
		weapons = {
			{
				type = WeaponClass.TORPEDO, 
				projectile = {id = ProjectileId.TORPEDO, pos = {x = 0, y = -.27, z = 4.6}, rot = {w = 1, x = 0, y = 0, z = 0}},
				damage = 200, 
				maxRange = 20, 
				rateOfFire = 500, 
				fireFx = {
					{
						vfx = false,
						duration = 2500,
						path = PATH .. 'Sounds/Units/Submarines/fire.ogg', 
					}
				},
			}
		},
		unitClass = UnitClass.SUBMARINE,
		unitType = UnitType.UNDERWATER,
		isVehicle = true,
		health = 500,
		buildTime = 1000,
		cost = 500,
		size = {x = 1, y = 1.15, z = 2},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 5,
		name = 'Stealth submarine',
		basePath = PATH .. vehiclePrefix .. 'Submarines/',
		meshPath = 'stealthSubmarine.xml',
		selectionSfx = PATH .. 'Sounds/Units/Submarines/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		speed = .1,
		destinationOffset = .1,
		anglePrecision = .1,
		maxTurnAngle = .1,
		garrisonCategory = 3
	},
	{
		buildableUnits = {
			{id = UnitId.WAR_MECH, buildable = true}, 
			{id = UnitId.TANK, buildable = true}, 
			{id = UnitId.ARTILLERY, buildable = true}, 
			{id = UnitId.RESOURCE_ROVER, buildable = true}
		},
		unitClass = UnitClass.LAND_FACTORY,
		unitType = UnitType.LAND,
		isVehicle = false,
		health = 500,
		buildTime = 1000,
		cost = 500,
		size = {x = 1, y = 1.15, z = 2},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 5,
		name = 'Land factory',
		basePath = PATH .. structurePrefix .. 'LandFactories/',
		meshPath = 'landFactory.xml',
		guiScreen = 'landFactoryCommands.lua',
		selectionSfx = PATH .. 'Sounds/Units/Sample/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
	},
	{
		unitClass = UnitClass.NAVAL_FACTORY,
		unitType = UnitType.SEA_LEVEL,
		isVehicle = false,
		health = 500,
		buildTime = 1000,
		cost = 500,
		size = {x = 1, y = 1.15, z = 2},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 5,
		name = 'Naval factory',
		basePath = PATH .. structurePrefix .. 'NavalFactories/',
		meshPath = 'navalFactory.xml',
		guiScreen = 'navalFactoryCommands.lua',
		selectionSfx = PATH .. 'Sounds/Units/Sample/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
	},
	{
		unitClass = UnitClass.TRADE_CENTER,
		unitType = UnitType.LAND,
		isVehicle = false,
		health = 500,
		buildTime = 1000,
		cost = 500,
		size = {x = 1, y = 1.15, z = 2},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 5,
		name = 'Trade center',
		basePath = PATH .. structurePrefix .. 'TradeCenters/',
		meshPath = 'tradeCenter.xml',
		guiScreen = 'tradingCenterCommands.lua',
		selectionSfx = PATH .. 'Sounds/Units/Sample/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
	},
	{
		unitClass = UnitClass.LAB,
		unitType = UnitType.LAND,
		isVehicle = false,
		health = 500,
		buildTime = 1000,
		cost = 500,
		size = {x = 1, y = 1.15, z = 2},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 5,
		generationRate = 100,
		generationSpeed = 10,
		researchCost = 1,
		name = 'Lab',
		basePath = PATH .. structurePrefix .. 'Labs/',
		meshPath = 'lab.xml',
		guiScreen = 'researchStructCommands.lua',
		selectionSfx = PATH .. 'Sounds/Units/Sample/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
	},
	{
		weapons = {
			{
				type = WeaponClass.HITSCAN, 
				rateOfFire = 100, 
				damage = 50, 
				maxRange = 50,
				fireFx = {
					{
						vfx = true,
						duration = 50,
						mesh = {
							path = PATH .. vfxPrefix .. 'muzzleFlash.xml',
							color = {x = 1, y = 1, z = 0, a = 1},
						},
						pos = {x = 0, y = .43, z = 3.66},
						rot = {w = 1, x = 0, y = 0, z = 0},
						parent = 'turret'
						--scale = .5
					},
					{
						vfx = false,
						duration = 500,
						path = PATH .. 'Sounds/Units/WarMechs/fire.ogg',
					},
				},
				hitFx = {}
			}
		},
		unitClass = UnitClass.POINT_DEFENSE,
		unitType = UnitType.LAND,
		isVehicle = false,
		health = 500,
		buildTime = 1000,
		cost = 500,
		size = {x = 2, y = 10, z = 2},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 55,
		name = 'Point defense',
		basePath = PATH .. structurePrefix .. 'PointDefenses/',
		meshPath = 'pointDefense.xml',
		selectionSfx = PATH .. 'Sounds/Units/Sample/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
	},
	{
		unitClass = UnitClass.EXTRACTOR,
		unitType = UnitType.LAND,
		isVehicle = false,
		health = 500,
		buildTime = 1000,
		drawRate = 1,
		drawSpeed = 5,
		cost = 500,
		size = {x = 4, y = 1.15, z = 4},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 5,
		name = 'Extractor',
		basePath = PATH .. structurePrefix .. 'Extractors/',
		meshPath = 'extractor.xml',
		selectionSfx = PATH .. 'Sounds/Units/Sample/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
	},
	{
		unitClass = UnitClass.REFINERY,
		unitType = UnitType.LAND,
		isVehicle = false,
		health = 500,
		buildTime = 1000,
		cost = 500,
		size = {x = 1, y = 1.15, z = 2},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 5,
		name = 'Refinery',
		basePath = PATH .. structurePrefix .. 'Refineries/',
		meshPath = 'refinery.xml',
		selectionSfx = PATH .. 'Sounds/Units/Sample/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
	},
	{
		buildableUnits = {{id = UnitId.ENGINEER, buildable = true}},
		unitClass = UnitClass.FORT,
		unitType = UnitType.LAND,
		isVehicle = false,
		health = 1000,
		buildTime = 1000,
		cost = 1000,
		size = {x = 1, y = 1.15, z = 2},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 5,
		name = 'Fort',
		basePath = PATH .. structurePrefix .. 'Forts/',
		meshPath = 'fort2.xml',
		guiScreen = 'fortCommands.lua',
		selectionSfx = PATH .. 'Sounds/Units/Sample/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
	},
}
