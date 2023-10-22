#include <root.h>
#include <quaternion.h>
#include <model.h>
#include <material.h>
#include <quad.h>
#include <text.h>
#include <ray.h>

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
					 	GameManager::getSingleton()->getPath() + "Scripts/options.lua"){
        this->guiState = guiState;
        this->playerId = playerId;

        mainPlayer = Game::getSingleton()->getPlayer(playerId);

		initDragbox();

		refinedsText = initText(Vector2(0, 100));
		wealthText = initText(Vector2(0, 200));
		researchText = initText(Vector2(0, 300));
		textNode = initText(Vector2(0, 400));
		depth = 1;
    }

    ActiveGameState::~ActiveGameState() {
    }

	void ActiveGameState::initDragbox(){
		Root *root = Root::getSingleton();

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

	Node* ActiveGameState::initText(Vector2 pos){
		Material *tm = new Material(Root::getSingleton()->getLibPath() + "text");
		tm->addBoolUniform("texturingEnabled", false);
		tm->addVec4Uniform("diffuseColor", Vector4(1, 1, 1, 1));

		Text *t = new Text(GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", L"");
		t->setMaterial(tm);

		Node *node = new Node(Vector3(pos.x, pos.y, 0));
		node->addText(t);

		return node;
	}

    void ActiveGameState::onAttached() {
        AbstractAppState::onAttached();
		Root::getSingleton()->getGuiNode()->attachChild(textNode);
		Root::getSingleton()->getGuiNode()->attachChild(refinedsText);
		Root::getSingleton()->getGuiNode()->attachChild(wealthText);
		Root::getSingleton()->getGuiNode()->attachChild(researchText);
    }

    void ActiveGameState::onDettached() {
		AbstractAppState::onDettached();
		Root::getSingleton()->getGuiNode()->dettachChild(textNode);
		Root::getSingleton()->getGuiNode()->dettachChild(refinedsText);
		Root::getSingleton()->getGuiNode()->dettachChild(wealthText);
		Root::getSingleton()->getGuiNode()->dettachChild(researchText);
    }

    void ActiveGameState::update() {
		textNode->getText(0)->setText(L"Depth: " + to_wstring(depth));
		refinedsText->getText(0)->setText(L"Refineds: " + to_wstring(mainPlayer->getRefineds()));
		wealthText->getText(0)->setText(L"Wealth: " + to_wstring(mainPlayer->getWealth()));
		researchText->getText(0)->setText(L"Research: " + to_wstring(mainPlayer->getResearch()));

        if (isSelectionBox)
			updateSelectionBox();

		dragboxNode->setVisible(isSelectionBox);

        renderUnits();

		CameraController *camCtr = CameraController::getSingleton();

		if(!camCtr->isLookingAround())
			camCtr->updateCameraPosition();

		GameObjectFrameController *ufCtr = GameObjectFrameController::getSingleton();

		if(ufCtr->isPlacingFrames())
			ufCtr->update();
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

        for (Unit *u : units) {
            if (u->getPlayer() == mainPlayer){
				Vector3 selectionBoxOrigin = dragboxNode->getPosition();
				Vector3 selectionBoxEnd = selectionBoxOrigin + dragbox->getSize();
            	Vector2 pos = u->getScreenPos();

                if(isSelectionBox && 
                    pos.x >= selectionBoxOrigin.x && pos.x <= selectionBoxEnd.x &&
                    pos.y >= selectionBoxOrigin.y && pos.y <= selectionBoxEnd.y){

                    if(!u->isSelected()){
                        if(!shiftPressed)
							deselectUnits();

                        mainPlayer->selectUnit(u);
                		u->toggleSelection(true);

						if(engineersSelected()){
							ConcreteGuiManager *guiManager = ConcreteGuiManager::getSingleton();
							guiManager->readLuaScreenScript("engineerCommands.lua");
						}
                    }
                }

                u->getNode()->setVisible(true);
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
		dragbox->setSize(size);
		dragboxNode->setPosition(Vector3(selectionBoxOrigin.x, selectionBoxOrigin.y, 0));
    }

	//TODO fix ejectable unit selection with multiple transports selected
    void ActiveGameState::issueOrder(Order::TYPE type, bool addOrder) {
		Vector3 color;

      	switch(type){
      	    case Order::TYPE::MOVE:
				color = Vector3::VEC_J;
      	        break;
      	    case Order::TYPE::ATTACK:
      	    case Order::TYPE::LAUNCH:
				color = Vector3::VEC_I;
      	        break;
      	    case Order::TYPE::PATROL:
      	    case Order::TYPE::GARRISON:
      	    case Order::TYPE::EJECT:
				color = Vector3::VEC_K;
				break;
      	    case Order::TYPE::BUILD:
				color = Vector3(1, 1, 0);
      	        break;
      	}

		if(type == Order::TYPE::PATROL){
			for(int i = targets.size() - 2; i >= 0; i--)
				targets[i + 1] = targets[i];
		}
		else if(type == Order::TYPE::EJECT){
			vector<Unit*> units = mainPlayer->getSelectedUnits();

			for(Unit *u : units){
				const vector<Unit::GarrisonSlot> &garrisonSlots = u->getGarrisonSlots();

				for(Unit::GarrisonSlot slot : garrisonSlots)
					if(slot.vehicle)
						targets.push_back(Order::Target((Unit*)slot.vehicle));
			}
		}
		else
			targets.push_back(Order::Target());

		LineRenderer *lineRenderer = LineRenderer::getSingleton();
		Order order;

        for (int i = 0; i < mainPlayer->getNumSelectedUnits(); ++i) {
			Unit *u = mainPlayer->getSelectedUnit(i);
			lineRenderer->addLine(u->getPos(), targets[i].pos, color);
			vector<LineRenderer::Line> lines = lineRenderer->getLines();
			LineRenderer::Line line = lines[lines.size() - 1];
        	order = Order(type, line, targets);

            if (type != Order::TYPE::LAUNCH) {
				if(type == Order::TYPE::PATROL){
					Vector3 p = u->getPos();
					targets[0] = Order::Target(nullptr, p);
				}

                if (addOrder)
                    u->addOrder(order);
                else
                    u->setOrder(order);
            }
        }

		targets.clear();
    }
    
    void ActiveGameState::addTarget() {
		Camera *cam = Root::getSingleton()->getCamera();
		Vector3 camPos = cam->getPosition();
		Vector3 endPos = screenToSpace(getCursorPos());

		vector<Ray::CollisionResult> results;
		Vector3 rayDir = (endPos - camPos).norm();
		Ray::retrieveCollisions(camPos, rayDir, Map::getSingleton()->getNodeParent(), results);
		std::map<Node*, Unit*> unitData;

		Ray::sortResults(results);

		if(!results.empty()){
			vector<Node*> ancestors = results[0].mesh->getNode()->getAncestors();
			Node *node = ancestors[ancestors.size() - 2];
			std::map<Node*, Unit*>::iterator it = unitData.find(node);

			Unit *unit = nullptr;
			GameObjectFrameController *ufCtr = GameObjectFrameController::getSingleton();

			if(ufCtr->isPlacingFrames()){
				GameObjectFrame goFr = ufCtr->getGameObjectFrame(0);
				unit = new Structure(mainPlayer, goFr.getId(), goFr.getModel()->getPosition(), goFr.getModel()->getOrientation());
				mainPlayer->addUnit(unit);
			}

			Map *map = Map::getSingleton();

			for(Unit *u : mainPlayer->getSelectedUnits()){
				Vector3 pos = u->getPos();
				Node *nodeParent = map->getNodeParent();

				if(u->getType() == UnitType::UNDERWATER && nodeParent->getNumChildren() > 0){
					vector<Ray::CollisionResult> res;
					Ray::retrieveCollisions(Vector3(pos.x, 100, pos.z), Vector3(0, -1, 0), nodeParent->getChild(0), res);
					Ray::sortResults(res);

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

			targets.push_back(Order::Target((it != unitData.end() ? unitData[node] : unit), results[0].pos));
		}
    }

	bool ActiveGameState::engineersSelected(){
		for(Unit *u : mainPlayer->getSelectedUnits())
			if(u->getUnitClass() == UnitClass::ENGINEER)
				return true;

		return false;
	}
    
    void ActiveGameState::onAction(int bind, bool isPressed) {
		GameManager *gm = GameManager::getSingleton();
        InGameAppState *inGameState = (InGameAppState*) gm->getStateManager()->getAppStateByType((int)AppStateType::IN_GAME_STATE);
		GameObjectFrameController *ufCtr = GameObjectFrameController::getSingleton();

        switch((Bind)bind){
			case Bind::DRAG_BOX: 
                isSelectionBox = isPressed;

                if (isPressed) {
					clickPoint = getCursorPos();

                    if (mainPlayer->getNumSelectedUnits() > 0){
                    	addTarget();

						if(!(targets.empty() || selectingPatrolPoints)){
							Order::TYPE type = Order::TYPE::MOVE;
						
							if(controlPressed || (targets[0].unit && targets[0].unit->getPlayer()->getSide() != mainPlayer->getSide()))
								type = Order::TYPE::ATTACK;
							else if(ufCtr->isPlacingFrames()){
								type = Order::TYPE::BUILD;
								ufCtr->setPlacingFrames(false);
								ufCtr->setRotatingFrames(false);
								ufCtr->removeGameObjectFrames();
							}
						
							issueOrder(type, shiftPressed);
						}
					}
                }

                break;
			case Bind::DESELECT:
				if(ufCtr->isPlacingFrames()) ufCtr->setRotatingFrames(isPressed);
                if (!isPressed) deselectUnits();
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
					vector<Ray::CollisionResult> results;
					Ray::retrieveCollisions(startPos, (screenToSpace(getCursorPos()) - startPos).norm(), Map::getSingleton()->getNodeParent()->getChild(0), results);
					Ray::sortResults(results);

					if(!results.empty())
						ufCtr->setPaintSelectRowStart(results[0].pos);
				}
			}
                break;
			case Bind::SELECT_PATROL_POINTS: 
                selectingPatrolPoints = isPressed;

				if(!selectingPatrolPoints)
					issueOrder(Order::TYPE::PATROL, shiftPressed);
								
                break;
			case Bind::LAUNCH:{ 
				if(isPressed){
					Node *node = Map::getSingleton()->getNodeParent()->getChild(1);
					node->setVisible(!node->isVisible());
				}

                selectingGuidedMissileTarget = isPressed;
                break;
										  }
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
				else if(ufCtr->isPlacingFrames() && ufCtr->isRotatingFrames())
					ufCtr->rotateGameObjectFrames(100 * strength);

				break;
		}
	}
}
