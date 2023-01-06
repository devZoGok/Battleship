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
	class TerrainObject;

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
					void updateCircleRadius(bool);
					void pushLandmassVerts(float);
					void toggleSelection(TerrainObject* ,bool);
					void castSelectionRay();
					inline float getGuiThreshold(){return guiThreshold;}
					inline float getCircleRadius(){return circleRadius;}
					inline bool isPushing(){return pushing;}
					inline void setPushing(bool p){pushing = p;}
					inline void setPushPos(vb01::Vector3 p){pushPos = p;}
				private:
					void generatePlane(vb01::Vector2);
					void prepareGui();
					void prepareTextures(std::string, bool, std::vector<vb01::Texture*>&);
					inline vb01::Texture* getSkyTexture(int i){return skyTextures[i];}
					inline vb01::Texture* getLandmassTexture(int i){return landmassTextures[i];}

					Map *map;
					TerrainObject *selectedTerrainObject = nullptr;
					bool pushing = false;
					const float MIN_RADIUS = 1, MAX_RADIUS = 100, INCREASE_RATE = 1;
					float circleRadius = MIN_RADIUS, guiThreshold = 200;
					vb01::Vector3 pushPos = vb01::Vector3::VEC_ZERO;
					std::vector<vb01::Texture*> skyTextures, landmassTextures, waterTextures;
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
			vb01::Text *radiusText = nullptr;
	};
}

#endif
