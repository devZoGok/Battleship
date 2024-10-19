#ifndef PLAY_BUTTON_H
#define PLAY_BUTTON_H

#include <button.h>

namespace vb01Gui{
	class Listbox;
}

namespace battleship{
	class PlayButton : public vb01Gui::Button {
	public:
	    PlayButton(std::vector<vb01Gui::Listbox*>, std::vector<vb01Gui::Listbox*>, vb01Gui::Listbox*, vb01::Vector3, vb01::Vector2, std::string, bool);
	    void onClick();
	private:
		std::vector<vb01Gui::Listbox*> difficultiesListboxes, factionsListboxes;
		vb01Gui::Listbox *mapListbox = nullptr;
	    int lengths[2];
	};
}

#endif
