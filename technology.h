#ifndef TECHNOLOGY_H
#define TECHNOLOGY_H

#include <string>
#include <vector>

namespace battleship{
	struct Technology{
		int id, cost;
		std::string name;
		std::string icon;
		std::string description;
		std::vector<int> parents;
		std::vector<int> children;
		std::vector<int> abilities;
	};
}

#endif
