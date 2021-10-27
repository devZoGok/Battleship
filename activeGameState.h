#pragma once
#ifndef ACTIVE_GAME_STATE_H
#define ACTIVE_GAME_STATE_H

#include "gameManager.h"
#include <irrlicht.h>
#include "guiAppState.h"
#include "player.h"
#include "map.h"

namespace game{
    namespace core{
        class ActiveGameState : public AbstractAppState {
        public:
            ActiveGameState(core::GameManager*, GuiAppState*, content::Map*, std::vector<content::Player*> players, int);
            ~ActiveGameState();
            void onAttachment();
            void onDetachment();
            void update();
            void onAction(Bind, bool);
            void onAnalog(Bind, double);
            inline void setSelectingLaunchPoint(bool s){this->selectingGuidedMissileTarget=s;}
            inline content::Player* getPlayer(){return mainPlayer;}
            inline std::vector<content::Unit*>& getSelectedUnits(){return selectedUnits;}
            inline std::vector<content::Unit*>& getUnitGroup(int i){return unitGroups[i];}
        private:
            class UnitButton : public gui::Button{
            public:
                UnitButton(GameManager*,ActiveGameState*,vector2di, vector2di, irr::core::stringw,int,int);
                ~UnitButton();
                virtual void onClick();
                virtual void onMouseOver();
                virtual void onMouseAway();
            private:
                ActiveGameState *activeState;
                int faction,unitId;
            };
            class UnitActionButton : public gui::Button{
            public:
                UnitActionButton(GameManager*, content::unitData::UNIT_TYPE, irr::core::vector2di, irr::core::vector2di, irr::core::stringw, irr::core::stringw);
                ~UnitActionButton();
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
            void issueOrder(content::Order::TYPE, std::vector<irr::core::vector3df*>, bool);
            void lookAround(bool);
            GameManager *gameManager;
            GuiAppState *guiState;
            content::Map *map;
            std::vector<content::Player*> players;
            content::Player *mainPlayer;
            irr::scene::ICameraSceneNode* cam;
            irr::core::vector3df camDir = irr::core::vector3df(0, 0, 1), camLeft = irr::core::vector3df(1, 0, 0), camUp = irr::core::vector3df(0, 1, 0);
            irr::core::vector2d<s32> clickPoint, selectionBoxOrigin, selectionBoxEnd;
            std::vector<irr::core::vector2d<s32>> unitScreenPosVec;
            std::vector<vector3df*> orderPos;
            std::vector<ILightSceneNode*> unitLightNodes;
            std::vector<content::Unit*> unitGroups[9], selectedUnits;
            UnitActionButton *actionButtons[4]{nullptr,nullptr,nullptr,nullptr};
            bool isSelectionBox = false, shiftPressed=false, controlPressed = false, selectingPatrolPoints = false, selectingGuidedMissileTarget = false,lookingAround=false;
            bool isInLineOfSight(irr::core::vector3df, float, content::Unit*);
            int playerId, zooms = 0;
        };
    }
}

#endif
