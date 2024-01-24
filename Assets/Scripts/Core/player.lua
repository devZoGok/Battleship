Player.sizeTaskForce = 5
Player.trainingStarted = false 
Player.taskForceSent = false 
Player.buildOrdered = false 

function Player:sendTaskForce()
	if self.taskForceSent then
		return true
	end

	self:deselectUnits()
	taskForce = {}

	for i = 1, self:getNumUnits() do
		if self:getUnit(i - 1):getUnitClass() == UnitClass.TANK then
			taskForce[#taskForce + 1] = self:getUnit(i - 1)
		end
	end

	if #taskForce >= self.sizeTaskForce then
		self.taskForceSent = true
		self:selectUnits(taskForce)
		self:issueOrder(2, Vector3:new(0, 0, 0), {Target:new(nil, Vector3:new(0, 0, 0))}, false)
		return true
	else
		return false
	end
end

function Player:trainTaskForce()
	if self.trainingStarted then
		return true
	end

	landFactory = nil

	for i = 1, self:getNumUnits() do
		if self:getUnit(i - 1):getUnitClass() == UnitClass.LAND_FACTORY then
			landFactory = self:getUnit(i - 1):toFactory()
			break
		end
	end

	if landFactory ~= nil then
		print('Training...')
		for i = 1, self.sizeTaskForce do
			landFactory:appendToQueue(UnitClass.TANK)
		end

		self.trainingStarted = true
		return true
	else
		return false
	end
	
end

function Player:selectEngi()
	taskForce = self:getSelectedUnitsByClass(UnitClass.ENGINEER)

	if #taskForce == 0 then
		for i = 1, self:getNumUnits() do
			if self:getUnit(i - 1):getUnitClass() == UnitClass.ENGINEER then
				taskForce = {self:getUnit(i - 1)}
				self:selectUnits(taskForce)
				break
			end
		end
	end

	return #taskForce > 0
end

function Player:issueBuildOrder()
	if self.buildOrdered then
		return true
	end

	for i = 1, self:getNumUnits() do
		if self:getUnit(i - 1):getUnitClass() == UnitClass.LAND_FACTORY then
			return true
		end
	end

	spawnPoint = self:getSpawnPoint()
	structure = GameObjectFactory.createUnit(self, 15, spawnPoint, Quaternion:new(1, 0, 0, 0), 0)
	self:addUnit(structure)

	self:issueOrder(1, Vector3:new(0, 0, 0), {Target:new(structure, Vector3:new(0, 0, 0))}, false)
	self.buildOrdered = true
	return true
end

Player.behaviour = {
	type = BTNodeType.SELECTOR,
	children = {
		{type = BTNodeType.FUNCTION, func = 'sendTaskForce'},
		{type = BTNodeType.FUNCTION, func = 'trainTaskForce'},
		{
			type = BTNodeType.SEQUENCE, children = {
				{type = BTNodeType.FUNCTION, func = 'selectEngi'},
				{type = BTNodeType.FUNCTION, func = 'issueBuildOrder'},
			}
		}
	}
}
