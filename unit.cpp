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
#include "gameObjectFactory.h"
#include "activeGameState.h"
#include "gameManager.h"
#include "projectile.h"
#include "defConfigs.h"
#include "pathfinder.h"
#include "ability.h"

using namespace glm;
using namespace vb01;
using namespace gameBase;
using namespace std;

namespace battleship{
	Unit::Weapon::Weapon(Unit *u, sol::table weaponTable) : 
		unit(u), 
		type((Weapon::Type)weaponTable["type"]), 
		rateOfFire(weaponTable["rateOfFire"]),
		damage(weaponTable["damage"].get_or(0))
	{
		maxRange = weaponTable["maxRange"];
		fireSfxBuffer = new sf::SoundBuffer();
		string sfxPath = weaponTable["fireSfx"];
		fireSfx = GameObject::prepareSfx(fireSfxBuffer, sfxPath);

		initProjectileData(weaponTable);

		initFx(weaponTable, "fireVfx");
		initFx(weaponTable, "hitVfx");
	}

	void Unit::Weapon::initProjectileData(sol::table weaponTable){
		string projTableKey = "projectile";
		sol::optional<sol::table> proj = weaponTable[projTableKey];

		if(proj != sol::nullopt){
			projId = weaponTable[projTableKey]["id"];
			sol::table projTable = generateView()["projectiles"];
			ProjectileClass pc = (ProjectileClass)projTable[projId + 1]["projectileClass"];

			if(pc == ProjectileClass::CRUISE_MISSILE){
				minRange = 0;
				float rotAngle = projTable[projId + 1]["rotAngle"];
				float speed = projTable[projId + 1]["speed"];
				float base = PI / 2, alpha = 0;

				while(base - alpha > .001){
					minRange += speed * sin(alpha);
					alpha += (base - alpha > rotAngle ? rotAngle : base - alpha);
				}

				minRange *= 2;
			}

			sol::table posTable = weaponTable[projTableKey]["pos"];
			projPos = Vector3(posTable["x"], posTable["y"], posTable["z"]);
			sol::table rotTable = weaponTable[projTableKey]["rot"];
			projRot = Quaternion(rotTable["w"], rotTable["x"], rotTable["y"], rotTable["z"]);

		}
	}

	vector<Node*> Unit::Weapon::initFx(sol::table weaponTable, string vfxKey){
		vector<Node*> vfxNodes;

		sol::table vfxTbl = weaponTable[vfxKey];
		int numVfx = vfxTbl.size();

		for(int i = 0; i < numVfx; i++){
			Material *mat = new Material(Root::getSingleton()->getLibPath() + "texture");

			if((std::optional<string>)vfxTbl["texture"] != sol::nullopt){
				string p[]{vfxTbl["texture"]};
				Texture *tex = new Texture(p, 1, false);
				mat->addBoolUniform("texturingEnabled", true);
				mat->addTexUniform("diffuseMap[0]", tex, false);
			}
			else{
				sol::table colorTable = vfxTbl["color"];
				mat->addVec4Uniform("diffuseColor", Vector4(colorTable["x"], colorTable["y"], colorTable["z"], colorTable["a"]));
				mat->addBoolUniform("texturingEnabled", false);
			}

			Model *flashModel = new Model((string)vfxTbl["model"]);
			flashModel->setMaterial(mat);
			unit->getModel()->attachChild(flashModel);

			sol::table posTable = vfxTbl["pos"][i + 1], rotTable = vfxTbl["rot"][i + 1];
			flashModel->setPosition(Vector3(posTable["x"], posTable["y"], posTable["z"]));
			flashModel->setOrientation(Quaternion(rotTable["w"], rotTable["x"], rotTable["y"], rotTable["z"]));
		}

		return vfxNodes;
	}

	Unit::Weapon::~Weapon(){
		if(fireSfx){
			fireSfx->stop();
			delete fireSfx;
			delete fireSfxBuffer;
		}
	}

	void Unit::Weapon::fire(Order order){
		if(!canFire()) return;

		fireSfx->play();
		Unit *targetUnit = order.targets[0].unit;

		if(targetUnit && projId == -1){
			targetUnit->takeDamage(damage);
			unit->updateGameStats(targetUnit);
		}
		else if(projId != -1){
			Vector3 leftVec = unit->getLeftVec();
			Vector3 upVec = unit->getUpVec();
			Vector3 dirVec = unit->getDirVec();
			Vector3 p = unit->getPos() + leftVec * projPos.x + upVec * projPos.y + dirVec * projPos.z;
			Quaternion r = unit->getRot() * projRot;
			unit->getPlayer()->addProjectile(GameObjectFactory::createProjectile(unit, projId, p, r));
		}

		lastFireTime = getTime();
	}

    Unit::Unit(Player *player, int id, Vector3 pos, Quaternion rot, State st) : GameObject(GameObject::Type::UNIT, id, player, pos, rot), state(st){
		selectable = true;

		initProperties();
		initModel();
		initHitbox();
		initSound();
		initWeapons();
        placeAt(pos);
		orientAt(rot);

		Vector2 size = Vector2(lenHpBar, 10);
		hpBackgroundNode = createBar(Vector2::VEC_ZERO, size, Vector4(0, 0, 0, 1));
		hpForegroundNode = createBar(Vector2::VEC_ZERO, size, Vector4(0, 1, 0, 1));
    }

    Unit::~Unit() {
		removeBar(hpBackgroundNode);
		removeBar(hpForegroundNode);
		destroySound();
		destroyHitbox();
		destroyModel();
		destroyWeapons();
    }

	void Unit::initProperties(){
		GameObject::initProperties();

		sol::state_view SOL_LUA_VIEW = generateView();
		string objType = GameObject::getGameObjTableName();
		sol::table unitTable = SOL_LUA_VIEW[objType][id + 1];

		Game *game = Game::getSingleton();
		vector<int> currTechs = player->getTechnologies();

		string name = unitTable["name"];
        health = unitTable["health"]; health += game->calcAbilFromTech(Ability::Type::HEALTH, currTechs, (int)GameObject::type, id);
        vehicle = unitTable["isVehicle"];
		maxHealth = health;

        lineOfSight = unitTable["lineOfSight"]; lineOfSight += game->calcAbilFromTech(Ability::Type::LINE_OF_SIGHT, currTechs, (int)GameObject::type, id);
        unitClass = (UnitClass)unitTable["unitClass"];
		type = (UnitType)unitTable["unitType"];

		string gsk = "guiScreen";
		sol::optional<string> nameOpt = unitTable[gsk];
		guiScreen = (nameOpt != sol::nullopt ? (string)unitTable[gsk] : "");

		string tblName = "garrisonCapacity";
		sol::optional<sol::table> gc = unitTable[tblName];

		if(gc != sol::nullopt){
			string varName = "numGarrisonSlots";
			SOL_LUA_VIEW.script(varName + " = #" + objType + "[" + to_string(id + 1) + "]." + tblName);
			int numGarrisonSlots = SOL_LUA_VIEW[varName];

			for(int i = 0; i < numGarrisonSlots; i++){
				Vector2 size = 10 * Vector2::VEC_IJ;
				Vector2 pos = Vector2(1.5 * size.x * i, 20);
				Node *bg = createBar(pos, size, Vector4(0, 0, 0, 1));
				Node *fg = createBar(pos, size, Vector4(0, 1, 0, 1));
				int category = unitTable[tblName][i + 1];
				garrisonSlots.push_back(GarrisonSlot(bg, fg, pos, category));
			}
		}

		tblName = "armor";
		sol::optional<sol::table> at = unitTable[tblName];

		if(at != sol::nullopt){
			string varName = "numArmorTypes"; 
			SOL_LUA_VIEW.script(varName + " = #" + objType + "[" + to_string(id + 1) + "]." + tblName);
			int numArmorTypes = SOL_LUA_VIEW[varName];

			for(int i = 0; i < numArmorTypes; i++){
				Armor arm = (Armor)unitTable[tblName][i + 1];
				armorTypes.push_back(arm);
			}
		}

		tblName = "buildableUnits";
		sol::optional<sol::table> bu = unitTable[tblName];

		if(bu != sol::nullopt){
			SOL_LUA_VIEW.script("numBuildableUnits = #units[" + to_string(id + 1) + "]." + tblName);
			int numBuildableUnits = SOL_LUA_VIEW["numBuildableUnits"];

			for(int i = 0; i < numBuildableUnits; i++){
				sol::table buTable = unitTable[tblName][i + 1];
				buildableUnits.push_back(BuildableUnit(buTable["id"], game->isUnitUnlocked(currTechs, id) | (bool)buTable["buildable"]));
			}
		}
	}

	void Unit::initWeapons(){
		sol::state_view SOL_STATE_VIEW = generateView();
		string objType = GameObject::getGameObjTableName();
		sol::table unitTable = SOL_STATE_VIEW[objType][id + 1];

		string tblName = "weapons";
		sol::optional<sol::table> wt = unitTable[tblName];

		if(wt != sol::nullopt){
			string varName = "numWeapons";
			SOL_STATE_VIEW.script(varName + " = #" + objType + "[" + to_string(id + 1) + "]." + tblName);
			int numWeapons = SOL_STATE_VIEW[varName];

			for(int i = 0; i < numWeapons; i++)
				weapons.push_back(new Weapon(this, unitTable[tblName][i + 1]));
		}
	}

	void Unit::destroyWeapons(){
		for(Weapon *weapon : weapons)
			delete weapon;

		weapons.clear();
	}
	
	void Unit::destroySound(){
		selectionSfx->stop();
		delete selectionSfx;
		delete selectionSfxBuffer;
	}

	//TODO factor out initSound()
	void Unit::initSound(){
		GameObject::initSound();
		selectionSfxBuffer = new sf::SoundBuffer();
		string sfxPath = generateView()[getGameObjTableName()][id + 1]["selectionSfx"];
		selectionSfx = GameObject::prepareSfx(selectionSfxBuffer, sfxPath);
	}

	void Unit::reinit(){
		destroySound();
		destroyHitbox();
		destroyModel();
		destroyWeapons();

		initProperties();

		initModel();
		initHitbox();
		initSound();
		initWeapons();

		GameObject::reinit();
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

	void Unit::launch(Order order){
		for(Weapon *weapon : weapons){
			Vector3 targPos = Vector3(order.targets[0].pos.x, pos.y, order.targets[0].pos.z);

			if(weapon->getProjectileId() == 0 && weapon->getMinRange() < targPos.getDistanceFrom(pos)){
				weapon->fire(order);
				break;
			}
		}

		removeOrder(0);
	}

	float Unit::calculateRotation(Vector3 dir, float angle, float maxTurnAngle){
		float rotSpeed = (maxTurnAngle > angle ? angle : maxTurnAngle); 

		if(isTargetToTheRight(dir, leftVec))
			rotSpeed *= -1;

		return rotSpeed;
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

	void Unit::targetUnitsAutomatically(){
		vector<Player*> players = Game::getSingleton()->getPlayers();
		vector<Unit*> units;

		for(Player *pl : players)
			if(!(pl == player || pl->getTeam() == player->getTeam())){
				vector<Unit*> un = pl->getUnits();
				units.insert(units.end(), un.begin(), un.end());
			}

		if(orders.empty())
			for(Unit *unit : units)
				if(unit->getPos().getDistanceFrom(pos) < lineOfSight){
					setOrder(Order(Order::TYPE::ATTACK, vector<Order::Target>{Order::Target(unit)}, Vector3::VEC_ZERO, -1, false));
					break;
				}
	}

    void Unit::update() {
		GameObject::update();

		ActiveGameState *activeState = (ActiveGameState*)GameManager::getSingleton()->getStateManager()->getAppStateByType(AppStateType::ACTIVE_STATE);
		Player *mainPlayer = (activeState ? activeState->getPlayer() : nullptr);

		if(state != State::HOLD_FIRE)
			targetUnitsAutomatically();

		vector<Player*> selectingPlayers = getSelectingPlayers();
		bool mainPlayerSelecting = (find(selectingPlayers.begin(), selectingPlayers.end(), mainPlayer) != selectingPlayers.end());
		bool mainPlayerOwner = (player == mainPlayer);
		bool renderSelectables = (mainPlayerSelecting && mainPlayerOwner);

		renderOrderLine(renderSelectables);
        executeOrders();

		displayUnitStats(hpForegroundNode, hpBackgroundNode, health, maxHealth, mainPlayerSelecting);

		for(GarrisonSlot &slot : garrisonSlots)
			displayUnitStats(slot.foreground, slot.background, (int)((bool)slot.vehicle), (int)true, renderSelectables, slot.offset);

        if (health <= DEATH_HP){
			Game::getSingleton()->explode(pos, 0, 0, deathSfx);
			remove = true;
		}
    }

    void Unit::displayUnitStats(Node *foreground, Node *background, int currVal, int maxVal, bool render, Vector2 offset) {
		foreground->setVisible(render);
		background->setVisible(render);

		if(render){
			Vector3 offset3d = Vector3(offset.x, offset.y, 0);

			Quad *bgQuad = (Quad*)background->getMesh(0);
			Vector3 size = bgQuad->getSize();
			float shiftedX = screenPos.x - 0.5 * size.x;
			background->setPosition(Vector3(shiftedX, screenPos.y, 0) + offset3d);

			Quad *fgQuad = (Quad*)foreground->getMesh(0);
			fgQuad->setSize(Vector3((float)currVal / maxVal * size.x, size.y, 0));
			fgQuad->updateVerts(fgQuad->getMeshBase());
			foreground->setPosition(Vector3(shiftedX, screenPos.y, .01) + offset3d);
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
                case Order::TYPE::SUPPLY:
                    supply(order);
                    break;
                case Order::TYPE::HACK:
                    hack(order);
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
