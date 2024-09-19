#ifndef ACTIVE_GAME_STATE_H
#define ACTIVE_GAME_STATE_H

#include "guiAppState.h"
#include "player.h"
#include "map.h"
#include "unit.h"

#include <util.h>

namespace vb01{
	class Node;
	class Texture;
}

namespace vb01Gui{
	class Button;
}

namespace battleship{
	class GameObject;

    class ActiveGameState : public gameBase::AbstractAppState {
    public:
        ActiveGameState(GuiAppState*, int);
        ~ActiveGameState();
        void onAttached();
        void onDettached();
        void update();
        void onAction(int, bool);
        void onAnalog(int, float);
		void onRawMouseWheelScroll(bool);
		inline void addButton(vb01Gui::Button *b){buttons.push_back(b);}
		inline std::vector<vb01Gui::Button*> getButtons(){return buttons;}
        inline Player* getPlayer(){return mainPlayer;}
        inline std::vector<Unit*>& getUnitGroup(int i){return unitGroups[i];}
		inline u8 *getOldMinimapImage(){return oldImageData;}
    private:
		enum CursorState{
			NORMAL,
			ATTACK,
			GARRISON,
			SUPPLY
		};

		bool selectedUnitsAmongst(std::vector<Unit*>);
		void updateGameObjHoveredOn();
		void initCursor();
		void updateCursor();
		void initDragbox();
		void removeDragbox();
		bool isGameObjSelectable(GameObject*, bool);
		void deselectUnits();
        void renderUnits();
        void updateDragBox();
		void updateStructureFrames();
        void castRayToTerrain();
        void issueOrder(Order::TYPE, std::vector<Order::Target>, bool);
        bool isInLineOfSight(vb01::Vector3, float, Unit*);
		void enableUnitState(Unit::State);
		inline bool canSelectHoveredOnGameObj(){return gameObjHoveredOn && gameObjHoveredOn->isSelectable() && gameObjHoveredOn->getPlayer() == mainPlayer;}

		CursorState cursorState = CursorState::NORMAL;
        Player *mainPlayer;
        GuiAppState *guiState;
		std::string unitGuiScreen = "";
		GameObject *gameObjHoveredOn = nullptr;
		vb01::Node *dragboxNode = nullptr;
		vb01::Vector2 clickPoint;
        std::vector<Unit*> unitGroups[9];
		std::vector<Order::Target> targets;
		std::vector<vb01Gui::Button*> buttons;
        bool isSelectionBox = false, shiftPressed = false, controlPressed = false, leftMouseClicked = false, selectingPatrolPoints = false, selectingDestOrient = false;
        int playerId, zooms = 0;
	   	const int NUM_MAX_ZOOMS = 10;
		float depth = 1;
		vb01::s64 lastLeftMouseClicked = 0;
		vb01::Node *cursorNode = nullptr;
		vb01::Texture *pointerTex = nullptr, *attackTex = nullptr, *garrisonTex = nullptr;
		vb01::u8 *oldImageData = nullptr;
    };
}

#endif
