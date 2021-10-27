#pragma once
#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <irrlicht.h>
#include <chrono>

namespace game{
    typedef unsigned char u8;
    typedef unsigned short u16;
    typedef unsigned int u32;
    typedef unsigned long long u64;
    typedef char s8;
    typedef short s16;
    typedef int s32;
    typedef long long s64;
    
    namespace core{
        class GameManager;
        class GuiAppState;
    }
    
    namespace util{
        irr::scene::ISceneNode* castRay(irr::scene::ISceneManager*,irr::core::vector3df,irr::core::vector3df);
        std::vector<irr::core::stringw> readFile(std::string,int=0,int=-1);
        void writeFile(std::string,std::vector<std::string>);
        void makeTitlescreenButtons(core::GameManager*,core::GuiAppState*);
        double getAngleBetween(irr::core::vector3df, irr::core::vector3df);
        bool isWithinRect(irr::core::vector3df,irr::core::vector3df,irr::core::vector3df,irr::core::vector3df);
        bool isWithinCuboid(irr::core::vector3df,irr::core::vector3df,irr::core::vector3df,irr::core::vector3df,irr::core::vector3df);
        inline s64 getTime(){return (s64)(std::chrono::system_clock::now().time_since_epoch()/std::chrono::milliseconds(1));}
    }
}

#endif
