#include "abstractCommand.h"

#include <cmath>
#include <vector.h>

namespace battleship{
	using namespace std;

	vector<string> AbstractCommand::explodeString(string commandStr){
		vector<int> spaceIds;
		vector<string> fullCommand;

		for(int i = 0; i < commandStr.length(); i++)
				if(commandStr[i] == ' ')
						spaceIds.push_back(i);

		fullCommand.push_back(commandStr.substr(0, spaceIds[0]));

		for(int i = 0; i < spaceIds.size(); i++){
				bool lastSpace = (i == spaceIds.size() - 1);
				string argument = commandStr.substr(spaceIds[i] + 1, lastSpace ? string::npos : spaceIds[i + 1] - spaceIds[i] - 1);
				fullCommand.push_back(argument);
		}

		return fullCommand;
	}

	void AbstractCommand::handle(){
		if(cmdStr.find(" ") == -1)
			return;

		arguments = explodeString(cmdStr);
	}
}
