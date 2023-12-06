UnitClass = {WAR_MECH = 0, TANK = 1, ARTILLERY = 2, ENGINEER = 3, TRANSPORT = 4, CARGO_SHIP = 5, CARRIER = 6, SUBMARINE = 7, LAND_FACTORY = 8, NAVAL_FACTORY = 9, MARKET = 10, LAB = 11, POINT_DEFENSE = 12}
UnitType = {UNDERWATER = 0, SEA_LEVEL = 1, HOVER = 2, LAND = 3, AIR = 4}

modelPrefix = "Models/GameObjects/"
vehiclePrefix = modelPrefix .. "Units/Vehicles/"
structurePrefix = modelPrefix .. "Units/Structures/"

units = {
	num = 13,
	
	unitClass = {
		UnitClass.WAR_MECH,
		UnitClass.TANK,
		UnitClass.ARTILLERY,
		UnitClass.ENGINEER,
		UnitClass.TRANSPORT,
		UnitClass.CARGO_SHIP,
		UnitClass.CARRIER,
		UnitClass.SUBMARINE,
		UnitClass.LAND_FACTORY,
		UnitClass.NAVAL_FACTORY,
		UnitClass.MARKET,
		UnitClass.LAB,
		UnitClass.POINT_DEFENSE
	},
	
	unitType = {
		UnitType.LAND,
		UnitType.LAND,
		UnitType.LAND,
		UnitType.LAND,
		UnitType.HOVER,
		UnitType.HOVER,
		UnitType.SEA_LEVEL,
		UnitType.UNDERWATER,
		UnitType.LAND,
		UnitType.LAND,
		UnitType.LAND,
		UnitType.LAND,
		UnitType.LAND
	},
	
	isVehicle = {
		true, true, true, true, true, true, true, true, false, false, false, false, false
	},
	
	health = {500, 600, 200, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500},
	cost = {500, 600, 200, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500},
	range = {15, 20, 45, 15, 14, 14, 13, 13, 12, 12, 15, 0, 40},
	damage = {15, 120, 250, 15, 14, 14, 13, 13, 12, 12, 15, 100, 100},
	rateOfFire = {50, 2000, 5000, 100, 100, 100, 100, 100, 0, 0, 0, 0, 100},
	garrisonCapacity = {0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0},
	
	unitCornerPoints = {
	    {
					{x = 1.45, y = -4.11, z = -1.21}, 
					{x = -1.45, y = -4.11, z = -1.21}, 
					{x = -1.45, y = -4.11, z = 1.21}, 
					{x = 1.45, y = -4.11, z = 1.21}, 
					{x = 1.45, y = 4.11, z = -1.21}, 
					{x = -1.45, y = 4.11, z = -1.21}, 
					{x = -1.45, y = 4.11, z = 1.21}, 
					{x = 1.45, y = 4.11, z = 1.21}
	    },
	    {
					{x = 1.45, y = -4.11, z = -1.21}, 
					{x = -1.45, y = -4.11, z = -1.21}, 
					{x = -1.45, y = -4.11, z = 1.21}, 
					{x = 1.45, y = -4.11, z = 1.21}, 
					{x = 1.45, y = 4.11, z = -1.21}, 
					{x = -1.45, y = 4.11, z = -1.21}, 
					{x = -1.45, y = 4.11, z = 1.21}, 
					{x = 1.45, y = 4.11, z = 1.21}
	    },
	    {
					{x = 1.45, y = -4.11, z = -1.21}, 
					{x = -1.45, y = -4.11, z = -1.21}, 
					{x = -1.45, y = -4.11, z = 1.21}, 
					{x = 1.45, y = -4.11, z = 1.21}, 
					{x = 1.45, y = 4.11, z = -1.21}, 
					{x = -1.45, y = 4.11, z = -1.21}, 
					{x = -1.45, y = 4.11, z = 1.21}, 
					{x = 1.45, y = 4.11, z = 1.21}
	    },
	    {
					{x = .5, y = -.5, z = -7.05}, 
					{x = -.5, y = -.5, z = -7.05}, 
					{x = -.5, y = -.5, z = 6.4}, 
					{x = .5, y = -.5, z = 6.4},
					{x = .5, y = 3, z = -7.05}, 
					{x = -.5, y = 3, z = -7.05}, 
					{x = -.5, y = 3, z = 6.4}, 
					{x = .5, y = 3, z = 6.4}
	    },
	    {
					{x = .5, y = -.5, z = -3.4}, 
					{x = -.5, y = -.5, z = -3.4}, 
					{x = -.5, y = -.5, z = 3.4},
					{x = .5, y = -.4, z = 3.4}, 
					{x = .5, y = 2.1, z = -3.4}, 
					{x = -.5, y = 2, z = -3.4},
					{x = -.5, y = 2, z = 3.4}, 
					{x = .5, y = 2, z = 3.4}
	    },
	    {
					{x = 1, y = -.25, z = -1}, 
					{x = -1, y = -.25, z = -1}, 
					{x = -1, y = -.25, z = 1}, 
					{x = 1, y = -.25, z = 1}, 
					{x = 1, y = .8, z = -1}, 
					{x = -1, y = .8, z = -1}, 
					{x = -1, y = .8, z = 1}, 
					{x = 1, y = .8, z = 1}
	    },
	    {
					{x = .7, y = -.3, z = -4}, 
					{x = -.7, y = -.3, z = -4}, 
					{x = -.7, y = -.3, z = 3.4}, 
					{x = .7, y = -.3, z = 3.4}, 
					{x = .7, y = 2.9, z = -4}, 
					{x = -.7, y = 2.9, z = -4}, 
					{x = -.7, y = 2.9, z = 3.4}, 
					{x = .7, y = 2.9, z = 3.4}
	    },
	    {
					{x = .5, y = -.5, z = -6.1}, 
					{x = -.5, y = -.5, z = -6.1}, 
					{x = -.5, y = -.5, z = 5.2}, 
					{x = .5, y = -.5, z = 5.2}, 
					{x = .5, y = 2.2, z = -6.1}, 
					{x = -.5, y = 2.2, z = -6.1}, 
					{x = -.5, y = 2.2, z = 5.2}, 
					{x = .5, y = 2.2, z = 5.2}
	    },
	    {
					{x = 2, y = -.5, z = -10}, 
					{x = -2, y = -.5, z = -10}, 
					{x = -2, y = -.5, z = 9}, 
					{x = 2, y = -.5, z = 9}, 
					{x = 2, y = 5.5, z = -10}, 
					{x = -2, y = 5.5, z = -10}, 
					{x = -2, y = 5.5, z = 9}, 
					{x = 2, y = 5.5, z = 9}
	    },
	    {
					{x = 2, y = -1, z = -10}, 
					{x = -2, y = -1, z = -10}, 
					{x = -2, y = -1, z = 9}, 
					{x = 2, y = -1, z = 9}, 
					{x = 2, y = 4.5, z = -10}, 
					{x = -2, y = 4.5, z = -10}, 
					{x = -2, y = 4.5, z = 9}, 
					{x = 2, y = 4.5, z = 9}
	    },
	    {
					{x = .25, y = -.5, z = -2}, 
					{x = -.25, y = -.5, z = -2}, 
					{x = -.25, y = -.5, z = 3}, 
					{x = .25, y = -.5, z = 3}, 
					{x = .25, y = .7, z = -2}, 
					{x = -.25, y = .7, z = -2}, 
					{x = -.25, y = .7, z = 3}, 
					{x = .25, y = .7, z = 3}
	    },
	    {
					{x = .6, y =-.1, z =-1.3},
					{x = -.6, y =-.1, z =-1.3},
					{x = -.6, y =-.1, z =.8},
					{x = .6, y =-.1, z =.8},
					{x = .6, y =.3, z =-1.3},
					{x = -.6, y =.3, z =-1.3},
					{x = -.6, y =.3, z =.8},
					{x = .6, y =.3, z =.8}
	    },
	    {
					{x = .5, y =-.1, z =-1.3},
					{x = -.5, y =-.1, z =-1.3},
					{x = -.5, y =-.1, z =.7},
					{x = .5, y =-.1, z =.7},
					{x = .5, y =.15, z =-1.3},
					{x = -.5, y =.15, z =-1.3},
					{x = -.5, y =.15, z =.7},
					{x = .5, y =.15, z =.7}
	    },
	    {
					{x = .5, y =-.1, z =-1.3},
					{x = -.5, y =-.1, z =-1.3},
					{x = -.5, y =-.1, z =.7},
					{x = .5, y =-.1, z =.7},
					{x = .5, y =.15, z =-1.3},
					{x = -.5, y =.15, z =-1.3},
					{x = -.5, y =.15, z =.7},
					{x = .5, y =.15, z =.7}
	    },
	    {
					{x = .5, y =-.1, z =-1.3},
					{x = -.5, y =-.1, z =-1.3},
					{x = -.5, y =-.1, z =.7},
					{x = .5, y =-.1, z =.7},
					{x = .5, y =.15, z =-1.3},
					{x = -.5, y =.15, z =-1.3},
					{x = -.5, y =.15, z =.7},
					{x = .5, y =.15, z =.7}
	    },
	    {
					{x = .5, y =-.1, z =-1.3},
					{x = -.5, y =-.1, z =-1.3},
					{x = -.5, y =-.1, z =.7},
					{x = .5, y =-.1, z =.7},
					{x = .5, y =.15, z =-1.3},
					{x = -.5, y =.15, z =-1.3},
					{x = -.5, y =.15, z =.7},
					{x = .5, y =.15, z =.7}
	    },
	    {
					{x = .5, y =-.1, z =-1.3},
					{x = -.5, y =-.1, z =-1.3},
					{x = -.5, y =-.1, z =.7},
					{x = .5, y =-.1, z =.7},
					{x = .5, y =.15, z =-1.3},
					{x = -.5, y =.15, z =-1.3},
					{x = -.5, y =.15, z =.7},
					{x = .5, y =.15, z =.7}
	    }
	},
	unitCuboidDimensions = {
	    {
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}
	    },
	    {
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}
	    },
	    {
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}
	    },
	    {
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}
	    },
	    {
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}
	    },
	    {
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}
	    },
	    {
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}
	    },
	    {
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}
	    },
	    {
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}
	    },
	    {
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}, 
					{x = 1, y = 1, z = 1}
	    },
	    {
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}
	    },
	    {
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}
	    },
	    {
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1},
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}
	    },
	    {
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1},
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}
	    },
	    {
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1},
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}
	    },
	    {
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1},
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}
	    },
	    {
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1},
					{x = .1, y = .1, z = .1}, 
					{x = .1, y = .1, z = .1}
	    }
	},
	
	unitAxisLength = {8, 8, 8, 8, 6, 6, 5, 5, 8, 8, 7, 2, 2, 2, 2, 2, 2},
	lineOfSight = {5, 8, 4, 4, 3, 3, 6, 6, 3, 3, 8, 1, 1, 1, 1},
	name = {
		'War mech',
		'Tank',
		'Artillery',
		'Engineer',
		'Transport',
		'Cargo ship',
		'Carrier',
		'Submarine',
		'Land factory',
		'Naval factory',
		'Market',
		'Lab',
		'Point defense',
	},
	meshPath = {
		'warMech.xml',
		'tank.xml',
		'artillery.xml',
		'engineer.xml',
		'transport.xml',
		'cargoship.xml',
		'carrier.xml',
		'submarine.xml',
		'landFactory.xml',
		'navalFactory.xml',
		'market.xml',
		'lab.xml',
		'pointDefense.xml',
	},
	basePath = {
		PATH .. vehiclePrefix .. 'WarMechs/',
		PATH .. vehiclePrefix .. 'Tanks/',
		PATH .. vehiclePrefix .. 'Artillery/',
		PATH .. vehiclePrefix .. 'Engineers/',
		PATH .. vehiclePrefix .. 'Transports/',
		PATH .. vehiclePrefix .. 'Cargoships/',
		PATH .. vehiclePrefix .. 'Carriers/',
		PATH .. vehiclePrefix .. 'Submarines/',
		PATH .. structurePrefix .. 'LandFactory/',
		PATH .. structurePrefix .. 'NavalFactory/',
		PATH .. structurePrefix .. 'Market/',
		PATH .. structurePrefix .. 'Lab/',
		PATH .. structurePrefix .. 'PointDefenses/'
	},
	selectionSfx = {
		PATH .. 'Sounds/Units/WarMechs/selection.ogg',
		PATH .. 'Sounds/Units/Tanks/selection.ogg',
		PATH .. 'Sounds/Units/Tanks/selection.ogg',
		PATH .. 'Sounds/Units/Engineers/selection.ogg',
		PATH .. 'Sounds/Units/Transports/selection.ogg',
		PATH .. 'Sounds/Units/Cargoships/selection.ogg',
		PATH .. 'Sounds/Units/Carriers/selection.ogg',
		PATH .. 'Sounds/Units/Submarines/selection.ogg',
		PATH .. 'Sounds/Units/Sample/selection.ogg',
		PATH .. 'Sounds/Units/Sample/selection.ogg',
		PATH .. 'Sounds/Units/Sample/selection.ogg',
		PATH .. 'Sounds/Units/Sample/selection.ogg',
		PATH .. 'Sounds/Units/Sample/selection.ogg',
	},
	fireSfx = {
		PATH .. 'Sounds/Units/WarMechs/fire.ogg',
		PATH .. 'Sounds/Units/Tanks/attack.ogg',
		PATH .. 'Sounds/Units/Tanks/attack.ogg',
		PATH .. 'Sounds/Units/WarMechs/fire.ogg',
		PATH .. 'Sounds/Units/WarMechs/fire.ogg',
		PATH .. 'Sounds/Units/WarMechs/fire.ogg',
		PATH .. 'Sounds/Units/WarMechs/fire.ogg',
		PATH .. 'Sounds/Units/WarMechs/fire.ogg',
		PATH .. 'Sounds/Units/WarMechs/fire.ogg',
		PATH .. 'Sounds/Units/WarMechs/fire.ogg',
		PATH .. 'Sounds/Units/WarMechs/fire.ogg',
		PATH .. 'Sounds/Units/WarMechs/fire.ogg',
		PATH .. 'Sounds/Units/WarMechs/fire.ogg',
	},
	deathSfx = {
		PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
		PATH .. 'Sounds/SFX/Explosions/explosion01.ogg',
	}
}
