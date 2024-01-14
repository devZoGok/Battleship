#include <model.h>
#include <quad.h>
#include <material.h>
#include <particleEmitter.h>
#include <texture.h>

#include <stateManager.h>
#include <solUtil.h>

#include <glm.hpp>
#include <ext.hpp>

#include "unit.h"
#include "util.h"
#include "game.h"
#include "vehicle.h"
#include "activeGameState.h"
#include "defConfigs.h"
#include "pathfinder.h"

using namespace glm;
using namespace vb01;
using namespace gameBase;
using namespace std;

namespace battleship{
    Unit::Unit(Player *player, int id, Vector3 pos, Quaternion rot) : GameObject(GameObject::Type::UNIT, id, player, pos, rot){
        this->id = id;
        this->player = player;
		selectable = true;

		init();

		Vector2 size = Vector2(lenHpBar, 10);
		hpBackgroundNode = createBar(Vector2::VEC_ZERO, size, Vector4(0, 0, 0, 1));
		hpForegroundNode = createBar(Vector2::VEC_ZERO, size, Vector4(0, 1, 0, 1));
    }

    Unit::~Unit() {
		removeBar(hpBackgroundNode);
		removeBar(hpForegroundNode);
		destroySound();
		destroyModel();
    }

	void Unit::init(){
		initProperties();
		initModel();
		initSound();
        placeAt(pos);
		orientAt(rot);
	}

	void Unit::initProperties(){
		GameObject::initProperties();

		sol::state_view SOL_LUA_VIEW = generateView();
		sol::table SOL_LUA_STATE = SOL_LUA_VIEW[GameObject::getGameObjTableName()];
		string name = SOL_LUA_STATE["name"][id + 1];
        health = SOL_LUA_STATE["health"][id + 1];
		maxHealth = health;

		rateOfFire = SOL_LUA_STATE["rateOfFire"][id + 1];
        range = SOL_LUA_STATE["range"][id + 1];
        damage = SOL_LUA_STATE["damage"][id + 1];
        lineOfSight = SOL_LUA_STATE["lineOfSight"][id + 1];
        unitClass = (UnitClass)SOL_LUA_STATE["unitClass"][id + 1];
		type = (UnitType)SOL_LUA_STATE["unitType"][id + 1];

		SOL_LUA_VIEW.script("numGarrisonSlots = #units.garrisonCapacity[" + to_string(id + 1) + "]");
		int numGarrisonSlots = SOL_LUA_VIEW["numGarrisonSlots"];

		for(int i = 0; i < numGarrisonSlots; i++){
			Vector2 size = 10 * Vector2::VEC_IJ;
			Vector2 pos = Vector2(1.5 * size.x * i, 20);
			Node *bg = createBar(pos, size, Vector4(0, 0, 0, 1));
			Node *fg = createBar(pos, size, Vector4(0, 1, 0, 1));
			int category = SOL_LUA_STATE["garrisonCapacity"][id + 1][i + 1];
			garrisonSlots.push_back(GarrisonSlot(bg, fg, pos, category));
		}

		SOL_LUA_VIEW.script("numArmorTypes = #units.armor[" + to_string(id + 1) + "]");
		int numArmorTypes = SOL_LUA_VIEW["numArmorTypes"];

		for(int i = 0; i < numArmorTypes; i++){
			Armor arm = (Armor)SOL_LUA_STATE["armor"][id + 1][i + 1];
			armorTypes.push_back(arm);
		}
	}
	
	void Unit::destroySound(){
		selectionSfx->stop();
		delete selectionSfx;
		delete selectionSfxBuffer;

		if(fireSfx){
			fireSfx->stop();
			delete fireSfx;
			delete fireSfxBuffer;
		}
	}

	void Unit::initSound(){
		GameObject::initSound();

		selectionSfxBuffer = new sf::SoundBuffer();
		selectionSfx = GameObject::prepareSfx(selectionSfxBuffer, "selectionSfx");

		fireSfxBuffer = new sf::SoundBuffer();
		fireSfx = prepareSfx(fireSfxBuffer, "fireSfx");
	}

	void Unit::reinit(){
		destroySound();
		destroyModel();
		initModel();
		initSound();
		initProperties();
        placeAt(pos);
		orientAt(rot);
	}

	bool Unit::canGarrison(Vehicle *vehicle){
		for(int i = 0; i < garrisonSlots.size(); i++)
			if(!garrisonSlots[i].vehicle && garrisonSlots[i].category >= vehicle->getGarrisonCategory())
				return true;

		return false;
	}

	Node* Unit::createBar(Vector2 pos, Vector2 size, Vector4 color){
		Root *root = Root::getSingleton();
		Material *mat = new Material(root->getLibPath() + "gui");
		mat->addBoolUniform("texturingEnabled", false);
		mat->addVec4Uniform("diffuseColor", color);

		Quad *quad = new Quad(Vector3(size.x, size.y, 0), false);
		quad->setMaterial(mat);

		Node *node = new Node(Vector3(pos.x, pos.y, 0));
		node->attachMesh(quad);
		node->setVisible(false);
		root->getGuiNode()->attachChild(node);

		return node;
	}

	void Unit::removeBar(Node *node){
		Root::getSingleton()->getGuiNode()->dettachChild(node);
		delete node;
	}

	float Unit::calculateRotation(Vector3 dir, float angle, float maxTurnAngle){
		float rotSpeed = (maxTurnAngle > angle ? angle : maxTurnAngle); 

		if(isTargetToTheRight(dir, leftVec))
			rotSpeed *= -1;

		return rotSpeed;
	}

	void Unit::fire(){
		fireSfx->play();

		Unit *targetUnit = orders[0].targets[0].unit;

		if(targetUnit){
			targetUnit->takeDamage(damage);

			if(targetUnit->getHealth() <= DEATH_HP){
				Player *targUnitPlayer = targetUnit->getPlayer();

				if(targetUnit->isVehicle()){
					player->incVehiclesDestroyed();
					targUnitPlayer->incVehiclesLost();
				}
				else{
					player->incStructuresDestroyed();
					targUnitPlayer->incStructuresLost();
				}
			}
		}

		lastFireTime = getTime();
	}

	void Unit::initUnitStats(){
	}

	void Unit::renderOrderLine(bool mainPlayerSelecting){
        if (!orders.empty() && orders[0].lineId != -1 && mainPlayerSelecting){
			bool display = canDisplayOrderLine();
			LineRenderer *lineRenderer = LineRenderer::getSingleton();
			lineRenderer->toggleVisibility(orders[0].lineId, display);

			if(display){
				lineRenderer->changeLineField(orders[0].lineId, pos, LineRenderer::START);
				Vector3 targPos = (orders[0].targets[0].unit ? orders[0].targets[0].unit->getPos() : orders[0].targets[0].pos);
				lineRenderer->changeLineField(orders[0].lineId, targPos, LineRenderer::END);
			}
        }
	}

    void Unit::update() {
		GameObject::update();

		ActiveGameState *activeState = (ActiveGameState*)GameManager::getSingleton()->getStateManager()->getAppStateByType(AppStateType::ACTIVE_STATE);
		Player *mainPlayer = (activeState ? activeState->getPlayer() : nullptr);

		vector<Player*> selectingPlayers = getSelectingPlayers();
		bool mainPlayerSelecting = (find(selectingPlayers.begin(), selectingPlayers.end(), mainPlayer) != selectingPlayers.end());
		bool mainPlayerOwner = (player == mainPlayer);
		bool renderSelectables = (mainPlayerSelecting && mainPlayerOwner);

		renderOrderLine(renderSelectables);
        executeOrders();

		displayUnitStats(hpForegroundNode, hpBackgroundNode, health, maxHealth, mainPlayerSelecting);

		for(GarrisonSlot &slot : garrisonSlots)
			displayUnitStats(slot.foreground, slot.background, (int)((bool)slot.vehicle), (int)true, renderSelectables, slot.offset);

        if (health <= DEATH_HP)
			blowUp();
    }

    void Unit::blowUp(){
		Root *root = Root::getSingleton();

		const int numFrames = 1;
		string p[numFrames];

		for(int i = 0; i < numFrames; i++)
			p[i] = GameManager::getSingleton()->getPath() + "Textures/Explosion/explosion07.png";

		Texture *tex = new Texture(p, numFrames, false);

		Material *mat = new Material(root->getLibPath() + "particle");
		mat->addTexUniform("tex", tex, true);

		ParticleEmitter *pe = new ParticleEmitter(1);
		pe->setMaterial(mat);
		pe->setLowLife(3);
		pe->setHighLife(3);
		pe->setSize(10 * Vector2::VEC_IJ);
		pe->setSpeed(0);

		Node *node = new Node(pos + Vector3(0, 2, 0));
		node->attachParticleEmitter(pe);
		node->lookAt(Vector3::VEC_J, Vector3::VEC_K);
		root->getRootNode()->attachChild(node);

		Fx fx(50, 2500, deathSfx, node);
		fx.activate();
		Game::getSingleton()->addFx(fx);

		player->removeUnit(this);
    }

    void Unit::displayUnitStats(Node *foreground, Node *background, int currVal, int maxVal, bool render, Vector2 offset) {
		foreground->setVisible(render);
		background->setVisible(render);

		if(render){
			Vector3 offset3d = Vector3(offset.x, offset.y, 0);

			Quad *bgQuad = (Quad*)background->getMesh(0);
			Vector3 size = bgQuad->getSize();
			float shiftedX = screenPos.x - 0.5 * size.x;
			background->setPosition(Vector3(shiftedX, screenPos.y, .1) + offset3d);

			Quad *fgQuad = (Quad*)foreground->getMesh(0);
			fgQuad->setSize(Vector3((float)currVal / maxVal * size.x, size.y, 0));
			fgQuad->updateVerts(fgQuad->getMeshBase());
			foreground->setPosition(Vector3(shiftedX, screenPos.y, 0) + offset3d);
		}
    }

	//TODO remove order argument from action methods
    void Unit::executeOrders() {
        if (orders.size() > 0) {
            Order order = orders[0];

            switch (order.type) {
                case Order::TYPE::ATTACK:
                    attack(order);
                    break;
                case Order::TYPE::BUILD:
                    build(order);
                    break;
                case Order::TYPE::MOVE:
                    move(order);
                    break;
				case Order::TYPE::GARRISON:
					garrison(order);
					break;
				case Order::TYPE::EJECT:
					eject(order);
					break;
                case Order::TYPE::PATROL:
                    patrol(order);
                    break;
                case Order::TYPE::LAUNCH:
                    launch(order);
                    break;
                default:
                    break;
            }
        }
    }

	void Unit::eject(Order order){
		Map *map = Map::getSingleton();
		int currCellId = map->getCellId(pos);
		vector<Map::Cell> &cells = map->getCells(); 
		int adjWaterCellId = -1, adjLandCellId = -1;

		for(Map::Edge edge : cells[currCellId].edges){
			if(adjLandCellId == -1 && cells[edge.destCellId].type == Map::Cell::LAND)
				adjLandCellId = edge.destCellId;
			if(adjWaterCellId == -1 && cells[edge.destCellId].type == Map::Cell::WATER)
				adjWaterCellId = edge.destCellId;
		}

		if(garrisonSlots.size() > 0){
			for(Order::Target targ : order.targets){
				bool exitToLandCell = (targ.unit->getType() == UnitType::LAND && adjLandCellId != -1);
				bool exitToWaterCell = ((targ.unit->getType() == UnitType::SEA_LEVEL || targ.unit->getType() == UnitType::UNDERWATER) && adjWaterCellId != -1);

				if(exitToLandCell || exitToWaterCell)
					((Vehicle*)targ.unit)->exitGarrisonable(cells[exitToLandCell ? adjLandCellId : adjWaterCellId].pos);
			}

			removeOrder(0);
		}
	}

	void Unit::attack(Order order){
		vector<Unit*> units;

		for(Player *pl : Game::getSingleton()->getPlayers()){
			vector<Unit*> un = pl->getUnits();
			units.insert(units.end(), un.begin(), un.end());
		}

		for(Order::Target &target : orders[0].targets)
			if(target.unit){
				if(find(units.begin(), units.end(), target.unit) != units.end())
					break;

				removeOrder(0);
			}
	}

    void Unit::setOrder(Order order) {
        while (!orders.empty())
			removeOrder(0);

        addOrder(order);
        orderLineDispTime = getTime();
    }

    void Unit::halt() {
        while (orders.size() > 0)
            removeOrder(orders.size() - 1);
    }

	void Unit::updateGarrison(Vehicle *garrVeh, bool entering){
		for(GarrisonSlot &slot : garrisonSlots){
			if(entering && !slot.vehicle){
				slot.vehicle = garrVeh;
				break;
			}
			else if(!entering && slot.vehicle == garrVeh){
				slot.vehicle = nullptr;
				break;
			}
		}
	}

	void Unit::addOrder(Order order){
		orders.push_back(order);
	}

	vector<Player*> Unit::getSelectingPlayers(){
		vector<Player*> players = Game::getSingleton()->getPlayers(), selectingPlayers;

		for(Player *pl : players){
			int numSelectedUnits = pl->getNumSelectedUnits();

			for(int i = 0; i < numSelectedUnits; i++)
				if(pl->getSelectedUnit(i) == this){
					selectingPlayers.push_back(pl);
					break;
				}
		}

		return selectingPlayers;
	}
    
    void Unit::removeOrder(int id) {
		if(orders[id].lineId != -1)
			LineRenderer::getSingleton()->removeLine(orders[id].lineId);

        orders.erase(orders.begin() + id);
    }

    void Unit::select() {
		ActiveGameState *activeState = (ActiveGameState*)GameManager::getSingleton()->getStateManager()->getAppStateByType(AppStateType::ACTIVE_STATE);
		Player *mainPlayer = (activeState ? activeState->getPlayer() : nullptr);

		vector<Player*> selectingPlayers = getSelectingPlayers();
		bool mainPlayerSelecting = (activeState && find(selectingPlayers.begin(), selectingPlayers.end(), mainPlayer) != selectingPlayers.end());

        if(mainPlayer == player && mainPlayerSelecting && selectionSfx)
            selectionSfx->play();

        orderLineDispTime = getTime();
    }
}
