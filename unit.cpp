#include <model.h>
#include <quad.h>
#include <material.h>
#include <texture.h>
#include <ray.h>

#include <stateManager.h>
#include <luaManager.h>

#include <glm.hpp>
#include <ext.hpp>

#include "unit.h"
#include "util.h"
#include "inGameAppState.h"
#include "defConfigs.h"
#include "pathfinder.h"

using namespace glm;
using namespace vb01;
using namespace gameBase;
using namespace std;

namespace battleship{
    Unit::Unit(Player *player, int id, Vector3 pos, Quaternion rot) {
        this->id = id;
        this->player = player;

		initProperties();
		initModel();
		initSound();
		initUnitStats();
        placeUnit(pos);
		orientUnit(rot);
    }

	//TODO complete implementation
    Unit::~Unit() {
    }

	void Unit::initProperties(){
		LuaManager *lm = LuaManager::getSingleton();
        health = lm->getIntFromTable("health", vector<Index>{Index(id + 1)});
		maxHealth = health;

        range = lm->getFloatFromTable("range", vector<Index>{Index(id + 1)});
        lineOfSight = lm->getFloatFromTable("lineOfSight", vector<Index>{Index(id + 1)});
        unitClass = (UnitClass)lm->getIntFromTable("unitClass", vector<Index>{Index(id + 1)});
		type = (UnitType)lm->getIntFromTable("unitType", vector<Index>{Index(id + 1)});

		string cornerInd = "unitCornerPoints";

		for(int i = 0; i < 8; i++){
			float x = lm->getFloatFromTable(cornerInd, vector<Index>{Index(id + 1), Index(i + 1), Index("x")});
			float y = lm->getFloatFromTable(cornerInd, vector<Index>{Index(id + 1), Index(i + 1), Index("y")});
			float z = lm->getFloatFromTable(cornerInd, vector<Index>{Index(id + 1), Index(i + 1), Index("z")});
			corners[i] = Vector3(x, y, z);
		}

        width = corners[0].x - corners[1].x;
        height = corners[4].y - corners[0].y;
        length = corners[3].z - corners[0].z;
	}
	
	void Unit::destroyModel(){
		Root::getSingleton()->getRootNode()->dettachChild(model);
		delete model;
	}

	void Unit::initModel(){
		LuaManager *lm = LuaManager::getSingleton();
		string basePath = lm->getStringFromTable("basePath", vector<Index>{Index(id + 1)});
		string meshPath = lm->getStringFromTable("meshPath", vector<Index>{Index(id + 1)});

		model = new Model(basePath + meshPath);
		Root *root = Root::getSingleton();
		string libPath = root->getLibPath();

		Material *mat = new Material(libPath + "texture");
		string f[]{configData::DEFAULT_TEXTURE};
        Texture *diffuseTexture = new Texture(f, 1, false);
		mat->addBoolUniform("texturingEnabled", true);
		mat->addBoolUniform("lightingEnabled", false);
		mat->addTexUniform("textures[0]", diffuseTexture, true);

		model->setMaterial(mat);
		root->getRootNode()->attachChild(model);
	}

	void Unit::destroySound(){
		selectionSfx->stop();
		delete selectionSfx;
		delete selectionSfxBuffer;
	}

	void Unit::initSound(){
		LuaManager *lm = LuaManager::getSingleton();
		string name = lm->getStringFromTable("name", vector<Index>{Index(id + 1)});
        selectionSfxBuffer = new sf::SoundBuffer();
        string p = GameManager::getSingleton()->getPath() + "Sounds/" + name + "s/selection.ogg";

        if(selectionSfxBuffer->loadFromFile(p.c_str()))
            selectionSfx = new sf::Sound(*selectionSfxBuffer);
	}

	void Unit::reinitUnit(){
		destroySound();
		destroyModel();
		initModel();
		initSound();
		initProperties();
		placeUnit(pos);
		orientUnit(rot);
	}

	void Unit::initUnitStats(){
		Root *root = Root::getSingleton();

		hpBackground = new Quad(Vector3(lenHpBar, 10, 0), false);
		string libPath = root->getLibPath();
		Material *hpBackgroundMat = new Material(libPath + "gui");
		hpBackgroundMat->addBoolUniform("texturingEnabled", false);
		hpBackgroundMat->addVec4Uniform("diffuseColor", Vector4(0, 0, 0, 1));
		hpBackground->setMaterial(hpBackgroundMat);
		hpBackgroundNode = new Node();
		hpBackgroundNode->attachMesh(hpBackground);
		Node *guiNode = root->getGuiNode();
		guiNode->attachChild(hpBackgroundNode);

		hpForeground = new Quad(Vector3(lenHpBar, 10, 0), false);
		Material *hpForegroundMat = new Material(libPath + "gui");
		hpForegroundMat->addBoolUniform("texturingEnabled", false);
		hpForegroundMat->addVec4Uniform("diffuseColor", Vector4(0, 1, 0, 1));
		hpForeground->setMaterial(hpForegroundMat);
		hpForegroundNode = new Node();
		hpForegroundNode->attachMesh(hpForeground);
		guiNode->attachChild(hpForegroundNode);
	}

    void Unit::update() {
		leftVec = model->getGlobalAxis(0);
		upVec = model->getGlobalAxis(1);
		dirVec = model->getGlobalAxis(2);

        if (!orders.empty()){
			LineRenderer *lineRenderer = LineRenderer::getSingleton();
			lineRenderer->toggleVisibility(orders[0].line.id, selected && canDisplayOrderLine());
			lineRenderer->changeLineField(orders[0].line.id, pos, LineRenderer::START);
        }

        executeOrders();

		screenPos = spaceToScreen(pos);
		hpBackgroundNode->setVisible(selected);
		hpForegroundNode->setVisible(selected);

        if (selected)
            displayUnitStats();

        if (health <= 0) 
            blowUp();
    }

    void Unit::blowUp(){
        working = false;
    }

    void Unit::displayUnitStats() {
		float shiftedX = screenPos.x - 0.5 * lenHpBar;
		hpForegroundNode->setPosition(Vector3(shiftedX, screenPos.y, 0));
		hpForeground->setSize(Vector3(health / maxHealth * lenHpBar, 10, 0));
		hpBackgroundNode->setPosition(Vector3(shiftedX, screenPos.y, .1));
    }

	//TODO factor out non mutual order methods
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

    void Unit::placeUnit(Vector3 p) {
        model->setPosition(p);
        pos = p;
    }

    void Unit::orientUnit(Quaternion rotQuat){
		model->setOrientation(rotQuat);
		rot = rotQuat;
    }
    
    std::vector<Projectile*> Unit::getProjectiles(){
        std::vector<Projectile*> projectiles;
        InGameAppState *inGameState = ((InGameAppState*)GameManager::getSingleton()->getStateManager()->getAppStateByType((int)AppStateType::IN_GAME_STATE));

        for(Projectile *p: inGameState->getProjectiles())
            if(p->getUnit() == this)
                projectiles.push_back(p);

        return projectiles;
    }

	void Unit::addOrder(Order order){
		orders.push_back(order);
	}
    
    void Unit::removeOrder(int id) {
		LineRenderer::getSingleton()->removeLine(orders[id].line.id);
        orders.erase(orders.begin() + id);
    }

    void Unit::toggleSelection(bool selection) {
        selected = selection;

        if(selection && selectionSfx)
            selectionSfx->play();

        orderLineDispTime = getTime();
    }

    void Unit::addProjectile(Projectile *p) {
		StateManager *stateManager = GameManager::getSingleton()->getStateManager();
		((InGameAppState*)stateManager->getAppStateByType((int)AppStateType::IN_GAME_STATE))->addProjectile(p);
	}
}
