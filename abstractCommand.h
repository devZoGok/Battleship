#ifndef ABSTRACT_COMMAND_H
#define ABSTRACT_COMMAND_H

#include <string>
#include <vector>

namespace battleship{
	class AbstractCommand{
		protected:
			AbstractCommand(std::string str) : cmdStr(str){}
			virtual void execute(){}
			virtual void validate(){}
			virtual void handle();

			std::string cmdStr;
			std::vector<std::string> arguments;
		private:
			std::vector<std::string> explodeString(std::string);
	};
}

#endif
