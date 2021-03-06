#pragma once
#ifndef KEY_H
#define KEY_H

namespace game{
    namespace core{
        enum Bind{
            LEFT_CLICK,
            SCROLLING_UP,
            SCROLLING_DOWN,
            LEFT,
            RIGHT,
            DELETE_CHAR,
            CAPS_LOCK,
            SHIFT_CAPS,
            SPACE,
            PLUS,
            MINUS,
            DEVSTERISK,
            TOGGLE_MAIN_MENU,
            HALT,
            ZOOM_IN,
            ZOOM_OUT,
            LOOK_AROUND,
            DRAG_BOX,
            DESELECT,
            LEFT_CONTROL,
            LEFT_SHIFT,
            SELECT_PATROL_POINTS,
            LAUNCH,
            TOGGLE_SUB,
            INSTALL_AWM,
            INSTALL_AAM,
            GROUP_0,
            GROUP_1,
            GROUP_2,
            GROUP_3,
            GROUP_4,
            GROUP_5,
            GROUP_6,
            GROUP_7,
            GROUP_8,
            GROUP_9,
            LAST_BIND
        };
        
        struct Key {
            Key(Bind,int,bool,bool);
            Bind bind;
            int trigger;
            bool key, pressed, analog, beingUsed;
        };
    }
}

#endif
