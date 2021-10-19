cmake_minimum_required(VERSION 2.6)
project(game)

include_directories(/usr/include/irrlicht)

link_directories(/usr/lib/)

set(CMAKE_BUILD_TYPE Debug)
set(states AbstractAppState.cpp ActiveGameState.cpp InGameAppState.cpp GuiAppState.cpp)
set(util util.cpp)
set(gui AbstractImage.cpp AbstractBitmapText.cpp Tooltip.cpp Button.cpp Listbox.cpp Textbox.cpp Checkbox.cpp Slider.cpp OptionsButton.cpp ExitButton.cpp ConsoleCommand.cpp)
set(core EventListener.cpp GameManager.cpp main.cpp Key.cpp)
set(content Player.cpp Vessel.cpp Destroyer.cpp Cruiser.cpp AircraftCarrier.cpp Submarine.cpp Unit.cpp Map.cpp GuidedMissile.cpp DepthCharge.cpp Jet.cpp DemoJet.cpp Missile.cpp MissileJet.cpp Projectile.cpp Shell.cpp Torpedo.cpp Explosion.cpp)

add_executable(game ${states} ${gui} ${core} ${content} ${util})
target_link_libraries(game Irrlicht)
target_link_libraries(game sfml-system)
target_link_libraries(game sfml-audio)
