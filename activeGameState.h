#ifndef ACTIVE_GAME_STATE_H
#define ACTIVE_GAME_STATE_H

#include "guiAppState.h"
#include "player.h"
#include "map.h"

namespace vb01{
	class Node;
}

namespace vb01Gui{
	class Button;
}

namespace battleship{
    class ActiveGameState : public gameBase::AbstractAppState {
    public:
        ActiveGameState(GuiAppState*, int);
        ~ActiveGameState();
        void onAttached();
        void onDettached();
        void update();
        void onAction(int, bool);
        void onAnalog(int, float);
		inline void addButton(vb01Gui::Button *b){buttons.push_back(b);}
		inline std::vector<vb01Gui::Button*> getButtons(){return buttons;}
        inline Player* getPlayer(){return mainPlayer;}
        inline std::vector<Unit*>& getUnitGroup(int i){return unitGroups[i];}
    private:
		void initDragbox();
		vb01::Node* initText(vb01::Vector2);
		void deselectUnits();
        void renderUnits();
        void updateSelectionBox();
		void updateStructureFrames();
        void addTarget();
        void issueOrder(Order::TYPE, bool);
        bool isInLineOfSight(vb01::Vector3, float, Unit*);
		bool engineersSelected();

        GuiAppState *guiState;
        Player *mainPlayer;
		vb01::Quad *dragbox = nullptr;
		vb01::Node *dragboxNode = nullptr, *textNode = nullptr, *refinedsText = nullptr, *wealthText = nullptr, *researchText = nullptr;
		vb01::Vector2 clickPoint;
        std::vector<vb01::Vector2> unitScreenPosVec;
        std::vector<vb01::Node*> unitLightNodes;
        std::vector<Unit*> unitGroups[9];
		std::vector<Order::Target> targets;
		std::vector<vb01Gui::Button*> buttons;
        bool isSelectionBox = false, shiftPressed = false, controlPressed = false, leftMouseClicked = false, selectingPatrolPoints = false, selectingDestOrient = false;
        int playerId, zooms = 0;
	   	const int NUM_MAX_ZOOMS = 10;
		float depth = 1;
		vb01::s64 lastLeftMouseClicked = 0;
    };
}

#endif
