#include <SFML/Audio.hpp>

#include "explosion.h"
#include "util.h"
#include "inGameAppState.h"

using namespace std;
using namespace vb01;

namespace battleship{
		using namespace configData;

    void detonate(InGameAppState *inGameState, Vector3 pos, Vector3 dir){
				sf::SoundBuffer *sfxBuffer = new sf::SoundBuffer();
				sf::Sound *sfx = nullptr;
        string  p = PATH + "Sounds/Explosions/explosion0" + to_string(rand() % 4) + ".ogg";

        if(sfxBuffer->loadFromFile(p.c_str())){
            sfx = new sf::Sound(*sfxBuffer);
            sfx->play();
        }

        Fx fx;
        fx.initTime = getTime();
        fx.time = 2500;
        fx.sfx = sfx;
        inGameState->addFx(fx);
    }

    void detonateDepthCharge(InGameAppState *inGameState, Vector3){
				sf::SoundBuffer *sfxBuffer = new sf::SoundBuffer();
				sf::Sound *sfx = nullptr;
        string p = PATH + "Sounds/Destroyers/depthCharge.ogg";

        if(sfxBuffer->loadFromFile(p.c_str())){
            sfx = new sf::Sound(*sfxBuffer);
            sfx->play();
        }
				
        Fx fx;
        fx.initTime = getTime();
        fx.time = 2000;
        fx.sfx = sfx;
        inGameState->addFx(fx);
    }

    void detonateTorpedo(InGameAppState *inGameState, Vector3){
				sf::SoundBuffer *sfxBuffer = new sf::SoundBuffer();
				sf::Sound *sfx = nullptr;
        string p = PATH + "Sounds/Submarines/torpedo.ogg";

        if(sfxBuffer->loadFromFile(p.c_str())){
            sfx = new sf::Sound(*sfxBuffer);
            sfx->play();
        }

        Fx fx;
        fx.initTime = getTime();
        fx.time = 250;
        fx.sfx = sfx;
        inGameState->addFx(fx);
    }
}
