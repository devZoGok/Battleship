#include "mapListbox.h"
#include "map.h"
#include "concreteGuiManager.h"
#include "gameManager.h"

#include <button.h>
#include <checkbox.h>
#include <slider.h>
#include <textbox.h>
#include <node.h>
#include <text.h>

namespace battleship{
	using namespace vb01;
	using namespace vb01Gui;
	using namespace std;

	MapListbox::MapListbox(Vector2 pos, Vector2 size, std::vector<string> &lines, int maxDisplay, string fontPath, bool closeable) : Listbox(pos, size, lines, maxDisplay, fontPath, closeable){}

	void MapListbox::onClose(){
		ConcreteGuiManager *guiManager = ConcreteGuiManager::getSingleton();

		Listbox *mapListbox = guiManager->getListboxes()[0];
		int selectedMap = mapListbox->getSelectedOption();
		string mapName = wstringToString(mapListbox->getContents()[selectedMap]);
		string mapPath = GameManager::getSingleton()->getPath() + "Models/Maps/" + mapName + "/" + mapName + ".lua";
		int numSpawnPoints = Map::getNumSpawnPointsByMap(mapPath);

		guiManager->readLuaScreenScript("singlePlayerMenu.lua");
		mapListbox = guiManager->getListboxes()[0];
		vector<Button*> buttons = guiManager->getButtons();
		guiManager->readLuaScreenScript("playerSelection.lua", buttons, vector<Listbox*>{mapListbox}, vector<Checkbox*>{}, vector<Slider*>{}, vector<Textbox*>{}, vector<Node*>{}, vector<Text*>{}, numSpawnPoints);
	}
}
