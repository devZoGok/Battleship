#pragma once
#ifndef KEY_H
#define KEY_H

namespace battleship{
	struct Mapping{
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
				LOOK_UP,
				LOOK_DOWN,
				LOOK_LEFT,
				LOOK_RIGHT,
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
		enum BindType{KEYBOARD, MOUSE_KEY, MOUSE_AXIS, JOYSTICK_KEY, JOYSTICK_AXIS};
		enum AuxTriggers{MOUSE_AXIS_LEFT = 310, MOUSE_AXIS_RIGHT = 311, MOUSE_AXIS_UP = 312, MOUSE_AXIS_DOWN = 313};
	
		Bind bind;
		BindType type;
		int trigger;
		bool action, pressed = false;
	};
}

#endif
