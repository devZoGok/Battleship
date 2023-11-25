#include "fx.h"

#include <particleEmitter.h>
#include <node.h>

#include <SFML/Audio.hpp>

namespace battleship{
	using namespace vb01;

    void Fx::activate(){
		if(sfx)
			sfx->play();

		if(peNode){

		}
    }
}


