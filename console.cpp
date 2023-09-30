#include "console.h"
#include "addUnitCommand.h"

using namespace std;

namespace battleship{
    void Console::execute(string cmdStr) {
		int space = cmdStr.find(" ");
		string cmdName, argsStr;

		if(space != -1){
			cmdName = cmdStr.substr(0, space);
			argsStr = cmdStr.substr(space + 1, string::npos);
		}

		if(cmdName == "add-unit")
			AddUnitCommand(argsStr).execute();
    }
}
