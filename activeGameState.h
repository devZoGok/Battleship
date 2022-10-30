#pragma once
#ifndef ACTIVE_GAME_STATE_H
#define ACTIVE_GAME_STATE_H

#include "guiAppState.h"
#include "player.h"
#include "map.h"

namespace vb01{
	class Node;
}

namespace battleship{
    class ActiveGameState : public gameBase::AbstractAppState {
    public:
        ActiveGameState(GuiAppState*, std::vector<Player*> players, int);
        ~ActiveGameState();
        void onAttached();
        void onDettached();
        void update();
        void onAction(int, bool);
        void onAnalog(int, float);
        inline void setSelectingLaunchPoint(bool s){this->selectingGuidedMissileTarget=s;}
        inline Player* getPlayer(){return mainPlayer;}
        inline std::vector<Unit*>& getSelectedUnits(){return selectedUnits;}
        inline std::vector<Unit*>& getUnitGroup(int i){return unitGroups[i];}
    private:
		void initDragbox();
		void initDepthText();
		void deselectUnits();
        void renderUnits();
        void updateCameraPosition();
        void updateSelectionBox();
        void addTarget();
        void issueOrder(Order::TYPE, bool);
        void lookAround(bool);
		void orientCamera(vb01::Vector3, double);
        bool isInLineOfSight(vb01::Vector3, float, Unit*);

        GuiAppState *guiState;
        std::vector<Player*> players;
        Player *mainPlayer;
		vb01::Quad *dragbox = nullptr;
		vb01::Node *dragboxNode = nullptr, *textNode = nullptr;
		vb01::Vector2 clickPoint;
        std::vector<vb01::Vector2> unitScreenPosVec;
        std::vector<vb01::Node*> unitLightNodes;
        std::vector<Unit*> unitGroups[9], selectedUnits;
		std::vector<Order::Target> targets;
        bool isSelectionBox = false, shiftPressed = false, controlPressed = false, selectingPatrolPoints = false, selectingGuidedMissileTarget = false, lookingAround = false;
        int playerId, zooms = 0;
	   	const int NUM_MAX_ZOOMS = 10;
		float depth = 1;
    };
}

#endif
