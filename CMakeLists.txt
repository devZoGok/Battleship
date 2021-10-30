cmake_minimum_required(VERSION 2.9)
project(game)

include_directories(/usr/include/irrlicht)

link_directories(/usr/lib/)

set(CMAKE_BUILD_TYPE Debug)
cmake_policy(SET CMP0015 NEW)

set(states abstractAppState.cpp activeGameState.cpp inGameAppState.cpp guiAppState.cpp)
set(util util.cpp)
set(gui abstractImage.cpp abstractBitmapText.cpp tooltip.cpp button.cpp listbox.cpp textbox.cpp checkbox.cpp slider.cpp optionsButton.cpp exitButton.cpp consoleCommand.cpp)
set(core stateManager.cpp eventListener.cpp gameManager.cpp main.cpp key.cpp)
set(content player.cpp vessel.cpp destroyer.cpp cruiser.cpp aircraftCarrier.cpp submarine.cpp unit.cpp map.cpp guidedMissile.cpp depthCharge.cpp jet.cpp demoJet.cpp missile.cpp missileJet.cpp projectile.cpp shell.cpp torpedo.cpp explosion.cpp)

add_executable(battleship ${states} ${gui} ${core} ${content} ${util})
target_link_libraries(battleship Irrlicht)
target_link_libraries(battleship sfml-system)
target_link_libraries(battleship sfml-audio)
