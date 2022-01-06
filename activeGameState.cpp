#include <root.h>
#include <quaternion.h>
#include <model.h>
#include <material.h>
#include <quad.h>
#include <ray.h>

#include <stateManager.h>

#include <algorithm>
#include <ctype.h>
#include <stdlib.h>

#include "activeGameState.h"
#include "inGameAppState.h"
#include "util.h"
#include "cruiser.h"
#include "destroyer.h"
#include "submarine.h"
#include "aircraftCarrier.h"
#include "demoJet.h"
#include "missileJet.h"
#include "tooltip.h"

using namespace vb01;
using namespace vb01Gui;
using namespace std;

namespace battleship{
		using namespace configData;

    const int size = 50;
    
    ActiveGameState::UnitButton::UnitButton(ActiveGameState *activeState, Vector2 pos, Vector2 size, std::string name, int faction, int unitId) : vb01Gui::Button(pos, size, name, PATH + "Fonts/batang.ttf", -1, true){
        this->activeState=activeState;
        this->faction=faction;
        this->unitId=unitId;
    }
    
    void ActiveGameState::UnitButton::onClick(){
        Player *player=activeState->getPlayer();
        Vector3 spawnPoint=player->getSpawnPoint();
        Unit *u;

        switch(unitData::unitType[unitId]){
            case unitData::UNIT_TYPE::VESSEL:
                u=new Vessel(player,spawnPoint,unitId);
                break;
            case unitData::UNIT_TYPE::DESTROYER:
                u=new Destroyer(player,spawnPoint,unitId);
                break;
            case unitData::UNIT_TYPE::CRUISER:
                u=new Cruiser(player,spawnPoint,unitId);
                break;
            case unitData::UNIT_TYPE::AIRCRAFT_CARRIER:
                u=new AircraftCarrier(player,spawnPoint,unitId);
                break;
            case unitData::UNIT_TYPE::SUBMARINE:
                u=new Submarine(player,spawnPoint,unitId);
                break;
        }

        player->addUnit(u);
    }
    
    void ActiveGameState::UnitButton::onMouseOver(){
				/*
        if(!mouseOverDone){
						GameManager *gm = GameManager::getSingleton();
            GuiAppState *guiState=((GuiAppState*)GameManager::getSingleton()->getStateManager()->getAppState(AppStateTypes::GUI_STATE));
            guiState->addTooltip(new gui::Tooltip(pos-Vector2(100,0),name));
        }
				*/
    }

    void ActiveGameState::UnitButton::onMouseAway(){
				/*
        if(!mouseAwayDone){
            GuiAppState *guiState=((GuiAppState*)GameManager::getSingleton()->getStateManager()->getAppState(AppStateTypes::GUI_STATE));
            guiState->removeAllTooltips();
        }
				*/
    }
    
    ActiveGameState::UnitActionButton::UnitActionButton(unitData::UNIT_TYPE type, Vector2 pos, Vector2 size, string name, string path) : vb01Gui::Button(pos, size, name, "", -1, true){
        this->type=type;
				GameManager *gm = GameManager::getSingleton();
        activeState=((ActiveGameState*)gm->getStateManager()->getAppStateByType(AppStateType::ACTIVE_STATE));
				/*
        IVideoDriver *driver = gm->getDevice()->getVideoDriver();
        setImageButton(new Image(driver->getTexture(path),pos,vector2di(50,50)));
				*/
    }

    void ActiveGameState::UnitActionButton::onClick(){
        std::vector<Unit*> &selectedUnits=activeState->getSelectedUnits();
        for(Unit *u : selectedUnits)
            if(u->getType()==type)
                units.push_back(u);
    }

    void ActiveGameState::UnitActionButton::onMouseOver(){
				/*
        if(!mouseOverDone){
            GuiAppState *guiState=((GuiAppState*)GameManager::getSingleton()->getStateManager()->getAppState(AppStateTypes::GUI_STATE));
            guiState->addTooltip(new gui::Tooltip(pos-Vector2(100,0),name));
        }
				*/
    }

    void ActiveGameState::UnitActionButton::onMouseAway(){
				/*
        if(!mouseAwayDone){
            GuiAppState *guiState=((GuiAppState*)GameManager::getSingleton()->getStateManager()->getAppState(AppStateTypes::GUI_STATE));
            guiState->removeAllTooltips();
        }
				*/
    }
    
    ActiveGameState::ActiveGameState(GuiAppState *guiState, Map *map, vector<Player*> players, int playerId) {
				type = AppStateType::ACTIVE_STATE;
        this->guiState = guiState;
        this->map = map;
        this->players = players;

				GameManager *gm = GameManager::getSingleton();
				Root *root = Root::getSingleton();
        Camera *cam = root->getCamera();

        this->playerId = playerId;
        mainPlayer = players[playerId];
        cam->setPosition(Vector3(0, 5, 0));
        Quaternion rotQuat = Quaternion(4 * atan(1) / 6, Vector3(1, 0, 0));
				
				Material *mat = new Material(root->getLibPath() + "gui");
				mat->addBoolUniform("texturingEnabled", false);
				mat->addBoolUniform("diffuseColorEnabled", false);
				mat->addVec4Uniform("diffuseColor", Vector4(.5, .5, .5, .5));
				dragbox = new Quad(Vector3::VEC_ZERO, false);
				dragbox->setMaterial(mat);
				dragboxNode = new Node();
				dragboxNode->attachMesh(dragbox);
				root->getGuiNode()->attachChild(dragboxNode);
    }

    ActiveGameState::~ActiveGameState() {
    }

    void ActiveGameState::onAttached() {
				readFile(PATH + "../options.cfg", bindingsLines, configData::numBinds[AppStateType::IN_GAME_STATE]);

        AbstractAppState::onAttached();

				GameManager *gm = GameManager::getSingleton();
        int faction = mainPlayer->getFaction(), width = gm->getWidth(), height = gm->getHeight(), size = 50;
        string names[5] = {"Battleship", "Destroyer", "Cruiser", "Carrier", "Submarine"};

				/*
        for(int i = 0; i < 5; i++){
            int id = 2 * i + (i == 4 && faction == 1 ? 0 : faction);
            Vector2 pos = Vector2(width - size - 1, 1 + i * size), sizeVec = Vector2(size, size);
            UnitButton *button = new UnitButton(this, pos, sizeVec, names[i], faction, id);

            if(i == 3)
                names[i] = "aircraftCarrier";
            else
								transform(names[i].begin(), names[i].end(), names[i], names[i]);

            //button->setImageButton(new Image(driver->getTexture(PATH+"Textures/Icons/"+names[i]+"0"+stringw(faction)+".png"),pos,sizeVec));
            guiState->addButton(button);
        }
				*/

				Bind binds[]{Bind::LOOK_UP, Bind::LOOK_DOWN, Bind::LOOK_LEFT, Bind::LOOK_RIGHT};
				int triggers[]{Mapping::MOUSE_AXIS_UP, Mapping::MOUSE_AXIS_DOWN, Mapping::MOUSE_AXIS_LEFT, Mapping::MOUSE_AXIS_RIGHT};
				int numData = sizeof(triggers) / sizeof(int);

				for(int i = 0; i < numData; i++){
					Mapping *m = new Mapping;
					m->bind = binds[i];
					m->trigger = triggers[i];
					m->action = false;
					m->type = Mapping::MOUSE_AXIS;

					mappings.push_back(m);
				}

				Mapping *rightClick = new Mapping;
				rightClick->bind = Bind::LOOK_AROUND;
				rightClick->type = Mapping::MOUSE_KEY;
				rightClick->trigger = 1;
				rightClick->action = true;

				Mapping *leftClick = new Mapping;
				leftClick->bind = Bind::DRAG_BOX;
				leftClick->type = Mapping::MOUSE_KEY;
				leftClick->trigger = 0;
				leftClick->action = true;

				mappings.push_back(rightClick);
				mappings.push_back(leftClick);
    }

    void ActiveGameState::onDettached() {
        AbstractAppState::onDettached();
    }

    void ActiveGameState::update() {
        if (isSelectionBox)
            updateSelectionBox();

				dragboxNode->setVisible(isSelectionBox);

        renderUnits();
        renderGUIBorders();
        renderActionButtons();

				if(!lookingAround)
					updateCameraPosition();
    }

    void ActiveGameState::renderUnits() {
				vector<Unit*> units;

        for (Player *p : players)
            for (Unit *u : p->getUnits())
                units.push_back(u);

        for (Unit *u : units) {
            if (mainPlayer->isThisPlayersUnit(u)){
            		u->updateUnitGUIInfo();
                bool selected = false;

                for(int i = 0; i < selectedUnits.size() && !selected; i++)
                    if(u == selectedUnits[i])
                        selected = true;

                if(!u->isSelected() && selected)
                    u->toggleSelection(true);
                else if(!selected)
                    u->toggleSelection(false);

								Vector3 selectionBoxOrigin = dragboxNode->getPosition();
								Vector3 selectionBoxEnd = selectionBoxOrigin + dragbox->getSize();
            		Vector2 pos = u->getScreenPos();

                if(isSelectionBox && 
                    pos.x >= selectionBoxOrigin.x && pos.x <= selectionBoxEnd.x &&
                    pos.y >= selectionBoxOrigin.y && pos.y <= selectionBoxEnd.y){

                    if(!selected){
                        if(!shiftPressed && u == mainPlayer->getUnit(0))
                            while(!selectedUnits.empty())
                                selectedUnits.pop_back();

                        selectedUnits.push_back(u);
                    }
                }
            }

            if (u->isDebuggable())
                u->debug();

            Vector3 rendUnPos = u->getPos();
            rendUnPos.y = 0;

            if (mainPlayer == u->getPlayer()) {
                u->getNode()->setVisible(true);
                //rendUn->getLight()->setVisible(true);
            } else
                for (int i = 0; i < units.size() && units[i] != u && units[i]->getPlayer() == mainPlayer; i++) {
                    Unit *compUn = units[i];
                    Vector3 compUnPos = compUn->getPos();
                    compUnPos.y = 0;
                    float dist = compUnPos.getDistanceFrom(rendUnPos);
                    u->getNode()->setVisible(dist <= compUn->getLineOfSight() || isInLineOfSight(compUnPos, compUn->getLineOfSight(), u) ? true : false);
                }
        }
    }

    void ActiveGameState::renderGUIBorders(){
				/*
				GameManager *gm = GameManager::getSingleton();
        IVideoDriver *driver = gm->getDevice()->getVideoDriver();
        int width = gm->getWidth(), height = gm->getHeight(), size = 50;

        for(int i=0;i<height/size;i++){
            driver->draw2DRectangleOutline(recti(vector2di(width-(2*size+3),size*i),dimension2di(size+2,size+2)));
            driver->draw2DRectangleOutline(recti(vector2di(width-(size+2),size*i),dimension2di(size+2,size+2)));
        }
				*/
    }

    void ActiveGameState::renderActionButtons(){
        class MakeJetButton : public UnitActionButton{
        public:
            MakeJetButton(int faction, unitData::UNIT_TYPE type, Vector2 pos, Vector2 size):UnitActionButton(type, pos, size, "Jet", PATH + "Textures/Icons/jet0" + to_string(faction) + ".png"){
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
            LaunchButton(int faction, unitData::UNIT_TYPE type, Vector2 pos, Vector2 size) : UnitActionButton(type, pos, size, "Launch", PATH + "Textures/Icons/guidedMissile0" + to_string(faction) + ".png"){}
            ~LaunchButton(){}
            void onClick(){
                UnitActionButton::onClick();
                activeState->setSelectingLaunchPoint(true);
            }
        private:
        };
        class MissileButton : public UnitActionButton{
        public:
            MissileButton(bool aam, Vector2 pos, Vector2 size, string name, string path) : UnitActionButton(unitData::UNIT_TYPE::MISSILE_JET,pos,size,name,path){
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

				GameManager *gm = GameManager::getSingleton();
        int width = gm->getWidth(), height = gm->getHeight(), slotSize = size + 2, faction = mainPlayer->getFaction();
        bool carriers=false, cruisers=false, missileJets=false;

        for(int i=0;i<selectedUnits.size()&&!(carriers&&cruisers&&missileJets);i++){
            if(selectedUnits[i]->getType()==unitData::UNIT_TYPE::AIRCRAFT_CARRIER)
                carriers=true;
            else if(selectedUnits[i]->getType()==unitData::UNIT_TYPE::CRUISER)
                cruisers=true;
            else if(selectedUnits[i]->getType()==unitData::UNIT_TYPE::MISSILE_JET)
                missileJets=true;
        }
        if(carriers&&!actionButtons[0]){
            actionButtons[0] = new MakeJetButton(faction, unitData::UNIT_TYPE::AIRCRAFT_CARRIER, Vector2(width - 2 * slotSize, 0), Vector2(size, size));
            guiState->addButton(actionButtons[0]);
        }
        else if(!carriers&&actionButtons[0]){
            guiState->removeButton(actionButtons[0]);
            actionButtons[0]=nullptr;
        }
        if(cruisers&&!actionButtons[1]){
            actionButtons[1] = new LaunchButton(faction, unitData::UNIT_TYPE::CRUISER, Vector2(width - 2 * slotSize, slotSize), Vector2(size, size));
            guiState->addButton(actionButtons[1]);
        }
        else if(!cruisers&&actionButtons[1]){
            guiState->removeButton(actionButtons[1]);
            actionButtons[1]=nullptr;
        }
        if(missileJets&&!actionButtons[2]){
            actionButtons[2]=new MissileButton(true, Vector2(width - 2 * slotSize, slotSize * 3), Vector2(size, size), "AAM", PATH + "Textures/Icons/aam.png");
            actionButtons[3]=new MissileButton(false, Vector2(width - 2 * slotSize, slotSize * 4), Vector2(size, size), "AWM", PATH + "Textures/Icons/awm.png");
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
    
    bool ActiveGameState::isInLineOfSight(Vector3 center, float radius, Unit *u) {
        bool inside = false;

        for (int i = 0; i < 4 && !inside; i++) {
            if (center.getDistanceFrom(u->getCorner(i)) <= radius)
                inside = true;
        }

        return inside;
    }

    void ActiveGameState::updateSelectionBox() {
        Vector2 mousePos = getCursorPos(), selectionBoxOrigin, selectionBoxEnd;

        if (mousePos.x >= clickPoint.x && mousePos.y >= clickPoint.y) {
            selectionBoxOrigin = Vector2(clickPoint.x, clickPoint.y);
            selectionBoxEnd = Vector2(mousePos.x, mousePos.y);
        }
				else if (mousePos.x < clickPoint.x && mousePos.y > clickPoint.y) {
            selectionBoxOrigin = Vector2(mousePos.x, clickPoint.y);
            selectionBoxEnd = Vector2(clickPoint.x, mousePos.y);
        }
				else if (mousePos.x >= clickPoint.x && mousePos.y < clickPoint.y) {
            selectionBoxOrigin = Vector2(clickPoint.x, mousePos.y);
            selectionBoxEnd = Vector2(mousePos.x, clickPoint.y);
        }
				else {
            selectionBoxOrigin = Vector2(mousePos.x, mousePos.y);
            selectionBoxEnd = Vector2(clickPoint.x, clickPoint.y);
        }

				Vector3 size = Vector3(selectionBoxEnd.x - selectionBoxOrigin.x, selectionBoxEnd.y - selectionBoxOrigin.y, 0);
				dragbox->setSize(size);
				dragboxNode->setPosition(Vector3(selectionBoxOrigin.x, selectionBoxOrigin.y, 0));
    }

    void ActiveGameState::updateCameraPosition() {
				GameManager *gm = GameManager::getSingleton();
        Vector2 cursorPos = getCursorPos();

				Camera *cam = Root::getSingleton()->getCamera();
				Vector3 camDir = cam->getDirection();
        Vector3 forwVec = Vector3(camDir.x, 0, camDir.z).norm();
				Vector3 camLeft = cam->getLeft();
				camLeft = Vector3(camLeft.x, 0, camLeft.z).norm();

				int width, height;
				glfwGetWindowSize(Root::getSingleton()->getWindow(), &width, &height);

        if (cursorPos.x <= 0 && 0 < cursorPos.y && cursorPos.y < height) 
            cam->setPosition(cam->getPosition() - camLeft * camPanSpeed);

        if (cursorPos.x >= width && 0 < cursorPos.y && cursorPos.y < height) 
            cam->setPosition(cam->getPosition() + camLeft * camPanSpeed);

        if (cursorPos.y <= 0 && 0 < cursorPos.x && cursorPos.x < width) 
            cam->setPosition(cam->getPosition() + forwVec * camPanSpeed);
				
        if (cursorPos.y >= height && 0 < cursorPos.x && cursorPos.x < width) 
            cam->setPosition(cam->getPosition() - forwVec * camPanSpeed);
    }

    void ActiveGameState::issueOrder(Order::TYPE type, vector<Order::Target> targets, bool addOrder) {
        Order o;
        o.type = type;
        o.targets = targets;

				LineRenderer *lineRenderer = LineRenderer::getSingleton();
				Vector3 color;

      	switch(o.type){
      	    case Order::TYPE::MOVE:
								color = Vector3::VEC_J;
      	        break;
      	    case Order::TYPE::ATTACK:
								color = Vector3::VEC_I;
      	        break;
      	    case Order::TYPE::PATROL:
								color = Vector3::VEC_K;
      	    case Order::TYPE::LAUNCH:
								color = Vector3(1, 1, 0);
      	        break;
      	}

        for (int i = 0; i < selectedUnits.size(); ++i) {
						Unit *u = selectedUnits[i];
						lineRenderer->addLine(u->getPos(), *targets[i].pos, color);
						vector<LineRenderer::Line> lines = lineRenderer->getLines();
						o.line = lines[lines.size() - 1];

            if (type != Order::TYPE::LAUNCH || (u->getId() == 4 || u->getId() == 5)) {
                if (addOrder)
                    u->addOrder(o);
                else
                    u->setOrder(o);
            }
        }

        for (Vector3 *v : orderPos)
            orderPos.pop_back();
    }
    
    void ActiveGameState::addPos() {
				Camera *cam = Root::getSingleton()->getCamera();
				Vector3 camPos = cam->getPosition();
				Vector3 endPos = screenToSpace(getCursorPos());

				vector<Ray::CollisionResult> results;
				Ray::retrieveCollisions(camPos, (endPos - camPos).norm(), map->getWaterNode(), results);
				Ray::sortResults(results);

				if(!results.empty())
						issueOrder(Order::TYPE::MOVE, vector<Order::Target>{Order::Target(false, new Vector3(results[0].pos))}, false);
    }
    
    void ActiveGameState::onAction(int bind, bool isPressed) {
				GameManager *gm = GameManager::getSingleton();
        InGameAppState *inGameState = (InGameAppState*) gm->getStateManager()->getAppStateByType((int)AppStateType::IN_GAME_STATE);

        switch((Bind)bind){
						case Bind::DRAG_BOX: 
                isSelectionBox = isPressed;

                if (isPressed) {
										clickPoint = getCursorPos();

                    if (!selectedUnits.empty())
                      addPos();
                }

                break;
						case Bind::DESELECT:
                if (isPressed)
                    while(!selectedUnits.empty())
                        selectedUnits.pop_back();
                break;
						case Bind::TOGGLE_SUB:
                if (isPressed) {
                    for (Unit *u : selectedUnits) {
                        if (u->getPlayer() == mainPlayer && u->getType() == unitData::UNIT_TYPE::SUBMARINE) {
                            Submarine *s = (Submarine*) u;

                            if (s->isSubmerged())
                                s->emerge();
                            else
                                s->submerge();
                        }
                    }
                }
                break;
						case Bind::ZOOM_IN:
                if (zooms > -10) {
										Camera *cam = Root::getSingleton()->getCamera();
                    cam->setPosition(cam->getPosition() + cam->getDirection().norm() * .5f);
                    zooms--;
                }
                break;
						case Bind::ZOOM_OUT:
                if (zooms < 10) {
										Camera *cam = Root::getSingleton()->getCamera();
                    cam->setPosition(cam->getPosition() - (cam->getDirection().norm().norm() * .5f));
                    zooms++;
                }
                break;
						case Bind::LOOK_AROUND:
                lookingAround = isPressed;
                break;
						case Bind::HALT: 
                for (Unit *u : selectedUnits)
                    u->halt();
                break;
						case Bind::LEFT_CONTROL:
                controlPressed = isPressed;
                break;
						case Bind::LEFT_SHIFT:
                shiftPressed=isPressed;
                break;
						case Bind::SELECT_PATROL_POINTS: 
                selectingPatrolPoints = isPressed;
                if (!isPressed && orderPos.size() > 0) {
                    /*
                    orderPos.push_back(vector3df(0, 0, 0));
                    swap(orderPos[0], orderPos[orderPos.size() - 1]);
                    issueOrder(Order::TYPE::PATROL, orderPos, false);
                    */
                }
                break;
						case Bind::LAUNCH: 
                selectingGuidedMissileTarget = isPressed;
                break;
						case Bind::INSTALL_AAM:
            case Bind::INSTALL_AWM:
                for(Unit *u : selectedUnits)
                    if(u->getType() == unitData::UNIT_TYPE::MISSILE_JET)
                        ((MissileJet*)u)->installMissiles(bind == Bind::INSTALL_AAM);
                break;
						case Bind::GROUP_0:
            case Bind::GROUP_1:
            case Bind::GROUP_2:
            case Bind::GROUP_3:
            case Bind::GROUP_4:
            case Bind::GROUP_5:
            case Bind::GROUP_6:
            case Bind::GROUP_7:
            case Bind::GROUP_8:
            case Bind::GROUP_9:
                if(isPressed){
                    int group = bind - Bind::GROUP_0;
                    if(controlPressed){
                        unitGroups[group] = selectedUnits;
                    }
                    else{
                        if(!shiftPressed)
                            selectedUnits = unitGroups[group];
                        else
                            for(Unit *u : unitGroups[group])
                                selectedUnits.push_back(u);
                    }
                }
                break;
        }
    }

		void ActiveGameState::orientCamera(Vector3 rotAxis, double str){
				float minStr = .001;
				Quaternion rotQuat = Quaternion(.5 * (fabs(str) > minStr ? str : 0), rotAxis);
				Camera *cam = Root::getSingleton()->getCamera();
				Vector3 dir = rotQuat * cam->getDirection();
				Vector3 up = rotQuat * cam->getUp();
				cam->lookAt(dir, up);
		}

    void ActiveGameState::onAnalog(int bind, float strength) {
				switch((Bind)bind){
						{
						case Bind::LOOK_UP: 
						case Bind::LOOK_DOWN: 
								if(lookingAround) {
									Vector3 dirProj = Root::getSingleton()->getCamera()->getDirection();
									dirProj = Vector3(dirProj.x, 0, dirProj.z).norm();
									orientCamera(Vector3(0, 1, 0).cross(dirProj), strength);
								}
								break;
						}
						case Bind::LOOK_LEFT: 
						case Bind::LOOK_RIGHT: 
								if(lookingAround)
									orientCamera(Vector3(0, 1, 0), strength);
								break;
				}
		}
}
