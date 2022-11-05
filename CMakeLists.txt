cmake_minimum_required(VERSION 2.9)

set(GAME_NAME battleship)
project(${GAME_NAME})

set(CMAKE_BUILD_TYPE Debug)
set(BUILD_TESTS ON)
cmake_policy(SET CMP0015 NEW)

set(STATES activeGameState.cpp inGameAppState.cpp guiAppState.cpp)
set(UTIL util.cpp binds.h)
set(GUI tooltip.cpp optionsButton.cpp exitButton.cpp consoleCommand.cpp)
set(CORE gameManager.cpp defConfigs.cpp)
set(PROJECTILES projectile.cpp)
set(FX explosion.cpp)
set(UNITS unit.cpp structure.cpp vehicle.cpp)
set(CONTENT player.cpp pathfinder.cpp map.cpp ${UNITS} ${PROJECTILES} ${FX})
set(GAME_SRC ${STATES} ${GUI} ${CORE} ${CONTENT} ${UTIL})

set(SFML_DIR external/SFML)
set(VB01_DIR external/vb01)
set(VB01_GUI_DIR external/vb01Gui)
set(GAME_BASE_DIR external/gameBase)

add_executable(${GAME_NAME} main.cpp ${GAME_SRC})

include_directories(external/tinydir)
include_directories(external/vb01Gui/external/vb01/external/glm)
include_directories(external/vb01Gui/external/vb01/external/glm/glm)

add_subdirectory(${VB01_DIR})
set(VB01_LIB_DIR ${VB01_DIR}/build)
target_include_directories(${GAME_NAME} PUBLIC ${VB01_DIR})
target_link_directories(${GAME_NAME} PUBLIC ${VB01_LIB_DIR})

add_subdirectory(${VB01_GUI_DIR})
set(VB01_GUI_LIB_DIR build/${VB01_GUI_DIR})
target_include_directories(${GAME_NAME} PUBLIC ${VB01_GUI_DIR})
target_link_directories(${GAME_NAME} PUBLIC ${VB01_GUI_LIB_DIR})

add_subdirectory(${GAME_BASE_DIR})
set(GAME_BASE_LIB_DIR build/${GAME_BASE_DIR}/src)
target_include_directories(${GAME_NAME} PUBLIC ${GAME_BASE_DIR})
target_link_directories(${GAME_NAME} PUBLIC ${GAME_BASE_LIB_DIR})

add_subdirectory(${SFML_DIR})
set(SFML_LIB_DIR build/${SFML_DIR}/src)
target_include_directories(${GAME_NAME} PUBLIC ${SFML_DIR}/include)
target_link_directories(${GAME_NAME} PUBLIC ${SFML_LIB_DIR})

set(DEPS vb01 vb01Gui gameBase sfml-system sfml-audio)
target_link_libraries(${GAME_NAME} ${DEPS})

if(BUILD_TESTS)
	if(UNIX)
		include_directories(/usr/include/cppunit)
		link_directories(/usr/lib)
	elseif(WIN32)
		include_directories(C:/Program\ Files\ \(x86\)/cppunit/include)
		link_directories(C:/Program\ Files\ \(x86\)/cppunit/lib)
	endif()
	
	set(TEST_SRC testMain.cpp pathfinderTest.cpp ${GAME_SRC})
	set(TEST_NAME battleshipTests)
	
	add_executable(${TEST_NAME} ${TEST_SRC})
	target_include_directories(${TEST_NAME} PUBLIC ${VB01_DIR})
	target_include_directories(${TEST_NAME} PUBLIC ${VB01_GUI_DIR})
	target_include_directories(${TEST_NAME} PUBLIC ${GAME_BASE_DIR})
	target_link_libraries(${TEST_NAME} ${DEPS} cppunit)
endif()
