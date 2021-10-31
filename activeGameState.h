#pragma once
#ifndef ACTIVE_GAME_STATE_H
#define ACTIVE_GAME_STATE_H

#include "guiAppState.h"
#include "player.h"
#include "map.h"

namespace vb01{
		class Node;
}

namespace game{
    namespace core{
        class ActiveGameState : public AbstractAppState {
        public:
            ActiveGameState(GuiAppState*, content::Map*, std::vector<content::Player*> players, int);
            ~ActiveGameState();
            void onAttachment();
            void onDetachment();
            void update();
            void onAction(Mapping::Bind, bool);
            void onAnalog(Mapping::Bind, double);
            inline void setSelectingLaunchPoint(bool s){this->selectingGuidedMissileTarget=s;}
            inline content::Player* getPlayer(){return mainPlayer;}
            inline std::vector<content::Unit*>& getSelectedUnits(){return selectedUnits;}
            inline std::vector<content::Unit*>& getUnitGroup(int i){return unitGroups[i];}
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
                UnitActionButton(content::unitData::UNIT_TYPE, vb01::Vector2, vb01::Vector2, std::string, std::string);
                ~UnitActionButton(){}
                virtual void onClick();
                virtual void onMouseOver();
                virtual void onMouseAway();
            protected:
                content::unitData::UNIT_TYPE type;
                std::vector<content::Unit*> units;
                ActiveGameState *activeState;
            };

            void renderGUIBorders();
            void renderActionButtons();
            void renderUnits(std::vector<content::Unit*>);
            void updateVectors();
            void updateSelectionBox();
            void addPos();
            void issueOrder(content::Order::TYPE, std::vector<vb01::Vector3*>, bool);
            void lookAround(bool);
            GuiAppState *guiState;
            content::Map *map;
            std::vector<content::Player*> players;
            content::Player *mainPlayer;
            vb01::Camera *cam;
            vb01::Vector3 camDir = vb01::Vector3(0, 0, 1), camLeft = vb01::Vector3(1, 0, 0), camUp = vb01::Vector3(0, 1, 0);
            vb01::Vector2 clickPoint, selectionBoxOrigin, selectionBoxEnd;
            std::vector<vb01::Vector2> unitScreenPosVec;
            std::vector<vb01::Vector3*> orderPos;
            std::vector<vb01::Node*> unitLightNodes;
            std::vector<content::Unit*> unitGroups[9], selectedUnits;
            UnitActionButton *actionButtons[4]{nullptr,nullptr,nullptr,nullptr};
            bool isSelectionBox = false, shiftPressed=false, controlPressed = false, selectingPatrolPoints = false, selectingGuidedMissileTarget = false,lookingAround=false;
            bool isInLineOfSight(vb01::Vector3, float, content::Unit*);
            int playerId, zooms = 0;
        };
    }
}

#endif
