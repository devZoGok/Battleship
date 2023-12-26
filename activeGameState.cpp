#include <root.h>
#include <quaternion.h>
#include <model.h>
#include <material.h>
#include <quad.h>
#include <text.h>
#include <rayCaster.h>

#include <stateManager.h>
#include <solUtil.h>

#include <algorithm>
#include <ctype.h>
#include <stdlib.h>
#include <map>

#include "activeGameState.h"
#include "inGameAppState.h"
#include "defConfigs.h"
#include "structure.h"
#include "game.h"
#include "util.h"
#include "tooltip.h"
#include "gameObjectFrameController.h"
#include "gameObjectFactory.h"
#include "cameraController.h"
#include "concreteGuiManager.h"

using namespace vb01;
using namespace vb01Gui;
using namespace std;

namespace battleship{
	using namespace configData;
	using namespace gameBase;

    ActiveGameState::ActiveGameState(GuiAppState *guiState, int playerId) : AbstractAppState(
						AppStateType::ACTIVE_STATE,
					 	configData::calcSumBinds(AppStateType::ACTIVE_STATE, true),
					 	configData::calcSumBinds(AppStateType::ACTIVE_STATE, false),
					 	GameManager::getSingleton()->getPath() + scripts[(int)ScriptFiles::OPTIONS]){
        this->guiState = guiState;
        this->playerId = playerId;
        mainPlayer = Game::getSingleton()->getPlayer(playerId);
		depth = 1;

		initDragbox();
    }

    ActiveGameState::~ActiveGameState() {
    }

	void ActiveGameState::initDragbox(){
		Root *root = Root::getSingleton();

		Material *mat = new Material(root->getLibPath() + "gui");
		mat->addBoolUniform("texturingEnabled", false);
		mat->addBoolUniform("diffuseColorEnabled", false);
		mat->addVec4Uniform("diffuseColor", Vector4(.5, .5, .5, .5));

		Quad *dragbox = new Quad(Vector3::VEC_ZERO, false);
		dragbox->setMaterial(mat);
		dragboxNode = new Node();
		dragboxNode->attachMesh(dragbox);

		root->getGuiNode()->attachChild(dragboxNode);
	}

    void ActiveGameState::onAttached() {
        AbstractAppState::onAttached();
		ConcreteGuiManager::getSingleton()->readLuaScreenScript("activeGameState.lua");
    }

    void ActiveGameState::onDettached() {
		AbstractAppState::onDettached();
    }

    void ActiveGameState::update() {
		ConcreteGuiManager *guiManager = ConcreteGuiManager::getSingleton();
		guiManager->getText("depth")->setText(L"Depth: " + to_wstring(depth));
		guiManager->getText("refineds")->setText(L"Refineds: " + to_wstring(mainPlayer->getRefineds()));
		guiManager->getText("wealth")->setText(L"Wealth: " + to_wstring(mainPlayer->getWealth()));
		guiManager->getText("research")->setText(L"Research: " + to_wstring(mainPlayer->getResearch()));

		if(!isSelectionBox && leftMouseClicked && getTime() - lastLeftMouseClicked > 10)
			isSelectionBox = true;
		else if (isSelectionBox)
			updateSelectionBox();

		dragboxNode->setVisible(isSelectionBox);

		updateGameObjHoveredOn();

		vector<Unit*> selectedUnits = mainPlayer->getSelectedUnits();

		if(!selectingDestOrient && leftMouseClicked && !selectedUnits.empty() && getTime() - lastLeftMouseClicked > 100){
			selectingDestOrient = true;

			GameObjectFrameController *fc = GameObjectFrameController::getSingleton();
			fc->setPlacingFrames(true);
			fc->setRotatingFrames(true);

			for(Unit *unit : selectedUnits)
				fc->addGameObjectFrame(GameObjectFrame(unit->getId(), GameObject::Type::UNIT));
		}

        renderUnits();

		CameraController *camCtr = CameraController::getSingleton();

		if(!camCtr->isLookingAround())
			camCtr->updateCameraPosition();

		GameObjectFrameController *ufCtr = GameObjectFrameController::getSingleton();

		if(ufCtr->isPlacingFrames())
			ufCtr->update();
    }

	void ActiveGameState::updateGameObjHoveredOn(){
		vector<GameObject*> gameObjs;
		vector<Player*> players = Game::getSingleton()->getPlayers();

		for(Player *pl : players){
			vector<ResourceDeposit*> resDeps = pl->getResourceDeposits();
			vector<Unit*> units = pl->getUnits();

			for(ResourceDeposit *rd : resDeps)
				gameObjs.push_back((GameObject*)rd);

			for(Unit *u : units)
				gameObjs.push_back((GameObject*)u);
		}

		Vector2 cursorPos = getCursorPos();

		for(GameObject *gameObj : gameObjs){
			Vector2 rectSize = gameObj->calculateSelectionRect();
			Vector2 screenPos = gameObj->getScreenPos();

			if(fabs(cursorPos.x - screenPos.x) < .5 * rectSize.x && fabs(cursorPos.y - screenPos.y) < .5 * rectSize.y){
				gameObjHoveredOn = gameObj;
				return;
			}
		}

		gameObjHoveredOn = nullptr;
	}

	void ActiveGameState::deselectUnits(){
		mainPlayer->deselectUnits();
		GameObjectFrameController *ufCtr = GameObjectFrameController::getSingleton();
		ufCtr->removeGameObjectFrames();
		ufCtr->setPlacingFrames(false);

		ConcreteGuiManager::getSingleton()->removeAllGuiElements();
		buttons.clear();
	}

	//TODO fix fog of war for hostile units
    void ActiveGameState::renderUnits() {
		vector<Unit*> units;

        for (Player *p : Game::getSingleton()->getPlayers())
            for (Unit *u : p->getUnits())
                units.push_back(u);

		ConcreteGuiManager *guiManager = ConcreteGuiManager::getSingleton();

        for (Unit *u : units) {
            if (u->getPlayer() == mainPlayer){
			   	Vector3 dragboxSize = ((Quad*)dragboxNode->getMesh(0))->getSize();
				Vector3 dragboxOrigin = dragboxNode->getPosition(), dragboxEnd = dragboxOrigin + dragboxSize;
            	Vector2 pos = u->getScreenPos();

				if(buttons.empty()){
					if(!mainPlayer->getSelectedUnitsByClass(UnitClass::ENGINEER).empty())
						guiManager->readLuaScreenScript("engineerCommands.lua");
					else if(!mainPlayer->getSelectedUnitsByClass(UnitClass::LAND_FACTORY).empty())
						guiManager->readLuaScreenScript("landFactoryCommands.lua");
					else if(!mainPlayer->getSelectedUnitsByClass(UnitClass::NAVAL_FACTORY).empty())
						guiManager->readLuaScreenScript("navalFactoryCommands.lua");
				}

                if(isSelectionBox && fabs(pos.x - dragboxOrigin.x) < .5 * dragboxSize.x && fabs(pos.y - dragboxOrigin.y) < .5 * dragboxSize.y){
					vector<Unit*> selectedUnits = mainPlayer->getSelectedUnits();

                    if(find(selectedUnits.begin(), selectedUnits.end(), u) == selectedUnits.end()){
                        if(!shiftPressed)
							deselectUnits();

                        mainPlayer->selectUnit(u);
                		u->select();
                    }
                }
            }
			else{
            	Vector3 rendUnPos = u->getPos();
            	rendUnPos.y = 0;

                for (int i = 0; i < units.size() && units[i] != u && units[i]->getPlayer() == mainPlayer; i++) {
                    Vector3 compUnPos = units[i]->getPos();
                    compUnPos.y = 0;
                    float dist = compUnPos.getDistanceFrom(rendUnPos);
                    u->getNode()->setVisible(dist <= units[i]->getLineOfSight() || isInLineOfSight(compUnPos, units[i]->getLineOfSight(), u));
                }
			}
        }
    }

    bool ActiveGameState::isInLineOfSight(Vector3 center, float radius, Unit *u) {
        bool inside = false;

        for (int i = 0; i < 4 && !inside; i++)
            if (center.getDistanceFrom(u->getCorner(i)) <= radius){
                inside = true;
				break;
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
		Quad *dragbox = (Quad*)dragboxNode->getMesh(0);
		dragbox->setSize(size);
		dragbox->updateVerts(dragbox->getMeshBase());
		dragboxNode->setPosition(Vector3(selectionBoxOrigin.x, selectionBoxOrigin.y, 0));
    }

	//TODO fix ejectable unit selection with multiple transports selected
    void ActiveGameState::issueOrder(Order::TYPE type, vector<Order::Target> targets, bool addOrder) {
		Vector3 destDir = (selectingDestOrient ? GameObjectFrameController::getSingleton()->getGameObjectFrame(0).getDirVec() : Vector3::VEC_ZERO);
		mainPlayer->issueOrder(type, destDir, targets, addOrder);
		this->targets.clear();
    }
    
    void ActiveGameState::castRayToTerrain() {
		Camera *cam = Root::getSingleton()->getCamera();
		Vector3 camPos = cam->getPosition();
		Vector3 endPos = screenToSpace(getCursorPos());

		Vector3 rayDir = (endPos - camPos).norm();
		vector<RayCaster::CollisionResult> results = RayCaster::cast(camPos, rayDir, Map::getSingleton()->getNodeParent());

		if(!results.empty()){
			GameObjectFrameController *ufCtr = GameObjectFrameController::getSingleton();
			Map *map = Map::getSingleton();

			for(Unit *u : mainPlayer->getSelectedUnits()){
				Vector3 pos = u->getPos();
				Node *nodeParent = map->getNodeParent();

				if(u->getType() == UnitType::UNDERWATER && nodeParent->getNumChildren() > 0){
					vector<RayCaster::CollisionResult> res = RayCaster::cast(Vector3(pos.x, 100, pos.z), Vector3(0, -1, 0), nodeParent->getChild(0));

					Vector3 waterBodyPos = Vector3::VEC_ZERO;
					Vector3 cellSize = map->getCellSize();

					for(int i = 1; i < nodeParent->getNumChildren(); i++){
						Vector3 wpos = nodeParent->getChild(i)->getPosition();
						
						if(fabs(wpos.x - pos.x) < .5 * cellSize.x && fabs(wpos.z - pos.z) < .5 * cellSize.z){
							waterBodyPos = wpos;
							break;
						}
					}

					results[0].pos.y = res[0].pos.y + depth * (waterBodyPos.y - res[0].pos.y);
				}
			}

			targets.push_back(Order::Target(nullptr, results[0].pos));
		}
    }

    
    void ActiveGameState::onAction(int bind, bool isPressed) {
		GameObjectFrameController *ufCtr = GameObjectFrameController::getSingleton();

        switch((Bind)bind){
			case Bind::DRAG_BOX: 
				leftMouseClicked = isPressed;

				if(isPressed){
					clickPoint = getCursorPos();
					lastLeftMouseClicked = getTime();
				}
				else{
                    if (mainPlayer->getNumSelectedUnits() > 0){
						if(selectingPatrolPoints){
                    		castRayToTerrain();

							if(!(targets.empty() || selectingPatrolPoints)){
								Order::TYPE type = Order::TYPE::MOVE;
							
								if(controlPressed || (targets[0].unit && targets[0].unit->getPlayer()->getTeam() != mainPlayer->getTeam()))
									type = Order::TYPE::ATTACK;
								else if(ufCtr->isPlacingFrames() && !selectingDestOrient)
									type = Order::TYPE::BUILD;
							
								issueOrder(type, targets, shiftPressed);
							}
						}
						else{
							bool canSelect = canSelectHoveredOnGameObj();
							bool ownGameObj = (gameObjHoveredOn && gameObjHoveredOn->getPlayer()->getTeam() == mainPlayer->getTeam());

							if(gameObjHoveredOn && (gameObjHoveredOn->getType() == GameObject::Type::UNIT && ((Unit*)gameObjHoveredOn)->getNumGarrisonSlots() > 0 && ownGameObj))
								issueOrder(Order::TYPE::GARRISON, vector<Order::Target>{Order::Target((Unit*)gameObjHoveredOn, gameObjHoveredOn->getPos())}, shiftPressed);
							else if(gameObjHoveredOn && (controlPressed || !ownGameObj))
								issueOrder(Order::TYPE::ATTACK, vector<Order::Target>{Order::Target((Unit*)gameObjHoveredOn, gameObjHoveredOn->getPos())}, shiftPressed);
							else if(controlPressed && !gameObjHoveredOn){
                    			castRayToTerrain();
								issueOrder(Order::TYPE::ATTACK, targets, shiftPressed);
							}
							else if(ufCtr->isPlacingFrames() && !selectingDestOrient){
                    			castRayToTerrain();

								GameObjectFrame gmObjFr = ufCtr->getGameObjectFrame(0);
								Unit *buildStruct = GameObjectFactory::createUnit(mainPlayer, gmObjFr.getId(), gmObjFr.getPos(), gmObjFr.getRot());
								mainPlayer->addUnit(buildStruct);
								targets[0].unit = buildStruct;

								issueOrder(Order::TYPE::BUILD, targets, shiftPressed);
							}
							else if(canSelect){
								if(!shiftPressed)
									deselectUnits();

								mainPlayer->selectUnit((Unit*)gameObjHoveredOn);
							}
							else if(!canSelect){
                    			castRayToTerrain();
								issueOrder(Order::TYPE::MOVE, targets, shiftPressed);
							}
						}
					}
					else{
						if(canSelectHoveredOnGameObj()){
							if(!shiftPressed)
								deselectUnits();

							mainPlayer->selectUnit((Unit*)gameObjHoveredOn);
						}
					}

					selectingDestOrient = false;
					isSelectionBox = false;

					ufCtr->setPlacingFrames(false);
					ufCtr->setRotatingFrames(false);
					ufCtr->removeGameObjectFrames();
                }

                break;
			case Bind::DESELECT:
				if(ufCtr->isPlacingFrames()) ufCtr->setRotatingFrames(isPressed);
                if (!isPressed) deselectUnits();
                break;
			case Bind::EJECT_GARRISON:
				if(isPressed) issueOrder(Order::TYPE::EJECT, vector<Order::Target>{}, shiftPressed);
				break;
			case Bind::TOGGLE_SUB:
                break;
			case Bind::ZOOM_IN:
                if (zooms > -NUM_MAX_ZOOMS) {
					Camera *cam = Root::getSingleton()->getCamera();
                    cam->setPosition(cam->getPosition() + cam->getDirection().norm() * .5f);
                    zooms--;
                }
                break;
			case Bind::ZOOM_OUT:
                if (zooms < NUM_MAX_ZOOMS) {
					Camera *cam = Root::getSingleton()->getCamera();
                    cam->setPosition(cam->getPosition() - (cam->getDirection().norm().norm() * .5f));
                    zooms++;
                }
                break;
			case Bind::LOOK_AROUND:
				if(!ufCtr->isPlacingFrames())
					CameraController::getSingleton()->setLookingAround(isPressed);
                break;
			case Bind::HALT: 
				if(isPressed)
            		for (Unit *u : mainPlayer->getSelectedUnits())
                    	u->halt();
                break;
				//TODO reimplement submarine diving mechanic
			case Bind::LEFT_CONTROL:
                controlPressed = isPressed;
				if(isPressed) depth -= 0.05;
                break;
			case Bind::LEFT_SHIFT:
			{
        		shiftPressed = isPressed;

				GameObjectFrameController *ufCtr = GameObjectFrameController::getSingleton();
				ufCtr->setPaintSelecting(shiftPressed);

				if(isPressed){
					depth += 0.05;

					Vector3 startPos = Root::getSingleton()->getCamera()->getPosition();
					vector<RayCaster::CollisionResult> results = RayCaster::cast(startPos, (screenToSpace(getCursorPos()) - startPos).norm(), Map::getSingleton()->getNodeParent()->getChild(0));

					if(!results.empty())
						ufCtr->setPaintSelectRowStart(results[0].pos);
				}
			}
                break;
			case Bind::SELECT_PATROL_POINTS: 
				if(isPressed && mainPlayer->getNumSelectedUnits() > 0){
					targets.push_back(Order::Target(nullptr, mainPlayer->getSelectedUnit(0)->getPos()));
			   		selectingPatrolPoints = isPressed;
				}

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

                    if(controlPressed)
                        unitGroups[group] = mainPlayer->getSelectedUnits();
                    else{
                        if(!shiftPressed)
                            mainPlayer->selectUnits(unitGroups[group]);
                        else
                            for(Unit *u : unitGroups[group])
                                mainPlayer->selectUnit(u);
                    }
                }

                break;
			case Bind::DESELECT_STRUCTURE:
				ufCtr->removeGameObjectFrames();
				ufCtr->setPlacingFrames(false);
				break;
        }
    }

    void ActiveGameState::onAnalog(int bind, float strength) {
		CameraController *camCtr = CameraController::getSingleton();
		GameObjectFrameController *ufCtr = GameObjectFrameController::getSingleton();

		switch((Bind)bind){
			case Bind::LOOK_UP: 
			case Bind::LOOK_DOWN: 
				if(camCtr->isLookingAround()) {
					Vector3 dirProj = Root::getSingleton()->getCamera()->getDirection();
					dirProj = Vector3(dirProj.x, 0, dirProj.z).norm();
					CameraController::getSingleton()->orientCamera(Vector3(0, 1, 0).cross(dirProj), strength);
				}

				break;
			case Bind::LOOK_LEFT: 
			case Bind::LOOK_RIGHT: 
				if(camCtr->isLookingAround())
					CameraController::getSingleton()->orientCamera(Vector3(0, 1, 0), strength);
				else if(ufCtr->isRotatingFrames())
					ufCtr->rotateGameObjectFrames(100 * strength);

				break;
		}
	}
}
