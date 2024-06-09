#include "fxManager.h"

#include <root.h>
#include <node.h>

#include <SFML/Audio.hpp>

namespace battleship{
	using namespace std;
	using namespace vb01;
	using namespace sf;

	FxManager::Fx::Component::Component(void *c, bool v, vb01::s64 dur, vb01::s64 ot) : comp(c), vfx(v), duration(dur), offsetTime(ot) {
		if(v)
			((vb01::Node*)c)->setVisible(false);
	}

	FxManager::Fx::Fx(std::vector<FxManager::Fx::Component> comps, bool act, bool re) : initTime(vb01::getTime()), components(comps), reuse(re){
		if(act)
			toggleComponents(true);
	}

	void FxManager::Fx::toggleComponents(bool active){
		for(Component &component : components)
			component.active = active;
	}

	static FxManager *fxManager = nullptr;

	FxManager* FxManager::getSingleton(){
		if(!fxManager)
			fxManager = new FxManager();

		return fxManager;
	}

	void FxManager::update(){
		for(int i = 0; i < fxs.size(); i++){
			s64 currTime = getTime();
			Fx *fx = fxs[i];

			for(int j = 0; j < fx->components.size(); j++){
				Fx::Component &comp = fx->components[j];

				if(comp.comp && getTime() - comp.initTime > comp.offsetTime){
					if(comp.active){
						if(comp.vfx)
							((Node*)comp.comp)->setVisible(true);
						else
							((sf::Sound*)comp.comp)->play();

						comp.active = false;
						comp.initTime = getTime();
					}

					if(!comp.active && getTime() - comp.initTime > comp.offsetTime + comp.duration){
						if(fx->reuse){
							if(comp.vfx)
								((Node*)comp.comp)->setVisible(false);
							else
								((sf::Sound*)comp.comp)->stop();
						}
						else{
							destroyFxComponent(i, j);
							fx->components.erase(fx->components.begin() + j);
							j--;
						}
					}
				}

			}

			if(fx->components.empty()){
				fxs.erase(fxs.begin() + i);
				i--;
			}
		}
	}

	void FxManager::destroyFxComponent(int fid, int cid){
		Fx *fx = fxs[fid];

		if(fx->components[cid].vfx){
			Node *vfxNode = (Node*)fx->components[cid].comp, *parNode = vfxNode->getParent();
			parNode->dettachChild(vfxNode);
			delete vfxNode;
		}
		else{
			sf::Sound *sfx = (sf::Sound*)fx->components[cid].comp;
			const sf::SoundBuffer *buffer = sfx->getBuffer();
			sfx->stop();

			delete sfx;
			delete buffer;
		}

		fx->components[cid].comp = nullptr;
	}

	FxManager::Fx* FxManager::addFx(Fx *fx){
		fxs.push_back(fx);
		return fxs[fxs.size() - 1];
	}

	//TODO remove dublicatory code
	void FxManager::removeFx(int id){
		Fx *fx = fxs[id];

		for(int j = 0; j < fx->components.size(); j++)
			destroyFxComponent(id, j);

		while(fx->components.size() > 0)
			fx->components.pop_back();

		fxs.erase(fxs.begin() + id);
	}

	/*
	void FxManager::removeFx(Fx &fx){
		for(int j = 0; j < fx.components.size(); j++)
			destroyFxComponent(id, j);

		while(fx.components.size() > 0)
			fx.components.pop_back();

		fxs.erase(fxs.begin() + id);
	}
	*/
}
