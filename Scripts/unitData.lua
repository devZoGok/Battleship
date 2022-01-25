--dofile("defConfigs.lua")

numUnits = 11
UnitType = {VESSEL = 0, DESTROYER = 1, CRUISER = 2, AIRCRAFT_CARRIER = 3, SUBMARINE = 4, MISSILE_JET = 5, DEMO_JET = 6};
unitType = {
	UnitType.VESSEL, UnitType.VESSEL,
	UnitType.DESTROYER, UnitType.DESTROYER,
	UnitType.CRUISER, UnitType.CRUISER,
	UnitType.AIRCRAFT_CARRIER, UnitType.AIRCRAFT_CARRIER,
	UnitType.SUBMARINE,
	UnitType.MISSILE_JET, UnitType.DEMO_JET
};
health = {500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500}
speed = {.025, .025, .05, .05, .1, .1, .1, .1, .1, .1, .15}
destinationOffset = {.75, .75, 1, 1, .1, .1, .1, .1, .1, .5, .5}
anglePrecision = {.1, .1, .1, .1, .1, .1, .1, .1, .1, 3, 3}
cost = {500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500}
maxTurnAngle = {1, 1, .5, .5, 1, 1, .5, .5, 1, 2, 2}
range = {15, 15, 14, 14, 13, 13, 12, 12,15,25}

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

unitAxisLength = {8, 8,6,6,5, 5,8, 8, 7,2,2}
lineOfSight = {5, 8, 4, 4, 3, 3, 6, 6, 3, 3, 8}
name = {
	"Battleship", "Battleship", 
	"Destroyer", "Destroyer", 
	"Cruiser", "Cruiser", 
	"Aircraft carrier", "Aircraft carrier", 
	"Submarine", 
	"Jet", "Jet"
}
meshPath = {
	"battleship00.vb", "battleship01.vb",
	"destroyer00.vb", "destroyer01.vb", 
	"cruiser00.vb", "cruiser01.vb", 
	"aircraftCarrier00.vb", "aircraftCarrier01.vb",
	"submarine.vb",
	"jet00.x", "jet01.x"
}
PATH = ""
basePath = {
	PATH .. "Models/Battleships/", PATH .. "Models/Battleships/",
	PATH .. "Models/Destroyers/", PATH .. "Models/Destroyers/",
	PATH .. "Models/Cruisers/", PATH .. "Models/Cruisers/",
	PATH .. "Models/Aircraft carriers/", PATH .. "Models/Aircraft carriers/",
	PATH .. "Models/Submarines/",
	PATH .. "Models/Jets/", PATH .. "Models/Jets/"
};
