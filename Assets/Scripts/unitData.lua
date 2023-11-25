UnitClass = {WAR_MECH = 0, ENGINEER = 1, TRANSPORT = 2, CARGO_SHIP = 3, CARRIER = 4, SUBMARINE = 5, SAMPLE_STRUCTURE = 6}
UnitType = {UNDERWATER = 0, SEA_LEVEL = 1, HOVER = 2, LAND = 3, AIR = 4}

modelPrefix = "Models/GameObjects/"
vehiclePrefix = modelPrefix .. "Units/Vehicles/"
structurePrefix = modelPrefix .. "Units/Structures/"

units = {
	num = 7,
	
	unitClass = {
		UnitClass.WAR_MECH,
		UnitClass.ENGINEER,
		UnitClass.TRANSPORT,
		UnitClass.CARGO_SHIP,
		UnitClass.CARRIER,
		UnitClass.SUBMARINE,
		UnitClass.SAMPLE_STRUCTURE
	},
	
	unitType = {
		UnitType.LAND,
		UnitType.LAND,
		UnitType.HOVER,
		UnitType.HOVER,
		UnitType.SEA_LEVEL,
		UnitType.UNDERWATER,
		UnitType.LAND
	},
	
	isVehicle = {
		true, true, true, true, true, true, false
	},
	
	health = {500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500},
	cost = {500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500},
	range = {15, 15, 14, 14, 13, 13, 12, 12, 15, 25},
	rateOfFire = {50, 100, 100, 100, 100, 100, 100},
	garrisonCapacity = {0, 0, 3, 0, 0, 0, 0},
	
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
	
	unitAxisLength = {8, 8, 6, 6, 5, 5, 8, 8, 7, 2, 2},
	lineOfSight = {5, 8, 4, 4, 3, 3, 6, 6, 3, 3, 8},
	name = {
		'War mech',
		'Engineer',
		'Transport',
		'Cargo ship',
		'Carrier',
		'Submarine',
		'Sample structure',
	},
	meshPath = {
		'warMech.xml',
		'engineer.xml',
		'transport.xml',
		'cargoship.xml',
		'carrier.xml',
		'submarine.xml',
		'sample.xml',
	},
	basePath = {
		PATH .. vehiclePrefix .. 'WarMechs/',
		PATH .. vehiclePrefix .. 'Engineers/',
		PATH .. vehiclePrefix .. 'Transports/',
		PATH .. vehiclePrefix .. 'Cargoships/',
		PATH .. vehiclePrefix .. 'Carriers/',
		PATH .. vehiclePrefix .. 'Submarines/',
		PATH .. structurePrefix .. 'Sample/'
	},
	selectionSfx = {
		PATH .. 'Sounds/Units/WarMechs/selection.ogg',
		PATH .. 'Sounds/Units/Engineers/selection.ogg',
		PATH .. 'Sounds/Units/Transports/selection.ogg',
		PATH .. 'Sounds/Units/Cargoships/selection.ogg',
		PATH .. 'Sounds/Units/Carriers/selection.ogg',
		PATH .. 'Sounds/Units/Submarines/selection.ogg',
		PATH .. 'Sounds/Units/Sample/selection.ogg',
	},
	fireSfx = {
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
	}
}
