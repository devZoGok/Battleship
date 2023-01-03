#include <root.h>
#include <quaternion.h>
#include <model.h>
#include <material.h>
#include <quad.h>
#include <text.h>
#include <ray.h>

#include <stateManager.h>
#include <luaManager.h>

#include <algorithm>
#include <ctype.h>
#include <stdlib.h>
#include <map>

#include "activeGameState.h"
#include "inGameAppState.h"
#include "defConfigs.h"
#include "structure.h"
#include "util.h"
#include "tooltip.h"
#include "unitFrameController.h"
#include "cameraController.h"

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
        mainPlayer = Map::getSingleton()->getPlayer(playerId);

		initDragbox();
		initDepthText();
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

	void ActiveGameState::initDepthText(){
		Root *root = Root::getSingleton();
		Text *t = new Text(GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", L"depth: ");

		Material *tm = new Material(root->getLibPath() + "text");
		tm->addBoolUniform("texturingEnabled", false);
		tm->addVec4Uniform("diffuseColor", Vector4(1, 1, 1, 1));
		t->setMaterial(tm);

		textNode = new Node(Vector3(0, 100, 0));
		textNode->addText(t);

		depth = 1;
	}

    void ActiveGameState::onAttached() {
        AbstractAppState::onAttached();
		Root::getSingleton()->getGuiNode()->attachChild(textNode);
    }

    void ActiveGameState::onDettached() {
		AbstractAppState::onDettached();
		Root::getSingleton()->getGuiNode()->attachChild(textNode);
    }

    void ActiveGameState::update() {
		textNode->getText(0)->setText(L"Depth: " + to_wstring(depth));

        if (isSelectionBox)
			updateSelectionBox();

		dragboxNode->setVisible(isSelectionBox);

        renderUnits();

		CameraController *camCtr = CameraController::getSingleton();

		if(!camCtr->isLookingAround())
			camCtr->updateCameraPosition();

		UnitFrameController *ufCtr = UnitFrameController::getSingleton();

		if(ufCtr->isPlacingFrames())
			ufCtr->update();
    }

	void ActiveGameState::deselectUnits(){
		mainPlayer->deselectUnits();
		UnitFrameController *ufCtr = UnitFrameController::getSingleton();
		ufCtr->removeUnitFrames();
		ufCtr->setPlacingFrames(false);
	}

	//TODO fix fog of war for hostile units
    void ActiveGameState::renderUnits() {
		vector<Unit*> units;

        for (Player *p : Map::getSingleton()->getPlayers())
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
				color = Vector3::VEC_K;
				break;
      	    case Order::TYPE::BUILD:
				color = Vector3(1, 1, 0);
      	        break;
      	}

        Order o;
        o.type = type;
        o.targets = targets;
		targets.push_back(Order::Target());

		if(o.type == Order::TYPE::PATROL)
			for(int i = targets.size() - 2; i >= 0; i--)
				targets[i + 1] = targets[i];

		LineRenderer *lineRenderer = LineRenderer::getSingleton();

        for (int i = 0; i < mainPlayer->getNumSelectedUnits(); ++i) {
			Unit *u = mainPlayer->getSelectedUnit(i);
			lineRenderer->addLine(u->getPos(), o.targets[i].pos, color);
			vector<LineRenderer::Line> lines = lineRenderer->getLines();
			o.line = lines[lines.size() - 1];

            if (type != Order::TYPE::LAUNCH) {
				if(type == Order::TYPE::PATROL){
					Vector3 p = u->getPos();
					targets[0] = Order::Target(nullptr, p);
				}

                if (addOrder)
                    u->addOrder(o);
                else
                    u->setOrder(o);
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
			UnitFrameController *ufCtr = UnitFrameController::getSingleton();

			if(ufCtr->isPlacingFrames()){
				unit = new Structure(mainPlayer, ufCtr->getUnitFrame(0).id, results[0].pos, ufCtr->getUnitFrame(0).model->getOrientation());
				mainPlayer->addUnit(unit);
			}

			Map *map = Map::getSingleton();

			for(Unit *u : mainPlayer->getSelectedUnits())
				if(u->getType() == UnitType::UNDERWATER){
					for(int i = 1; i < map->getNumTerrainObjects(); i++){
						if(map->isPointWithinTerrainObject(u->getPos(), i) && map->isPointWithinTerrainObject(results[0].pos, i)){
							vector<Ray::CollisionResult> res;
							Vector3 pos = u->getPos();
							Ray::retrieveCollisions(Vector3(pos.x, map->getTerrainObject(0).size.y, pos.z), Vector3(0, -1, 0), map->getTerrainObject(0).node->getChild(0), res);
							Ray::sortResults(res);
							results[0].pos.y = res[0].pos.y + depth * (map->getTerrainObject(i).pos.y - res[0].pos.y);
							break;
						}
					}
				}

			targets.push_back(Order::Target((it != unitData.end() ? unitData[node] : unit), results[0].pos));
		}
    }
    
    void ActiveGameState::onAction(int bind, bool isPressed) {
		GameManager *gm = GameManager::getSingleton();
        InGameAppState *inGameState = (InGameAppState*) gm->getStateManager()->getAppStateByType((int)AppStateType::IN_GAME_STATE);
		UnitFrameController *ufCtr = UnitFrameController::getSingleton();

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
								ufCtr->removeUnitFrames();
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
                for (Unit *u : mainPlayer->getSelectedUnits())
                    u->halt();
                break;
				//TODO reimplement submarine diving mechanic
			case Bind::LEFT_CONTROL:
                controlPressed = isPressed;
				if(isPressed) depth -= 0.05;
                break;
			case Bind::LEFT_SHIFT:
                shiftPressed = isPressed;
				if(isPressed) depth += 0.05;
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
			case Bind::SELECT_STRUCTURE:
			{
				if(isPressed){
					bool engineersSelected = false;

					for(Unit *u : mainPlayer->getSelectedUnits())
						if(u->getUnitClass() == UnitClass::ENGINEER){
							engineersSelected = true;
							break;
						}

					if(!engineersSelected)
						break;

					//TODO fix the magic value
					int id = 3;
					LuaManager *lm = LuaManager::getSingleton();
					string modelPath = lm->getStringFromTable("basePath", vector<Index>{Index(id + 1)}) + lm->getStringFromTable("meshPath", vector<Index>{Index(id + 1)});
					ufCtr->addUnitFrame(UnitFrameController::UnitFrame(modelPath, id, (int)UnitType::LAND));

					ufCtr->setPlacingFrames(true);
				}

				break;
			}
			case Bind::DESELECT_STRUCTURE:
				ufCtr->removeUnitFrames();
				ufCtr->setPlacingFrames(false);
				break;
        }
    }

    void ActiveGameState::onAnalog(int bind, float strength) {
		CameraController *camCtr = CameraController::getSingleton();
		UnitFrameController *ufCtr = UnitFrameController::getSingleton();

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
					ufCtr->rotateUnitFrames(100 * strength);

				break;
		}
	}
}
