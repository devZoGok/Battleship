Player.numStartEngis = 3
Player.dirToCenter = nil 
Player.startedFort = false 
Player.finishedFort = false 
Player.startedRefinery = false 
Player.finishedRefinery = false 
Player.startedLandFactory = false 
Player.finishedLandFactory = false 
Player.startedNavalFactory = false 
Player.finishedNavalFactory = false 
Player.startedExtractor = false
Player.finishedExtractor = false
Player.startedHarvesting = false
Player.startedBuildingTaskforce = false
Player.finishedBuildingTaskforce = false

--TODO use enum-like values instead of literals for order types
--TODO simplify building construction
function Player:buildFort()
	if self.startedFort then
		return true
	end

	engis = self:getUnitsByClass(UnitClass.ENGINEER, 1)

	if #engis == 0 then
		return false
	end

	engineer = engis[1]
	self:selectUnits({engineer})

	fort = GameObjectFactory.createUnit(self, UnitId.FORT, engineer:getPos(), Quaternion:new(1, 0, 0, 0), 0)
	self:addUnit(fort)
	self:issueOrder(1, Vector3:new(0, 0, 0), {Target:new(fort, Vector3:new(0, 0, 0))}, false)

	self.startedFort = true
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

	return #self:getUnitsByClass(UnitClass.ENGINEER, -1) == 3
end

function Player:buildExtractor()
	if self.startedExtractor then
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
	self.startedExtractor = true
 
	return true
end

function Player:startBuilding(buildingId, engiId, angle)
	engineer = self:getUnitsByClass(UnitClass.ENGINEER, -1)[engiId]
	self:deselectUnits()
	self:selectUnits({engineer})
	
	dirVec = Quaternion:new(angle, Vector3:new(0, 1, 0)):multVec(dirToCenter)
	refPos = self:getSpawnPoint():add(dirVec:mult(20))
	refinery = GameObjectFactory.createUnit(self, buildingId, refPos, Quaternion:new(1, 0, 0, 0), 0)
	self:addUnit(refinery)
	self:issueOrder(1, Vector3:new(0, 0, 0), {Target:new(refinery, Vector3:new(0, 0, 0))}, false)
end

function Player:buildRefinery()
	if not self.startedRefinery then
		self.startedRefinery = true
		self:startBuilding(UnitId.REFINERY, 2, -.6)
	end

	if not self.finishedRefinery then
		refTable = self:getUnitsByClass(UnitClass.REFINERY, 1)
		
		if #refTable == 1 and refTable[1]:toFactory():getBuildStatus() == 100 then
			self.finishedRefinery = true
		end
	end

	return self.finishedRefinery
end

function Player:buildLandFactory()
	if not self.startedLandFactory then
		self.startedLandFactory = true
		self:startBuilding(UnitId.LAND_FACTORY, 3, .6)
	end

	if not self.finishedLandFactory then
		facTable = self:getUnitsByClass(UnitClass.LAND_FACTORY, 1)
		
		if #facTable == 1 and facTable[1]:toFactory():getBuildStatus() == 100 then
			self.finishedLandFactory = true
		end
	end

	return self.finishedLandFactory
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

	return true
end

function Player:startHarvesting()
	if self.startedHarvesting then return true end

	harvs = self:getUnitsByClass(UnitClass.RESOURCE_ROVER, 1)
	extrs = self:getUnitsByClass(UnitClass.EXTRACTOR, 1)

	if #harvs == 0 or (#extrs == 0) then return false end

	self:deselectUnits()
	self:selectUnits({harvs[1]})
	self:issueOrder(7, Vector3:new(0, 0, 0), {Target:new(extrs[1], Vector3:new(0, 0, 0))}, false)

	self.startedHarvesting = true

	return true
end

function Player:buildTaskforce()
	landFactory = self:getUnitsByClass(UnitClass.LAND_FACTORY, 1)[1]:toFactory()

	numWarMechs = 10
	numTanks = 5
	numArtillery = 5

	if not self.startedBuildingTaskforce then
		self.startedBuildingTaskforce = true

		for i = 0, numWarMechs do
			landFactory:appendToQueue(UnitId.WAR_MECH)
		end
		
		for i = 0, numTanks do
			landFactory:appendToQueue(UnitId.TANK)
		end
		
		for i = 0, numArtillery do
			landFactory:appendToQueue(UnitId.ARTILLERY)
		end
	end

	currWarMechs = self:getUnitsByClass(UnitClass.WAR_MECH, -1)
	currTanks = self:getUnitsByClass(UnitClass.TANK, -1)
	currArtillery = self:getUnitsByClass(UnitClass.ARTILLERY, -1)

	return #currWarMechs == numWarMechs and #currTanks == numTanks and #currArtillery == numArtillery
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
	}
}
