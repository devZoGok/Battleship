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
						pos = {x = 0.428, y = 5.484, z = 3.08},
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
		size = {x = 6, y = 9.22, z = 2.42},
		hitboxOffset = {x = 0, y = 4.5, z = 0},
		lineOfSight = 250,
		name = 'War mech',
		basePath = PATH .. vehiclePrefix .. 'WarMechs/',
		meshPath = 'warMech.xml',
		albedoPath = 'mech.jpg',
		colorNodes = {'WarRobotTemplate.001'},
		selectionSfx = PATH .. 'Sounds/Units/WarMechs/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		speed = .3,
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
						pos = {x = 0.06, y = 5.82, z = 11.4},
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
		size = {x = 9.57, y = 7, z = 20.1},
		hitboxOffset = {x = 0, y = 3, z = 1.06},
		lineOfSight = 25,
		name = 'Tank',
		basePath = PATH .. vehiclePrefix .. 'Tanks/',
		meshPath = 'tank2.xml',
		albedoPath = 'tank.jpg',
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
						pos = {x = 1, y = 5.8, z = 3.9},
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
		size = {x = 6.8, y = 6.8, z = 15.2},
		hitboxOffset = {x = 0, y = 1, z = 0},
		lineOfSight = 30,
		name = 'Artillery',
		basePath = PATH .. vehiclePrefix .. 'Artillery/',
		meshPath = 'artillery.xml',
		albedoPath = 'artillery.jpg',
		colorNodes = {'turret'},
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
			{id = UnitId.REFINERY, buildable = true},
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
		size = {x = 1, y = 8, z = 1},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 20,
		name = 'Engineer',
		basePath = PATH .. vehiclePrefix .. 'Engineers/',
		albedoPath = 'engineer.jpg',
		colorNodes = {'Cube.008'},
		meshPath = 'engineer2.xml',
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
		size = {x = 21, y = 17.5, z = 56.6},
		garrisonCapacity = {2, 2, 2},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 5,
		colorNodes = {'TransportTemplateOBJ.001'},
		albedoPath = 'transport.jpg',
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
		size = {x = 17.5, y = 16.5, z = 63.5},
		hitboxOffset = {x = 0, y = 0, z = 0},
		lineOfSight = 5,
		colorNodes = {'Cube.002'},
		name = 'Hover transport',
		basePath = PATH .. vehiclePrefix .. 'Transports/',
		meshPath = 'hoverTransport.xml',
		albedoPath = 'transport.jpg',
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
		size = {x = 3.29, y = 2.23, z = 9.5},
		hitboxOffset = {x = 0, y = .37, z = 0},
		lineOfSight = 5,
		name = 'Cargo ship',
		colorNodes = {'CargoshipTemplateOBJ.001'},
		basePath = PATH .. vehiclePrefix .. 'Cargoships/',
		meshPath = 'cargoship.xml',
		albedoPath = 'cargoship.jpg',
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
		size = {x = 18.8, y = 17.5, z = 52.6},
		hitboxOffset = {x = 0, y = 1.9, z = 0},
		lineOfSight = 5,
		colorNodes = {'tacticalCruiser.001'},
		name = 'Tactical cruiser',
		basePath = PATH .. vehiclePrefix .. 'Cruisers/',
		meshPath = 'tacticalCruiser.xml',
		albedoPath = 'cruiser.jpg',
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
		size = {x = 17, y = 17, z = 45.1},
		hitboxOffset = {x = 0, y = 3.78, z = 0},
		lineOfSight = 5,
		colorNodes = {'defensiveCruiser.001'},
		name = 'Defensive cruiser',
		basePath = PATH .. vehiclePrefix .. 'Cruisers/',
		meshPath = 'defensiveCruiser.xml',
		albedoPath = 'cruiser.jpg',
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
		size = {x = 21.1, y = 18.1, z = 28.6},
		hitboxOffset = {x = 0, y = 4.19, z = 0},
		lineOfSight = 5,
		colorNodes = {'cheapCruiser.001'},
		name = 'Cheap cruiser',
		basePath = PATH .. vehiclePrefix .. 'Cruisers/',
		meshPath = 'cheapCruiser.xml',
		albedoPath = 'cruiser.jpg',
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
		size = {x = 17.9, y = 14.9, z = 45.6},
		hitboxOffset = {x = 0, y = 2.98, z = 1.21},
		lineOfSight = 5,
		colorNodes = {'transportCruiser.001'},
		name = 'Transport cruiser',
		basePath = PATH .. vehiclePrefix .. 'Cruisers/',
		meshPath = 'transportCruiser.xml',
		albedoPath = 'cruiser.jpg',
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
		size = {x = 37.8, y = 16.7, z = 87},
		hitboxOffset = {x = 0, y = 6.38, z = 0},
		lineOfSight = 5,
		name = 'Heavy carrier',
		colorNodes = {'heavyCarrier.001'},
		basePath = PATH .. vehiclePrefix .. 'Carriers/',
		meshPath = 'heavyCarrier.xml',
		albedoPath = 'carrier.jpg',
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
				projectile = {id = ProjectileId.CRUISE_MISSILE, pos = {x = 0, y = 15, z = 0}, rot = {w = .707, x = -.707, y = 0, z = 0}},
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
		size = {x = 26, y = 21.1, z = 81.8},
		hitboxOffset = {x = 0, y = 4.13, z = 0},
		lineOfSight = 5,
		name = 'Champion carrier',
		colorNodes = {'Models4.001'},
		basePath = PATH .. vehiclePrefix .. 'Carriers/',
		meshPath = 'championCarrier.xml',
		albedoPath = 'carrier.jpg',
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
				projectile = {id = ProjectileId.CRUISE_MISSILE, pos = {x = 0, y = 15, z = -3.5}, rot = {w = .707, x = -.707, y = 0, z = 0}}
			},
		},
		unitClass = UnitClass.SUBMARINE,
		unitType = UnitType.UNDERWATER,
		isVehicle = true,
		health = 500,
		buildTime = 1000,
		cost = 500,
		size = {x = 14.1, y = 29.2, z = 59.5},
		hitboxOffset = {x = 0, y = 6.7, z = 0},
		lineOfSight = 50,
		name = 'Missile submarine',
		basePath = PATH .. vehiclePrefix .. 'Submarines/',
		meshPath = 'missileSubmarine.xml',
		albedoPath = 'submarine.jpg',
		colorNodes = {'missileSubmarine.001'},
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
		hitboxOffset = {x = 0, y = 1.25, z = 0},
		size = {x = 10.7, y = 15.4, z = 50},
		lineOfSight = 5,
		name = 'Stealth submarine',
		colorNodes = {'stealthSubmarine.001'},
		basePath = PATH .. vehiclePrefix .. 'Submarines/',
		meshPath = 'stealthSubmarine.xml',
		albedoPath = 'submarine.jpg',
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
		size = {x = 21.6, y = 11.9, z = 25.4},
		hitboxOffset = {x = 0, y = 1.02, z = .95},
		lineOfSight = 5,
		colorNodes = {'Cube.001'},
		name = 'Land factory',
		albedoPath = 'factory.jpg',
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
		albedoPath = 'factory.jpg',
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
		size = {x = 8.53, y = 51.9, z = 8.53},
		hitboxOffset = {x = 0, y = .97, z = 0},
		lineOfSight = 5,
		colorNodes = {'Cube.001'},
		name = 'Trade center',
		basePath = PATH .. structurePrefix .. 'TradeCenters/',
		meshPath = 'tradeCenter.xml',
		albedoPath = 'tradeCenter.jpg',
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
		size = {x = 40, y = 26.2, z = 40},
		hitboxOffset = {x = 0, y = 14, z = 0},
		lineOfSight = 50,
		generationRate = 100,
		generationSpeed = 10,
		researchCost = 1,
		name = 'Lab',
		colorNodes = {'Cube.008'},
		basePath = PATH .. structurePrefix .. 'Labs/',
		meshPath = 'lab2.xml',
		albedoPath = 'lab.jpg',
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
						pos = {x = 1.4, y = -.18, z = 5.4},
						rot = {w = 1, x = 0, y = 0, z = 0},
						parent = 'LeftBarrel'
						--scale = .5
					},
					{
						vfx = true,
						duration = 50,
						mesh = {
							path = PATH .. vfxPrefix .. 'muzzleFlash.xml',
							color = {x = 1, y = 1, z = 0, a = 1},
						},
						pos = {x = -1.4, y = -.18, z = 5.4},
						rot = {w = 1, x = 0, y = 0, z = 0},
						parent = 'RightBarrel'
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
		size = {x = 5, y = 8, z = 5},
		hitboxOffset = {x = 0, y = 4, z = 0},
		lineOfSight = 55,
		name = 'Point defense',
		colorNodes = {'Hoses', 'Hoses.001'},
		turretNode = 'TurretBody',
		basePath = PATH .. structurePrefix .. 'PointDefenses/',
		meshPath = 'pointDefense2.xml',
		albedoPath = 'pointDefense.jpg',
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
		colorNodes = {'Cube.001'},
		hitboxOffset = {x = 0, y = 1, z = 0},
		size = {x = 3.24, y = 3.38, z = 3.24},
		lineOfSight = 5,
		name = 'Extractor',
		basePath = PATH .. structurePrefix .. 'Extractors/',
		meshPath = 'extractor.xml',
		albedoPath = 'extractor.jpg',
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
		colorNodes = {'Cube.001'},
		size = {x = 15.1, y = 13.5, z = 25},
		hitboxOffset = {x = 0, y = 1.05, z = 1.85},
		lineOfSight = 5,
		name = 'Refinery',
		basePath = PATH .. structurePrefix .. 'Refineries/',
		meshPath = 'refinery.xml',
		albedoPath = 'refinery.jpg',
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
		size = {x = 43.3, y = 50, z = 43.3},
		hitboxOffset = {x = 0, y = .8, z = 0},
		lineOfSight = 5,
		name = 'Fort',
		basePath = PATH .. structurePrefix .. 'Forts/',
		meshPath = 'fort2.xml',
		colorNodes = {'Cube.004'},
		albedoPath = 'fort.jpg',
		guiScreen = 'fortCommands.lua',
		selectionSfx = PATH .. 'Sounds/Units/Sample/selection.ogg',
		deathSfx = PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
	},
}
