#ifndef MAP_EDITOR_APP_STATE_H
#define MAP_EDITOR_APP_STATE_H

#include <abstractAppState.h>

#include <vector.h>

#include <string>

namespace battleship{
	class MapEditor;
	class Map;

	class MapEditorAppState : public gameBase::AbstractAppState{
		public:
			MapEditorAppState(std::string, vb01::Vector2);
			void update();
			void onAttached();
			void onDettached();
			void onAction(int, bool);
			void onAnalog(int, float);
		private:
			class MapEditor{
				public:
					MapEditor(std::string, vb01::Vector2);
				private:
					void generatePlane(vb01::Vector2);
					void prepareGui();

					Map *map;
			};

			MapEditor *mapEditor = nullptr;
			std::string mapName;
			vb01::Vector2 mapSize;
	};
}

#endif
