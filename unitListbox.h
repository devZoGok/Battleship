#ifndef UNIT_LISTBOX_H
#define UNIT_LISTBOX_H

#include <listbox.h>

namespace battleship{
	class UnitListbox : public vb01Gui::Listbox{
		public:
			UnitListbox(vb01::Vector2, vb01::Vector2, std::vector<std::string>, int, std::string);
			void onClose();
		private:
	};
}

#endif
