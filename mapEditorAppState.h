#ifndef MAP_EDITOR_APP_STATE_H
#define MAP_EDITOR_APP_STATE_H

#include <listbox.h>

#include <abstractAppState.h>

#include <vector.h>

#include <string>

namespace vb01{
	class Texture;
}

namespace battleship{
	class MapEditor;
	class Map;

	class MapEditorAppState : public gameBase::AbstractAppState{
		public:
			class MapEditor{
				public:
					class UnitListbox : public vb01Gui::Listbox{
						public:
							UnitListbox(int, vb01::Vector2, vb01::Vector2, std::vector<std::string>, int, std::string);
							void onClose();
						private:
							int startId;
					};

					MapEditor(std::string, vb01::Vector2);
				private:
					void generatePlane(vb01::Vector2);
					void prepareGui();
					void prepareTextures(std::string, bool, std::vector<vb01::Texture*>&);
					inline vb01::Texture* getSkyTexture(int i){return skyTextures[i];}
					inline vb01::Texture* getLandmassTexture(int i){return landmassTextures[i];}

					Map *map;
					std::vector<vb01::Texture*> skyTextures, landmassTextures;
					UnitListbox *vehicleListbox = nullptr, *structureListbox = nullptr;
			};

			MapEditorAppState(std::string, vb01::Vector2);
			void update();
			void onAttached();
			void onDettached();
			void onAction(int, bool);
			void onAnalog(int, float);
			inline MapEditor* getMapEditor(){return mapEditor;}
		private:
			MapEditor *mapEditor = nullptr;
			std::string mapName;
			vb01::Vector2 mapSize;
	};
}

#endif
