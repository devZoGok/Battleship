#ifndef MAP_LISTBOX_H
#define MAP_LISTBOX_H

#include <listbox.h>

namespace battleship{
	class MapListbox : public vb01Gui::Listbox{
		public:
			MapListbox(vb01::Vector2, vb01::Vector2, std::vector<std::string>&, int, std::string, bool);
		private:
			void onClose();
	};
}

#endif
