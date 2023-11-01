Unit = {}

function Unit:new(un)
	unit = un or {}
	self.__index = index
	setmetatable(unit, self)
	return unit
end
