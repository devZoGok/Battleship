#include"ActiveGameState.h"
#include"InGameAppState.h"
#include"util.h"
#include"Cruiser.h"
#include"Destroyer.h"
#include"Submarine.h"
#include"AircraftCarrier.h"
#include"DemoJet.h"
#include"MissileJet.h"
#include"Tooltip.h"
#include<algorithm>
#include<stdlib.h>

using namespace game::core;
using namespace game::util;
using namespace game::content;
using namespace std;
using namespace irr::video;

namespace game{
    namespace core{
        const int size=50;
        
        ActiveGameState::UnitButton::UnitButton(GameManager *gm,ActiveGameState *activeState,vector2di pos, vector2di size, irr::core::stringw name,int faction,int unitId) : gui::Button(gm,pos,size,name,true){
            this->activeState=activeState;
            this->faction=faction;
            this->unitId=unitId;
        }
        
        void ActiveGameState::UnitButton::onClick(){
            Player *player=activeState->getPlayer();
            vector3df spawnPoint=player->getSpawnPoint();
            Unit *u;
            switch(unitData::unitType[unitId]){
                case unitData::UNIT_TYPE::VESSEL:
                    u=new Vessel(gameManager,player,spawnPoint,unitId);
                    break;
                case unitData::UNIT_TYPE::DESTROYER:
                    u=new Destroyer(gameManager,player,spawnPoint,unitId);
                    break;
                case unitData::UNIT_TYPE::CRUISER:
                    u=new Cruiser(gameManager,player,spawnPoint,unitId);
                    break;
                case unitData::UNIT_TYPE::AIRCRAFT_CARRIER:
                    u=new AircraftCarrier(gameManager,player,spawnPoint,unitId);
                    break;
                case unitData::UNIT_TYPE::SUBMARINE:
                    u=new Submarine(gameManager,player,spawnPoint,unitId);
                    break;
            }
            player->addUnit(u);
        }
        
        void ActiveGameState::UnitButton::onMouseOver(){
            if(!mouseOverDone){
                GuiAppState *guiState=((GuiAppState*)gameManager->getAppState(AppStateTypes::GUI_STATE));
                guiState->addTooltip(new gui::Tooltip(gameManager,pos-vector2di(100,0),name));
            }
        }

        void ActiveGameState::UnitButton::onMouseAway(){
            if(!mouseAwayDone){
                GuiAppState *guiState=((GuiAppState*)gameManager->getAppState(AppStateTypes::GUI_STATE));
                guiState->removeAllTooltips();
            }
        }
        
        ActiveGameState::UnitActionButton::UnitActionButton(GameManager *gm, unitData::UNIT_TYPE type, vector2di pos, vector2di size, stringw name, stringw path) : gui::Button(gm,pos,size,name,true){
            this->type=type;
            IVideoDriver *driver=gameManager->getDevice()->getVideoDriver();
            activeState=((ActiveGameState*)gm->getAppState(AppStateTypes::ACTIVE_STATE));
            setImageButton(new Image(gameManager,driver->getTexture(path),pos,vector2di(50,50)));
        }

        void ActiveGameState::UnitActionButton::onClick(){
            std::vector<Unit*> &selectedUnits=activeState->getSelectedUnits();
            for(Unit *u : selectedUnits)
                if(u->getType()==type)
                    units.push_back(u);
        }

        void ActiveGameState::UnitActionButton::onMouseOver(){
            if(!mouseOverDone){
                GuiAppState *guiState=((GuiAppState*)gameManager->getAppState(AppStateTypes::GUI_STATE));
                guiState->addTooltip(new gui::Tooltip(gameManager,pos-vector2di(100,0),name));
            }
        }

        void ActiveGameState::UnitActionButton::onMouseAway(){
            if(!mouseAwayDone){
                GuiAppState *guiState=((GuiAppState*)gameManager->getAppState(AppStateTypes::GUI_STATE));
                guiState->removeAllTooltips();
            }
        }
        
        ActiveGameState::ActiveGameState(GameManager *gM, GuiAppState *guiState, Map *map, vector<Player*> players, int playerId) {
            gameManager = gM;
            type = AppStateTypes::ACTIVE_STATE;
            this->guiState = guiState;
            this->map = map;
            this->players = players;
            cam = gM->getDevice()->getSceneManager()->addCameraSceneNodeFPS(0, 100, 0.f, -1, nullptr, 10, false, 0.6f);
            cam->setInputReceiverEnabled(false);
            gameManager->getDevice()->getCursorControl()->setVisible(true);
            this->playerId = playerId;
            mainPlayer = players[playerId];
            cam->setPosition(vector3df(0, 5, 0));
            quaternion rotQuat = rotQuat.fromAngleAxis(PI / 6, vector3df(1, 0, 0));
            cam->setTarget(vector3df(0, 5, 0) + rotQuat * vector3df(0, 0, 1));
        }

        ActiveGameState::~ActiveGameState() {
        }

        void ActiveGameState::onAttachment() {
            AbstractAppState::onAttachment();
            IVideoDriver *driver=gameManager->getDevice()->getVideoDriver();
            int faction=mainPlayer->getFaction(),width=gameManager->getWidth(),heigh=gameManager->getHeight(),size=50;
            stringw names[5]={"Battleship","Destroyer","Cruiser","Carrier","Submarine"};
            for(int i=0;i<5;i++){
                int id=2*i+(i==4&&faction==1?0:faction);
                vector2di pos=vector2di(width-size-1,1+i*size),sizeVec=vector2di(size,size);
                UnitButton *button=new UnitButton(gameManager,this,pos,sizeVec,names[i],faction,id);
                if(i==3)
                    names[i]="aircraftCarrier";
                else
                    names[i].make_lower();
                button->setImageButton(new Image(gameManager,driver->getTexture(PATH+"Textures/Icons/"+names[i]+"0"+stringw(faction)+".png"),pos,sizeVec));
                guiState->addButton(button);
            }
        }

        void ActiveGameState::onDetachment() {
            AbstractAppState::onDetachment();
        }

        void ActiveGameState::update() {
            vector<Unit*> units;
            for (Player *p : players)
                for (Unit *u : p->getUnits())
                    units.push_back(u);
            if (isSelectionBox)
                updateSelectionBox();
            for (Unit *u : units) {
                u->updateUnitGUIInfo(cam, camDir, camLeft, camUp);
                vector2d<s32> pos = u->getScreenPos();
                if (mainPlayer->isThisPlayersUnit(u)){
                    bool selected=false;
                    for(int i=0;i<selectedUnits.size()&&!selected;i++)
                        if(u==selectedUnits[i])
                            selected=true;
                    if(!u->isSelected()&&selected)
                        u->toggleSelection(true);
                    else if(!selected)
                        u->toggleSelection(false);
                    if(isSelectionBox && u->isSelectable() && 
                        pos.X >= selectionBoxOrigin.X && pos.X <= selectionBoxEnd.X &&
                        pos.Y >= selectionBoxOrigin.Y && pos.Y <= selectionBoxEnd.Y){
                        if(!selected){
                            if(!shiftPressed&&u==mainPlayer->getUnit(0))
                                while(!selectedUnits.empty())
                                    selectedUnits.pop_back();
                            selectedUnits.push_back(u);
                        }
                    }
                }
                if (u->isDebuggable())
                    u->debug();
            }
            updateVectors();
            renderUnits(units);
            if (cam->getPosition().X > map->getSize().X / 2)
                cam->setPosition(vector3df(map->getSize().X / 2, cam->getPosition().Y, cam->getPosition().Z));
            else if (cam->getPosition().X < -map->getSize().X / 2)
                cam->setPosition(vector3df(-map->getSize().X / 2, cam->getPosition().Y, cam->getPosition().Z));
            if (cam->getPosition().Z > map->getSize().Y / 2)
                cam->setPosition(vector3df(cam->getPosition().X, cam->getPosition().Y, map->getSize().Y / 2));
            else if (cam->getPosition().Z < -map->getSize().Y / 2)
                cam->setPosition(vector3df(cam->getPosition().X, cam->getPosition().Y, -map->getSize().Y / 2));
            cam->setTarget(cam->getPosition() + camDir);
            renderGUIBorders();
            renderActionButtons();
        }

        void ActiveGameState::renderUnits(vector<Unit*> units) {
            ISceneManager *smgr = gameManager->getDevice()->getSceneManager();
            for (Unit *rendUn : units) {
                vector3df rendUnPos = rendUn->getPos();
                rendUnPos.Y = 0;
                if (mainPlayer == rendUn->getPlayer()) {
                    rendUn->getNode()->setVisible(true);
                    rendUn->getLight()->setVisible(true);
                } else
                    for (int i = 0; i < units.size() && units[i] != rendUn && units[i]->getPlayer() == mainPlayer; i++) {
                        Unit *compUn = units[i];
                        vector3df compUnPos = compUn->getPos();
                        compUnPos.Y = 0;
                        float dist = compUnPos.getDistanceFrom(rendUnPos);
                        rendUn->getNode()->setVisible(dist <= compUn->getLineOfSight() || isInLineOfSight(compUnPos, compUn->getLineOfSight(), rendUn) ? true : false);
                    }
            }
        }

        void ActiveGameState::renderGUIBorders(){
            IVideoDriver *driver=gameManager->getDevice()->getVideoDriver();
            int width=gameManager->getWidth(),height=gameManager->getHeight(),size=50;
            for(int i=0;i<height/size;i++){
                driver->draw2DRectangleOutline(recti(vector2di(width-(2*size+3),size*i),dimension2di(size+2,size+2)));
                driver->draw2DRectangleOutline(recti(vector2di(width-(size+2),size*i),dimension2di(size+2,size+2)));
            }
        }

        void ActiveGameState::renderActionButtons(){
            class MakeJetButton : public UnitActionButton{
            public:
                MakeJetButton(GameManager *gm,int faction,unitData::UNIT_TYPE type,vector2di pos, vector2di size):UnitActionButton(gm,type,pos,size,"Jet",PATH+"Textures/Icons/jet0"+stringw(faction)+".png"){
                }
                ~MakeJetButton(){}
                void onClick(){
                    UnitActionButton::onClick();
                    for(int i=0;i<units.size();i++)
                        ((AircraftCarrier*)units[i])->makeJet();
                }
            private:
            };
            class LaunchButton : public UnitActionButton{
            public:
                LaunchButton(GameManager *gm,int faction,unitData::UNIT_TYPE type,vector2di pos,vector2di size) : UnitActionButton(gm,type,pos,size,"Launch",PATH+"Textures/Icons/guidedMissile0"+stringw(faction)+".png"){}
                ~LaunchButton(){}
                void onClick(){
                    UnitActionButton::onClick();
                    activeState->setSelectingLaunchPoint(true);
                }
            private:
            };
            class MissileButton : public UnitActionButton{
            public:
                MissileButton(GameManager *gm,bool aam,vector2di pos,vector2di size,stringw name,stringw path) : UnitActionButton(gm,unitData::UNIT_TYPE::MISSILE_JET,pos,size,name,path){
                    this->aam=aam;
                }
                ~MissileButton(){}
                void onClick(){
                    UnitActionButton::onClick();
                    for(int i=0;i<units.size();i++)
                        ((MissileJet*)units[i])->installMissiles(aam);
                }
            private:
                bool aam;
            };
            int width=gameManager->getWidth(),height=gameManager->getHeight(),slotSize=size+2,faction=mainPlayer->getFaction();
            bool carriers=false,cruisers=false,missileJets=false;
            for(int i=0;i<selectedUnits.size()&&!(carriers&&cruisers&&missileJets);i++){
                if(selectedUnits[i]->getType()==unitData::UNIT_TYPE::AIRCRAFT_CARRIER)
                    carriers=true;
                else if(selectedUnits[i]->getType()==unitData::UNIT_TYPE::CRUISER)
                    cruisers=true;
                else if(selectedUnits[i]->getType()==unitData::UNIT_TYPE::MISSILE_JET)
                    missileJets=true;
            }
            if(carriers&&!actionButtons[0]){
                actionButtons[0]=new MakeJetButton(gameManager,faction,unitData::UNIT_TYPE::AIRCRAFT_CARRIER,vector2di(width-2*slotSize,0),vector2di(size,size));
                guiState->addButton(actionButtons[0]);
            }
            else if(!carriers&&actionButtons[0]){
                guiState->removeButton(actionButtons[0]);
                actionButtons[0]=nullptr;
            }
            if(cruisers&&!actionButtons[1]){
                actionButtons[1]=new LaunchButton(gameManager,faction,unitData::UNIT_TYPE::CRUISER,vector2di(width-2*slotSize,slotSize),vector2di(size,size));
                guiState->addButton(actionButtons[1]);
            }
            else if(!cruisers&&actionButtons[1]){
                guiState->removeButton(actionButtons[1]);
                actionButtons[1]=nullptr;
            }
            if(missileJets&&!actionButtons[2]){
                actionButtons[2]=new MissileButton(gameManager,true,vector2di(width-2*slotSize,slotSize*3),vector2di(size,size),"AAM",PATH+"Textures/Icons/aam.png");
                actionButtons[3]=new MissileButton(gameManager,false,vector2di(width-2*slotSize,slotSize*4),vector2di(size,size),"AWM",PATH+"Textures/Icons/awm.png");
                guiState->addButton(actionButtons[2]);
                guiState->addButton(actionButtons[3]);
            }
            else if(!missileJets&&actionButtons[2]){
                guiState->removeButton(actionButtons[2]);
                guiState->removeButton(actionButtons[3]);
                actionButtons[2]=nullptr;
                actionButtons[3]=nullptr;
            }
        }
        
        bool ActiveGameState::isInLineOfSight(vector3df center, float radius, Unit *u) {
            bool inside = false;
            for (int i = 0; i < 4 && !inside; i++) {
                if (center.getDistanceFrom(u->getCorner(i)) <= radius)
                    inside = true;
            }
            return inside;
        }

        void ActiveGameState::updateSelectionBox() {
            vector2d<s32> mousePos = gameManager->getDevice()->getCursorControl()->getPosition();
            IVideoDriver *driver = gameManager->getDevice()->getVideoDriver();
            if (mousePos.X >= clickPoint.X && mousePos.Y >= clickPoint.Y) {
                selectionBoxOrigin = vector2d<s32>(clickPoint.X, clickPoint.Y);
                selectionBoxEnd = vector2d<s32>(mousePos.X, mousePos.Y);
            } else if (mousePos.X < clickPoint.X && mousePos.Y > clickPoint.Y) {
                selectionBoxOrigin = vector2d<s32>(mousePos.X, clickPoint.Y);
                selectionBoxEnd = vector2d<s32>(clickPoint.X, mousePos.Y);
            } else if (mousePos.X >= clickPoint.X && mousePos.Y < clickPoint.Y) {
                selectionBoxOrigin = vector2d<s32>(clickPoint.X, mousePos.Y);
                selectionBoxEnd = vector2d<s32>(mousePos.X, clickPoint.Y);
            } else {
                selectionBoxOrigin = vector2d<s32>(mousePos.X, mousePos.Y);
                selectionBoxEnd = vector2d<s32>(clickPoint.X, clickPoint.Y);
            }
            driver->draw2DRectangle(SColor(10, 255, 255, 255), rect<s32>(selectionBoxOrigin.X, selectionBoxOrigin.Y, selectionBoxEnd.X, selectionBoxEnd.Y), nullptr);
        }

        void ActiveGameState::updateVectors() {
            vector2d<s32> cursorPos = gameManager->getDevice()->getCursorControl()->getPosition();
            camDir = (cam->getTarget() - cam->getPosition()).normalize();
            float vecAngle = getAngleBetween(vector3df(0, 0, 1), vector3df(camDir.X, 0, camDir.Z));
            quaternion rotQuat;
            if (quaternion(0, 0, 0, 0).fromAngleAxis(vecAngle, vector3df(0, 1, 0)) * vector3df(0, 0, 1) != vector3df(camDir.X, 0, camDir.Z).normalize()) {
                rotQuat.fromAngleAxis(-vecAngle + PI / 2, vector3df(0, 1, 0));
                camLeft = rotQuat * vector3df(0, 0, -1);
            } else {
                rotQuat.fromAngleAxis(vecAngle - PI / 2, vector3df(0, 1, 0));
                camLeft = rotQuat * vector3df(0, 0, 1);
            }
            vector3df forwVec = quaternion(0, 0, 0, 0).fromAngleAxis(PI / 2, vector3df(0, 1, 0)) * camLeft;
            camUp = quaternion(0, 0, 0, 0).fromAngleAxis(PI / 2, camLeft.normalize()) * camDir;
            if (cursorPos.X == 0 && cursorPos.Y < gameManager->getHeight() && cursorPos.Y > 0) {
                cam->setPosition(cam->getPosition() + camLeft * camPanSpeed);
                cam->setTarget(cam->getPosition() + camDir);
            }//<-
            if (cursorPos.X >= gameManager->getWidth() - 1 && cursorPos.Y < gameManager->getHeight() && cursorPos.Y > 0) {
                cam->setPosition(cam->getPosition() - camLeft * camPanSpeed);
                cam->setTarget(cam->getPosition() + camDir);
            }//->
            if (cursorPos.X > 0 && cursorPos.X < gameManager->getWidth() && cursorPos.Y == 0) {
                cam->setPosition(cam->getPosition() + (forwVec.normalize()) * camPanSpeed);
                cam->setTarget(cam->getPosition() + camDir);
            }//^
            if (cursorPos.X > 0 && cursorPos.X < gameManager->getWidth() && cursorPos.Y >= gameManager->getHeight() - 1) {
                cam->setPosition(cam->getPosition() - (forwVec.normalize()) * camPanSpeed);
                cam->setTarget(cam->getPosition() + camDir);
            }
        }

        void ActiveGameState::issueOrder(Order::TYPE type, vector<vector3df*> pos, bool addOrder) {
            Order o;
            o.type = type;
            o.targetPos = pos;
            for (Unit *u : selectedUnits) {
                if (type != Order::TYPE::LAUNCH || (u->getId() == 4 || u->getId() == 5)) {
                    if (addOrder)
                        u->addOrder(o);
                    else
                        u->setOrder(o);
                }
            }
            for (vector3df *v : orderPos)
                orderPos.pop_back();
        }
        
        void ActiveGameState::addPos() {
            vector3df camPos = cam->getPosition();
            vector3df topLeft=cam->getViewFrustum()->getNearLeftUp();
            vector3df topRight=cam->getViewFrustum()->getNearRightUp();
            vector3df bottomLeft=cam->getViewFrustum()->getNearLeftDown();
            vector2d<int> mousePos=gameManager->getDevice()->getCursorControl()->getPosition();
            vector3df p=topLeft;
            p+=(topRight-topLeft)*((float)mousePos.X/gameManager->getWidth());
            p+=(bottomLeft-topLeft)*((float)mousePos.Y/gameManager->getHeight());
            vector3df orderDir=(p-camPos).normalize();
            
            ISceneManager *smgr = gameManager->getDevice()->getSceneManager();
            ISceneCollisionManager *collMan = smgr->getSceneCollisionManager();
            line3d<float> ray;
            ray.start = camPos;
            ray.end = camPos + orderDir * map->getSize().X * 1000;
            triangle3df t;
            vector3df collPoint;
            ISceneNode *collNode = collMan->getSceneNodeAndCollisionPointFromRay(ray, collPoint, t, 0, 0);
            if (collNode) {
                for (Player *p : players)
                    for (int i = 0; i < p->getNumberOfUnits(); i++)
                        if (collNode == p->getUnit(i)->getNode()) {
                            orderPos.push_back(p->getUnit(i)->getPosPtr());
                        }
            } 
            else {
                if(orderDir.Y>0)
                    orderDir.Y*=-1;
                float angle = getAngleBetween(vector3df(0, -1, 0), orderDir);
                float hyp = camPos.Y / cos(angle);
                vector3df *v=new vector3df(orderDir * hyp + camPos);
                (*v).Y=0;
                orderPos.push_back(v);
            }
            if (!selectingPatrolPoints) {
                Order::TYPE t = Order::TYPE::MOVE;
                if (selectingGuidedMissileTarget)
                    t = Order::TYPE::LAUNCH;
                else if (controlPressed)
                    t = Order::TYPE::ATTACK;
                issueOrder(t, orderPos, false);
            }
        }
        
        void ActiveGameState::onAction(Bind bind, bool isPressed) {
            InGameAppState *inGameState = (InGameAppState*) gameManager->getAppState(AppStateTypes::IN_GAME_STATE);
            switch(bind){
                case DRAG_BOX: 
                    isSelectionBox = isPressed;
                    if (isPressed) {
                        clickPoint = gameManager->getDevice()->getCursorControl()->getPosition();
                        if (!selectedUnits.empty())
                            addPos();
                    }
                    break;
                case DESELECT:
                    if (isPressed) {
                        while(!selectedUnits.empty())
                            selectedUnits.pop_back();
                    }
                    break;
                case TOGGLE_SUB:
                    if (isPressed) {
                        for (Unit *u : selectedUnits) {
                            if (u->getPlayer()==mainPlayer && u->getType() == unitData::UNIT_TYPE::SUBMARINE) {
                                Submarine *s = (Submarine*) u;
                                if (s->isSubmerged())
                                    s->emerge();
                                else
                                    s->submerge();
                            }
                        }
                    }
                    break;
                case ZOOM_IN:
                    if (zooms > -10) {
                        cam->setPosition(cam->getPosition() + camDir.normalize()*.5f);
                        cam->setTarget(cam->getPosition() + camDir);
                        zooms--;
                    }
                    break;
                case ZOOM_OUT:
                    if (zooms < 10) {
                        cam->setPosition(cam->getPosition() - (camDir.normalize()*.5f));
                        cam->setTarget(cam->getPosition() + camDir);
                        zooms++;
                    }
                    break;
                case LOOK_AROUND:
                    cam->setInputReceiverEnabled(isPressed);
                    gameManager->getDevice()->getCursorControl()->setVisible(!isPressed);
                    lookingAround=isPressed;
                    break;
                case HALT: 
                    for (Unit *u : selectedUnits)
                        u->halt();
                    break;
                case LEFT_CONTROL:
                    controlPressed = isPressed;
                    break;
                case LEFT_SHIFT:
                    shiftPressed=isPressed;
                    break;
                case SELECT_PATROL_POINTS: 
                    selectingPatrolPoints = isPressed;
                    if (!isPressed && orderPos.size() > 0) {
                        /*
                        orderPos.push_back(vector3df(0, 0, 0));
                        swap(orderPos[0], orderPos[orderPos.size() - 1]);
                        issueOrder(Order::TYPE::PATROL, orderPos, false);
                        */
                    }
                    break;
                case LAUNCH: 
                    selectingGuidedMissileTarget = isPressed;
                    break;
                case INSTALL_AAM:
                case INSTALL_AWM:
                    for(Unit *u : selectedUnits)
                        if(u->getType()==unitData::UNIT_TYPE::MISSILE_JET)
                            ((MissileJet*)u)->installMissiles(bind==INSTALL_AAM);
                    break;
                case GROUP_0:
                case GROUP_1:
                case GROUP_2:
                case GROUP_3:
                case GROUP_4:
                case GROUP_5:
                case GROUP_6:
                case GROUP_7:
                case GROUP_8:
                case GROUP_9:
                    if(isPressed){
                        int group=bind-GROUP_0;
                        if(controlPressed){
                            unitGroups[group]=selectedUnits;
                        }
                        else{
                            if(!shiftPressed)
                                selectedUnits=unitGroups[group];
                            else
                                for(Unit *u : unitGroups[group])
                                    selectedUnits.push_back(u);
                        }
                    }
                    break;
            }
        }

        void ActiveGameState::onAnalog(Bind bind, double strength) {}
    }
}
