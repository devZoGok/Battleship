#include <root.h>
#include <quaternion.h>
#include <model.h>
#include <material.h>
#include <quad.h>
#include <text.h>
#include <ray.h>

#include <stateManager.h>

#include <algorithm>
#include <ctype.h>
#include <stdlib.h>
#include <map>

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
		using namespace gameBase;

    const int size = 50;
    
    ActiveGameState::ActiveGameState(GuiAppState *guiState, vector<Player*> players, int playerId) : AbstractAppState(
						AppStateType::ACTIVE_STATE,
					 	configData::calcSumBinds(AppStateType::ACTIVE_STATE, true),
					 	configData::calcSumBinds(AppStateType::ACTIVE_STATE, false),
					 	GameManager::getSingleton()->getPath() + "Scripts/options.lua"){
        this->guiState = guiState;
        this->players = players;

				GameManager *gm = GameManager::getSingleton();
				Root *root = Root::getSingleton();
        Camera *cam = root->getCamera();

        this->playerId = playerId;
        mainPlayer = players[playerId];
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

		Text *t = new Text(GameManager::getSingleton()->getPath() + "Fonts/batang.ttf", L"depth: ");
		Material *tm = new Material(root->getLibPath() + "text");
		tm->addBoolUniform("texturingEnabled", false);
		tm->addVec4Uniform("diffuseColor", Vector4(1, 1, 1, 1));
		t->setMaterial(tm);
		textNode = new Node(Vector3(0, 100, 0));
		textNode->addText(t);

		depth = 1;
    }

    ActiveGameState::~ActiveGameState() {
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

				if(!lookingAround)
					updateCameraPosition();
    }

		void ActiveGameState::deselectUnits(){
				while(!selectedUnits.empty()){
						selectedUnits[0]->toggleSelection(false);
						selectedUnits.pop_back();
				}
		}

    void ActiveGameState::renderUnits() {
				vector<Unit*> units;

        for (Player *p : players)
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

                        selectedUnits.push_back(u);
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
                    u->getNode()->setVisible(dist <= units[i]->getLineOfSight() || isInLineOfSight(compUnPos, units[i]->getLineOfSight(), u) ? true : false);
                }
						}
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

    void ActiveGameState::issueOrder(Order::TYPE type, bool addOrder) {
        Order o;
        o.type = type;
        o.targets = targets;

		Vector3 color;

      	switch(type){
      	    case Order::TYPE::MOVE:
				color = Vector3::VEC_J;
      	        break;
      	    case Order::TYPE::ATTACK:
				color = Vector3::VEC_I;
      	        break;
      	    case Order::TYPE::PATROL:
				color = Vector3::VEC_K;
				break;
      	    case Order::TYPE::LAUNCH:
				color = Vector3(1, 1, 0);
      	        break;
      	}

		targets.push_back(Order::Target());

		if(o.type == Order::TYPE::PATROL)
			for(int i = targets.size() - 2; i >= 0; i--)
				targets[i + 1] = targets[i];

		LineRenderer *lineRenderer = LineRenderer::getSingleton();
		Map *map = Map::getSingleton();

        for (int i = 0; i < selectedUnits.size(); ++i) {
			Unit *u = selectedUnits[i];

			if(u->getType() == UnitType::UNDERWATER && o.type == Order::TYPE::MOVE){
				for(int j = 1; j < map->getNumTerrainObjects(); j++){
					if(map->isPointWithinTerrainObject(u->getPos(), j) && map->isPointWithinTerrainObject(o.targets[i].pos, j)){
						vector<Ray::CollisionResult> res;
						Ray::retrieveCollisions(u->getPos(), Vector3(0, -1, 0), map->getTerrainObject(0).node->getChild(0), res);
						Ray::sortResults(res);
						o.targets[i].pos.y = res[0].pos.y + depth * (map->getTerrainObject(j).pos.y - res[0].pos.y);
						break;
					}
				}
			}

			lineRenderer->addLine(u->getPos(), o.targets[i].pos, color);
			vector<LineRenderer::Line> lines = lineRenderer->getLines();
			o.line = lines[lines.size() - 1];

            if (type != Order::TYPE::LAUNCH || (u->getId() == 4 || u->getId() == 5)) {
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
			targets.push_back(Order::Target((it != unitData.end() ? unitData[node] : nullptr), results[0].pos));
		}
    }
    
    void ActiveGameState::onAction(int bind, bool isPressed) {
				GameManager *gm = GameManager::getSingleton();
        InGameAppState *inGameState = (InGameAppState*) gm->getStateManager()->getAppStateByType((int)AppStateType::IN_GAME_STATE);

        switch((Bind)bind){
						case Bind::DRAG_BOX: 
                isSelectionBox = isPressed;

                if (isPressed) {
										clickPoint = getCursorPos();

                    if (!selectedUnits.empty()){
                    	addTarget();

											if(!(targets.empty() || selectingPatrolPoints)){
												Order::TYPE type = Order::TYPE::MOVE;

												if(controlPressed || (targets[0].unit && targets[0].unit->getPlayer()->getSide() != mainPlayer->getSide()))
														type = Order::TYPE::ATTACK;

												issueOrder(type, shiftPressed);
											}
										}
                }

                break;
						case Bind::DESELECT:
                if (!isPressed)
										deselectUnits();

                break;
						case Bind::TOGGLE_SUB:
                if (isPressed) {
                    for (Unit *u : selectedUnits) {
                        if (u->getPlayer() == mainPlayer && u->getUnitClass() == UnitClass::SUBMARINE) {
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
								Map *map = Map::getSingleton();
								Node *node = map->getNodeParent()->getChild(1);
								bool visible = node->isVisible();
								node->setVisible(!visible);
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
