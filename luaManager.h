#ifndef LUA_MANAGER_H
#define LUA_MANAGER_H

#include <lua.hpp>
#include <lauxlib.h>
#include <lualib.h>

#include <string>
#include <vector>

namespace battleship{
		class LuaManager{
				public:
						struct Index{
								std::string index;
							 	bool string;

								Index(std::string index, bool string){
										this->index = index;
										this->string = string;
								}
						};

						static LuaManager* getSingleton();
						inline lua_State* getState(){return state;}
						void readFile(std::string);
						int getInt(std::string);
						int getIntFromTable(std::string, std::vector<Index>);
						float getFloatFromTable(std::string, std::vector<Index>);
						std::string getStringFromTable(std::string, std::vector<Index>);
				private:
						LuaManager();
						void prepareTableForRetrieval(std::string, std::vector<Index>&);

						lua_State *state = nullptr;
		};
}

#endif
