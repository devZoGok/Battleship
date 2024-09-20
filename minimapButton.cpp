#include "minimapButton.h"
#include "activeGameState.h"
#include "game.h"
#include "map.h"

#include <stateManager.h>

#include <node.h>
#include <mesh.h>
#include <material.h>
#include <imageAsset.h>
#include <assetManager.h>

#include <algorithm>
#include <vector>

namespace battleship{
	using namespace vb01;
	using namespace std;

	MinimapButton::MinimapButton(Vector3 pos, Vector2 size, string ip) : Button(pos, size, "minimap", "", -1, true, ip){
		content = new Node();
		rectNode->attachChild(content);
	}

	MinimapButton::~MinimapButton(){
	}

	void MinimapButton::update(){
		Button::update();

		Material *mat = rectNode->getMesh(0)->getMaterial();
		Texture *tex = ((Material::TextureUniform*)mat->getUniform("diffuseMap"))->value;
		ImageAsset *asset = (ImageAsset*)AssetManager::getSingleton()->getAsset(imagePath);
		int width = asset->width, height = asset->height, numChannels = asset->numChannels;

		Vector3 mapSize = Map::getSingleton()->getMapSize();
		vector<pair<Unit*, Vector2>> unitMinimapPos;

		for(Player *pl : Game::getSingleton()->getPlayers())
			for(Unit *un : pl->getUnits()){
				Vector2 coords = Vector2(
					int(un->getPos().x / mapSize.x * width),
					int(un->getPos().z / mapSize.z * height)
				);

				unitMinimapPos.push_back(make_pair(un, coords));
			}

		ActiveGameState *activeState = (ActiveGameState*)GameManager::getSingleton()->getStateManager()->getAppStateByType(AppStateType::ACTIVE_STATE);
		Player *mainPlayer = activeState->getPlayer();
		vector<Unit*> units = mainPlayer->getUnits();

		int size = width * height * numChannels;
		int pxId = 0, numIter = 0;

		u8 *oldImageData = activeState->getOldMinimapImage();

		for(u8 *p = asset->image; p != asset->image + size; p += numChannels, pxId += numChannels, numIter++){
			Vector2 coords = Vector2(
				-(numIter % asset->width - .5 * asset->width),
				numIter / asset->width - .5 * asset->height
			);
			float losFactor = .6, pxCol[3]{
				losFactor * oldImageData[pxId + 0],
				losFactor * oldImageData[pxId + 1],
				losFactor * oldImageData[pxId + 2]
			};

			for(pair<Unit*, Vector2> unitPair : unitMinimapPos){
				if(unitPair.first->getPlayer() == mainPlayer && unitPair.second == coords){
					Vector3 plCol = mainPlayer->getColor();
					pxCol[0] = plCol.x * 255;
					pxCol[1] = plCol.y * 255;
					pxCol[2] = plCol.z * 255;
					break;
				}

				float minimapLos = unitPair.first->getLineOfSight() / mapSize.x * width;

				if(unitPair.first->getPlayer() == mainPlayer && coords.getDistanceFrom(unitPair.second) < minimapLos){
					bool foreignUnit = false;
					Player *pl = nullptr;

					for(pair<Unit*, Vector2> up : unitMinimapPos)
						if(up.first->getPlayer() != mainPlayer && up.second == coords){
							foreignUnit = true;
							pl = up.first->getPlayer();
							break;
						}

					if(foreignUnit){
						Vector3 plCol = pl->getColor();
						pxCol[0] = plCol.x * 255;
						pxCol[1] = plCol.y * 255;
						pxCol[2] = plCol.z * 255;
					}
					else{
						pxCol[0] = oldImageData[pxId + 0];
						pxCol[1] = oldImageData[pxId + 1];
						pxCol[2] = oldImageData[pxId + 2];
					}

					break;
				}
			}

			*p = pxCol[0];
			*(p + 1) = pxCol[1];
			*(p + 2) = pxCol[2];
		}

		tex->loadImageData(asset);
	}

	void MinimapButton::onClick(){
		Vector2 clickPos = getCursorPos();
		float posXRatio = 1. - (clickPos.x - pos.x) / size.x;
		float posYRatio = 1. - (clickPos.y - pos.y) / size.y;

		Camera *cam = Root::getSingleton()->getCamera();
		Vector3 mapSize = Map::getSingleton()->getMapSize();
		cam->setPosition(Vector3(mapSize.x * (-.5 + posXRatio), cam->getPosition().y, mapSize.z * (-.5 + posYRatio)));
	}
}
