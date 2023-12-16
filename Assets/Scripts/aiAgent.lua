BTNodeType = {SELECTOR = 0, SEQUENCE = 1, FUNCTION = 2}

function executeBtNode(agent, btNode)
	for i = 1, #btNode.children do
		if btNode.children[i].type == BTNodeType.FUNCTION then
			func = btNode.children[i].func
			res = agent[func](agent)
		else
			res = executeBtNode(agent, btNode.children[i])
		end
	
		if btNode.type == BTNodeType.SELECTOR then
			if res then return true end

			if i == #btNode.children and not res then return false end
		elseif btNode.type == BTNodeType.SEQUENCE then
			if not res then return false end

			if i == #btNode.children and res then return true end
		end
	end
end
