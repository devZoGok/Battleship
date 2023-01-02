numUnits = 4

UnitClass = {VESSEL = 0, ENGINEER = 1, SAMPLE_STRUCTURE = 2};

UnitType = {UNDERWATER = 0, SEA_LEVEL = 1, LAND = 2, AIR = 3};

unitClass = {
	UnitClass.VESSEL, UnitClass.VESSEL,
	UnitClass.ENGINEER, UnitClass.SAMPLE_STRUCTURE 
};

unitType = {
	UnitType.SEA_LEVEL, UnitType.UNDERWATER,
	UnitType.LAND, UnitType.LAND 
}

isVehicle = {
	true, true, true, false
};

health = {500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500}
cost = {500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500}
range = {15, 15, 14, 14, 13, 13, 12, 12, 15, 25}

unitCornerPoints = {
    {
				{x = .9, y = -1, z = -6}, 
				{x = -.9, y = -1, z = -6}, 
				{x = -.9, y = -1, z = 5.8}, 
				{x = .9, y = -1, z = 5.8}, 
				{x = .9, y = 3.5, z = -6}, 
				{x = -.9, y = 3.5, z = -6}, 
				{x = -.9, y = 3.5, z = 5.8}, 
				{x = .9, y = 3.5, z = 5.8}
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
				{x = .5, y = -.25, z = -4.8}, 
				{x = -.5, y = -.25, z = -4.8}, 
				{x = -.5, y = -.25, z = 4.7}, 
				{x = .5, y = -.25, z = 4.7}, 
				{x = .5, y = .8, z = -4.8}, 
				{x = -.5, y = .8, z = -4.8}, 
				{x = -.5, y = .8, z = 4.7}, 
				{x = .5, y = .8, z = 4.7}
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
};
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
};

unitAxisLength = {8, 8, 6, 6, 5, 5, 8, 8, 7, 2, 2}
lineOfSight = {5, 8, 4, 4, 3, 3, 6, 6, 3, 3, 8}
name = {
	"Battleship", "Battleship", 
	"Battleship", "Sample", 
	"Cruiser", "Cruiser", 
	"Aircraft carrier", "Aircraft carrier", 
	"Submarine", 
	"Jet", "Jet"
}
meshPath = {
	"battleship00.xml", "battleship00.xml",
	"battleship00.xml", "sample.xml", 
	"cruiser00.xml", "cruiser01.xml", 
	"aircraftCarrier00.xml", "aircraftCarrier01.xml",
	"submarine.xml",
	"jet00.xml", "jet01.xml"
}
modelPrefix = "Models/"
vehiclePrefix = modelPrefix .. "Units/Vehicles/"
structurePrefix = modelPrefix .. "Units/Structures/"
basePath = {
	PATH .. vehiclePrefix .. "Battleships/", PATH .. vehiclePrefix .. "Battleships/",
	PATH .. vehiclePrefix .. "Battleships/", PATH .. structurePrefix .. "Sample/",
	PATH .. vehiclePrefix .. "Cruisers/", PATH .. vehiclePrefix .. "Cruisers/",
	PATH .. vehiclePrefix .. "Aircraft carriers/", PATH .. vehiclePrefix .. "Aircraft carriers/",
	PATH .. vehiclePrefix .. "Submarines/",
	PATH .. vehiclePrefix .. "Jets/", PATH .. vehiclePrefix .. "Jets/"
};
