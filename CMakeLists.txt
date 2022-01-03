cmake_minimum_required(VERSION 2.9)
project(battleship)

set(CMAKE_BUILD_TYPE Debug)
cmake_policy(SET CMP0015 NEW)

set(states activeGameState.cpp inGameAppState.cpp guiAppState.cpp)
set(util util.cpp binds.h)
set(gui tooltip.cpp optionsButton.cpp exitButton.cpp consoleCommand.cpp)
set(core gameManager.cpp main.cpp)
set(ships vessel.cpp destroyer.cpp cruiser.cpp aircraftCarrier.cpp submarine.cpp)
set(projectiles projectile.cpp guidedMissile.cpp depthCharge.cpp missile.cpp shell.cpp torpedo.cpp)
set(aircraft jet.cpp demoJet.cpp missileJet.cpp)
set(fx explosion.cpp)
set(content player.cpp unit.cpp map.cpp ${ships} ${projectiles} ${aircraft} ${fx})

set(SFML_DIR external/SFML)
set(VB01_DIR external/vb01)
set(FREETYPE_DIR ${VB01_DIR}/external/freetype)
set(VB01_GUI_DIR external/vb01Gui)
set(GAME_BASE_DIR external/gameBase)
set(GLFW_DIR ${VB01_DIR}/external/glfw/)
set(GAME_NAME battleship)

add_executable(${GAME_NAME} ${states} ${gui} ${core} ${content} ${util})

target_include_directories(${GAME_NAME} PUBLIC ${FREETYPE_DIR}/include)

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

set(GLFW_LIB_DIR build/${GLFW_DIR}/src)
include_directories(${GLFW_DIR}/include/GLFW)
target_link_directories(${GAME_NAME} PUBLIC ${GLFW_LIB_DIR})

target_link_libraries(${GAME_NAME} vb01)
target_link_libraries(${GAME_NAME} vb01Gui)
target_link_libraries(${GAME_NAME} gameBase)
target_link_libraries(${GAME_NAME} sfml-system)
target_link_libraries(${GAME_NAME} sfml-audio)
target_link_libraries(${GAME_NAME} glfw)
