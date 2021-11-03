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
    class ActiveGameState : public AbstractAppState {
    public:
        ActiveGameState(GuiAppState*, Map*, std::vector<Player*> players, int);
        ~ActiveGameState();
        void onAttachment();
        void onDetachment();
        void update();
        void onAction(Mapping::Bind, bool);
        void onAnalog(Mapping::Bind, double);
        inline void setSelectingLaunchPoint(bool s){this->selectingGuidedMissileTarget=s;}
        inline Player* getPlayer(){return mainPlayer;}
        inline std::vector<Unit*>& getSelectedUnits(){return selectedUnits;}
        inline std::vector<Unit*>& getUnitGroup(int i){return unitGroups[i];}
    private:
        class UnitButton : public vb01Gui::Button{
        public:
            UnitButton(ActiveGameState*, vb01::Vector2, vb01::Vector2, std::string, int, int);
            ~UnitButton(){}
            virtual void onClick();
            virtual void onMouseOver();
            virtual void onMouseAway();
        private:
            ActiveGameState *activeState;
            int faction, unitId;
        };

        class UnitActionButton : public vb01Gui::Button{
        public:
            UnitActionButton(unitData::UNIT_TYPE, vb01::Vector2, vb01::Vector2, std::string, std::string);
            ~UnitActionButton(){}
            virtual void onClick();
            virtual void onMouseOver();
            virtual void onMouseAway();
        protected:
            unitData::UNIT_TYPE type;
            std::vector<Unit*> units;
            ActiveGameState *activeState;
        };

        void renderGUIBorders();
        void renderActionButtons();
        void renderUnits(std::vector<Unit*>);
        void updateVectors();
        void updateSelectionBox();
        void addPos();
        void issueOrder(Order::TYPE, std::vector<vb01::Vector3*>, bool);
        void lookAround(bool);
				void orientCamera(vb01::Vector3, double);

        GuiAppState *guiState;
        Map *map;
        std::vector<Player*> players;
        Player *mainPlayer;
        vb01::Camera *cam;
        vb01::Vector3 camDir = vb01::Vector3(0, 0, 1), camLeft = vb01::Vector3(1, 0, 0), camUp = vb01::Vector3(0, 1, 0);
        vb01::Vector2 clickPoint, selectionBoxOrigin, selectionBoxEnd;
        std::vector<vb01::Vector2> unitScreenPosVec;
        std::vector<vb01::Vector3*> orderPos;
        std::vector<vb01::Node*> unitLightNodes;
        std::vector<Unit*> unitGroups[9], selectedUnits;
        UnitActionButton *actionButtons[4]{nullptr,nullptr,nullptr,nullptr};
        bool isSelectionBox = false, shiftPressed=false, controlPressed = false, selectingPatrolPoints = false, selectingGuidedMissileTarget = false,lookingAround=false;
        bool isInLineOfSight(vb01::Vector3, float, Unit*);
        int playerId, zooms = 0;
    };
}

#endif
