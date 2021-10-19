#include"Explosion.h"
#include"util.h"
#include"InGameAppState.h"
#include<SFML/Audio.hpp>

using namespace irr::io;
using namespace irr::core;
using namespace irr::video;
using namespace irr::scene;
using namespace game::util;
using namespace sf;

namespace game{
    namespace content{
        void detonate(InGameAppState *inGameState,vector3df pos,vector3df dir){
            SoundBuffer *sfxBuffer=new SoundBuffer();
            Sound *sfx=nullptr;
            path p=PATH+"Sounds/Explosions/explosion0"+stringw(rand()%4)+".ogg";
            if(sfxBuffer->loadFromFile(p.c_str())){
                sfx=new Sound(*sfxBuffer);
                sfx->play();
            }
            Fx fx;
            fx.initTime=getTime();
            fx.time=2500;
            fx.sfx=sfx;
            inGameState->addFx(fx);
        }

        void detonateDepthCharge(core::InGameAppState *inGameState, irr::core::vector3df){
            SoundBuffer *sfxBuffer=new SoundBuffer();
            Sound *sfx=nullptr;
            path p=PATH+"Sounds/Destroyers/depthCharge.ogg";
            if(sfxBuffer->loadFromFile(p.c_str())){
                sfx=new Sound(*sfxBuffer);
                sfx->play();
            }
            Fx fx;
            fx.initTime=getTime();
            fx.time=2000;
            fx.sfx=sfx;
            inGameState->addFx(fx);
        }

        void detonateTorpedo(core::InGameAppState *inGameState, irr::core::vector3df){
            SoundBuffer *sfxBuffer=new SoundBuffer();
            Sound *sfx=nullptr;
            path p=PATH+"Sounds/Submarines/torpedo.ogg";
            if(sfxBuffer->loadFromFile(p.c_str())){
                sfx=new Sound(*sfxBuffer);
                sfx->play();
            }
            Fx fx;
            fx.initTime=getTime();
            fx.time=250;
            fx.sfx=sfx;
            inGameState->addFx(fx);
        }
    }
}
