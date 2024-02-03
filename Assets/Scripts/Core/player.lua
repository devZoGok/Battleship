Player.numStartEngis = 3
Player.startedFort = false 

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

	while #fort:getQueue() + #self:getUnitsByClass(UnitClass.ENGINEER, -1) < self.numStartEngis do
		fort:appendToQueue(UnitId.ENGINEER)
	end

	return true
end

Player.behaviour = {
	type = BTNodeType.SEQUENCE,
	children = {
		{type = BTNodeType.FUNCTION, func = 'buildFort'},
		{type = BTNodeType.FUNCTION, func = 'trainEngineers'},
	}
}
