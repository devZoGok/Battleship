function Player:selectTaskForce()
	taskForce = self:getSelectedUnitsByClass(UnitClass.TANK)
	return #taskForce > 5
end

function Player:issueMoveOrder()
	return false
end

function Player:selectLandFactory()
	return false
end

function Player:trainTaskForce()
	return false
end

function Player:selectLandCell()
	return true
end

function Player:selectEngi()
	taskForce = self:getSelectedUnitsByClass(UnitClass.ENGINEER)

	if #taskForce == 0 then
		self:selectUnits({self:getUnit(0)})
	end

	taskForce = self:getSelectedUnitsByClass(UnitClass.ENGINEER)
	return #taskForce > 0
end

function Player:issueBuildOrder()
	for i = 1, self:getNumUnits() do
		if self:getUnit(i - 1):getUnitClass() == UnitClass.LAND_FACTORY then
			return true
		end
	end

	spawnPoint = self:getSpawnPoint()
	structure = GameObjectFactory.createUnit(self, 8, spawnPoint, Quaternion:new(1, 0, 0, 0), 0)
	self:addUnit(structure)

	targets = {Target:new(structure, Vector3:new(0, 0, 0))}
	self:issueOrder(1, Vector3:new(0, 0, 0), targets, false)
	return true
end

Player.behaviour = {
	type = BTNodeType.SELECTOR,
	children = {
		{
			type = BTNodeType.SEQUENCE, children = {
				{type = BTNodeType.FUNCTION, func = 'selectTaskForce'},
				{type = BTNodeType.FUNCTION, func = 'issueMoveOrder'},
			},
			type = BTNodeType.SEQUENCE, children = {
				{type = BTNodeType.FUNCTION, func = 'selectLandFactory'},
				{type = BTNodeType.FUNCTION, func = 'trainTaskForce'}
			},
			type = BTNodeType.SEQUENCE, children = {
				{type = BTNodeType.FUNCTION, func = 'selectLandCell'},
				{type = BTNodeType.FUNCTION, func = 'selectEngi'},
				{type = BTNodeType.FUNCTION, func = 'issueBuildOrder'}
			}
		}
		--[[
		]]--
	}
}
