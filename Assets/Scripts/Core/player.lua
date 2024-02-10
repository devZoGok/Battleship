Player.numStartEngis = 3
Player.dirToCenter = nil 
Player.numDefWarMechs = 5
Player.numTaskForceWarMechs = 5
Player.numTaskForceTanks = 5
Player.numTaskForceArtillery = 5
Player.givenOrder = false

--TODO use enum-like values instead of literals for order types
--TODO simplify building construction
function Player:buildFort()
	if #self:getUnitsByClass(UnitClass.FORT, 1) > 0 then
		return true
	end

	engis = self:getUnitsByClass(UnitClass.ENGINEER, 1)
	if #engis == 0 then return false end

	engineer = engis[1]
	self:deselectUnits()
	self:selectUnits({engineer})

	fort = GameObjectFactory.createUnit(self, UnitId.FORT, self:getSpawnPoint(), Quaternion:new(1, 0, 0, 0), 0)
	self:addUnit(fort)
	self:issueOrder(1, Vector3:new(0, 0, 0), {Target:new(fort, Vector3:new(0, 0, 0))}, false)

	return true
end

function Player:trainEngineers()
	forts = self:getUnitsByClass(UnitClass.FORT, 1)

	if #forts == 0 or (#forts > 0 and forts[1]:toFactory():getBuildStatus() < 100) then
		return false
	end

	fort = forts[1]:toFactory()
	dirToCenter = Vector3:new(0, 0, 0):subtr(fort:getPos()):norm()

	while #fort:getQueue() + #self:getUnitsByClass(UnitClass.ENGINEER, -1) < self.numStartEngis do
		fort:appendToQueue(UnitId.ENGINEER)
	end

	return #self:getUnitsByClass(UnitClass.ENGINEER, -1) == self.numStartEngis
end

--Correct resource deposit selection
function Player:buildExtractor()
	if #self:getUnitsByClass(UnitClass.EXTRACTOR, 1) > 0 then
		return true
	end

	engineer = self:getUnitsByClass(UnitClass.ENGINEER, -1)[1]
	self:deselectUnits()
	self:selectUnits({engineer})

	deposits = self:getResourceDeposits()
	depPos = nil

	if #deposits > 0 then
		depPos = deposits[1]:getPos()
	end

	if depPos == nil then
		return false
	end

	extractor = GameObjectFactory.createUnit(self, UnitId.EXTRACTOR, depPos, Quaternion:new(1, 0, 0, 0), 0)
	self:addUnit(extractor)
	self:issueOrder(1, Vector3:new(0, 0, 0), {Target:new(extractor, Vector3:new(0, 0, 0))}, false)
 
	return true
end

function Player:startBuilding(buildingId, engiId, angle)
	engineer = self:getUnitsByClass(UnitClass.ENGINEER, -1)[engiId]
	self:deselectUnits()
	self:selectUnits({engineer})
	
	dirVec = Quaternion:new(angle, Vector3:new(0, 1, 0)):multVec(dirToCenter)
	refPos = self:getSpawnPoint():add(dirVec:mult(20))
	building = GameObjectFactory.createUnit(self, buildingId, refPos, Quaternion:new(1, 0, 0, 0), 0)
	self:addUnit(building)
	self:issueOrder(1, Vector3:new(0, 0, 0), {Target:new(building, Vector3:new(0, 0, 0))}, false)
end

function Player:buildRefinery()
	if #self:getUnitsByClass(UnitClass.REFINERY, 1) == 0 then
		self:startBuilding(UnitId.REFINERY, 2, -.6)
	end

	refineries = self:getUnitsByClass(UnitClass.REFINERY, -1)
	refineryBuilt = false

	for i = 1, #refineries do
		if refineries[i]:toStructure():getBuildStatus() == 100 then
			refineryBuilt = true
			break
		end
	end

	return refineryBuilt
end

function Player:buildLandFactory()
	if #self:getUnitsByClass(UnitClass.LAND_FACTORY, 1) == 0 then
		self:startBuilding(UnitId.LAND_FACTORY, 3, .6)
	end

	factories = self:getUnitsByClass(UnitClass.LAND_FACTORY, -1)
	factoryBuilt = false

	for i = 1, #factories do
		if factories[i]:toStructure():getBuildStatus() == 100 then
			factoryBuilt = true
			break
		end
	end

	return factoryBuilt
end

function Player:buildHarvester()
	if #self:getUnitsByClass(UnitClass.RESOURCE_ROVER, 1) > 0 then
		return true
	end

	landFactories = self:getUnitsByClass(UnitClass.LAND_FACTORY, 1)
	if #landFactories == 0 then return false end

	factory = landFactories[1]:toFactory()

	if #factory:getQueue() == 0 then
		factory:appendToQueue(UnitId.RESOURCE_ROVER)
	end

	return false
end

function Player:startHarvesting()
	harvesters = self:getUnitsByClass(UnitClass.RESOURCE_ROVER, -1)

	for i = 1, #harvesters do
		if harvesters[i]:getNumOrders() > 0 and harvesters[i]:getOrder(0).type == 7 then
			return true
		end
	end

	extrs = self:getUnitsByClass(UnitClass.EXTRACTOR, 1)
	if #extrs == 0 then return false end

	self:deselectUnits()
	self:selectUnits({harvesters[1]})
	self:issueOrder(7, Vector3:new(0, 0, 0), {Target:new(extrs[1], Vector3:new(0, 0, 0))}, false)

	return true
end

function Player:buildTaskforceUnitGroup(numUnits, currNumUnits, factory, unitId)
	if currNumUnits < numUnits then
		for i = 1, numUnits - currNumUnits do
			factory:appendToQueue(unitId)
		end
	end
end

function Player:buildTaskforce()
	currNumBuiltWarMechs = #self:getUnitsByClass(UnitClass.WAR_MECH, -1)
	currNumBuiltTanks = #self:getUnitsByClass(UnitClass.TANK, -1)
	currNumBuiltArtillery = #self:getUnitsByClass(UnitClass.ARTILLERY, -1)
	currNumWarMechs = currNumBuiltWarMechs
	currNumTanks = currNumBuiltTanks
	currNumArtillery = currNumBuiltArtillery
	currLandFactories = self:getUnitsByClass(UnitClass.LAND_FACTORY, -1)

	for i = 1, #currLandFactories do
		currNumWarMechs = currNumWarMechs + currLandFactories[i]:toFactory():getNumQueueUnitsById(UnitId.WAR_MECH)
		currNumTanks = currNumTanks + currLandFactories[i]:toFactory():getNumQueueUnitsById(UnitId.TANK)
		currNumArtillery = currNumArtillery + currLandFactories[i]:toFactory():getNumQueueUnitsById(UnitId.ARTILLERY)
	end

	numWarMechs = self.numDefWarMechs + self.numTaskForceWarMechs
	numTanks = self.numTaskForceTanks
	numArtillery = self.numTaskForceArtillery

	if currNumBuiltWarMechs >= numWarMechs and currNumBuiltTanks >= numTanks and currNumBuiltArtillery >= numArtillery then
		return true
	end

	landFactory = currLandFactories[1]:toFactory()

	self:buildTaskforceUnitGroup(numWarMechs, currNumWarMechs, landFactory, UnitId.WAR_MECH)
	self:buildTaskforceUnitGroup(numTanks, currNumTanks, landFactory, UnitId.TANK)
	self:buildTaskforceUnitGroup(numArtillery, currNumArtillery, landFactory, UnitId.ARTILLERY)

	return false
end

function Player:sendTaskforce()
	if --[[taskForceMechs[1]:getNumOrders() > 0 and taskForceMechs[1]:getOrder(0).type == 2--]] self.givenOrder then
		return true
	end

	taskForceMechs = self:getUnitsByClass(UnitClass.WAR_MECH, self.numTaskForceWarMechs)
	taskForceTanks = self:getUnitsByClass(UnitClass.TANK, self.numTaskForceTanks)
	taskForceArtillery = self:getUnitsByClass(UnitClass.ARTILLERY, self.numTaskForceArtillery)

	self:deselectUnits()
	self:selectUnits(taskForceMechs)
	self:selectUnits(taskForceTanks)
	self:selectUnits(taskForceArtillery)

	players = Game.getSingleton():getPlayers()
	enemySpawnPoint = nil

	for i = 1, #players do
		if players[i] ~= self then
			enemySpawnPoint = players[i]:getSpawnPoint()
			break
		end
	end

	self:issueOrder(2, Vector3:new(0, 0, 0), {Target:new(nil, enemySpawnPoint)}, false)
	self.givenOrder = true

	return true
end

Player.behaviour = {
	type = BTNodeType.SEQUENCE,
	children = {
		{type = BTNodeType.FUNCTION, func = 'buildFort'},
		{type = BTNodeType.FUNCTION, func = 'trainEngineers'},
		{type = BTNodeType.FUNCTION, func = 'buildExtractor'},
		{type = BTNodeType.FUNCTION, func = 'buildRefinery'},
		{type = BTNodeType.FUNCTION, func = 'buildLandFactory'},
		{type = BTNodeType.FUNCTION, func = 'buildHarvester'},
		{type = BTNodeType.FUNCTION, func = 'startHarvesting'},
		{type = BTNodeType.FUNCTION, func = 'buildTaskforce'},
		{type = BTNodeType.FUNCTION, func = 'sendTaskforce'},
	}
}
