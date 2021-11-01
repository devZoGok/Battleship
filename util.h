#pragma once
#ifndef UTIL_BATTLESHIP_H
#define UTIL_BATTLESHIP_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

namespace battleship{
    typedef unsigned char u8;
    typedef unsigned short u16;
    typedef unsigned int u32;
    typedef unsigned long long u64;
    typedef char s8;
    typedef short s16;
    typedef int s32;
    typedef long long s64;
    
    class GameManager;
    class GuiAppState;
    
    void makeTitlescreenButtons(GuiAppState*);
		/*
    bool isWithinRect(irr::core::vector3df,irr::core::vector3df,irr::core::vector3df,irr::core::vector3df);
    bool isWithinCuboid(irr::core::vector3df,irr::core::vector3df,irr::core::vector3df,irr::core::vector3df,irr::core::vector3df);
		*/
}

#endif
