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
#include "inGameAppState.h"
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

		sol::table SOL_LUA_STATE = generateView()[GameObject::getGameObjTableName()];
		string name = SOL_LUA_STATE["name"][id + 1];
        health = SOL_LUA_STATE["health"][id + 1];
		maxHealth = health;

		rateOfFire = SOL_LUA_STATE["rateOfFire"][id + 1];
        range = SOL_LUA_STATE["range"][id + 1];
        lineOfSight = SOL_LUA_STATE["lineOfSight"][id + 1];
        unitClass = (UnitClass)SOL_LUA_STATE["unitClass"][id + 1];
		type = (UnitType)SOL_LUA_STATE["unitType"][id + 1];
		int capacity = SOL_LUA_STATE["garrisonCapacity"][id + 1];

		for(int i = 0; i < capacity; i++){
			Vector2 size = 10 * Vector2::VEC_IJ;
			Vector2 pos = Vector2(1.5 * size.x * i, 20);
			Node *bg = createBar(pos, size, Vector4(0, 0, 0, 1));
			Node *fg = createBar(pos, size, Vector4(0, 1, 0, 1));
			garrisonSlots.push_back(GarrisonSlot(bg, fg, pos));
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

	void Unit::initUnitStats(){
	}

    void Unit::update() {
		GameObject::update();

        if (!orders.empty()){
			LineRenderer *lineRenderer = LineRenderer::getSingleton();
			lineRenderer->toggleVisibility(orders[0].line.id, selected && canDisplayOrderLine());
			lineRenderer->changeLineField(orders[0].line.id, pos, LineRenderer::START);
        }

        executeOrders();

		displayUnitStats(hpForegroundNode, hpBackgroundNode, health, maxHealth);

		for(GarrisonSlot &slot : garrisonSlots)
			displayUnitStats(slot.foreground, slot.background, (int)((bool)slot.vehicle), (int)true, slot.offset);

        if (health <= 0) 
			blowUp();
    }

    void Unit::blowUp(){
		Root *root = Root::getSingleton();

		const int numFrames = 1;
		string p[numFrames];

		for(int i = 0; i < numFrames; i++)
			p[i] = GameManager::getSingleton()->getPath() + "Textures/Explosion/explosion0" + to_string(7) + ".png";

		Texture *tex = new Texture(numFrames, p, false);

		Material *mat = new Material(root->getLibPath() + "particle");
		mat->addVec4Uniform("startColor", Vector4(1, 1, 1, 1));
		mat->addVec4Uniform("endColor", Vector4(1, 1, 0, 1));
		mat->addTexUniform("tex", tex, false);

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

		Fx fx(2500, deathSfx, node);
		fx.activate();
		Game::getSingleton()->addFx(fx);

		player->removeUnit(this);
    }

    void Unit::displayUnitStats(Node *foreground, Node *background, int currVal, int maxVal, Vector2 offset) {
		foreground->setVisible(selected);
		background->setVisible(selected);

		if(selected){
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
		if(garrisonSlots.size() > 0){
			for(Order::Target targ : order.targets)
				((Vehicle*)targ.unit)->exitGarrisonable();

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
}
