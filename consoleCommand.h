#pragma once
#ifndef CONSOLE_COMMAND_H
#define CONSOLE_COMMAND_H

#include <vector>

#include "unit.h"
#include "listbox.h"
#include "player.h"

namespace battleship{
    class ConsoleCommand{
    public:
      static void execute(std::string);
    private:
			static void parse(std::string, std::vector<std::string>&);

			static void validateAddUnitArguments(std::vector<std::string>&);
      static void executeAddUnit(int, int);

			static void validateDebugUnitsArguments();
    };
}

#endif
