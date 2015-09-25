#ifndef INC_MOUSE_DEF_H
#define INC_MOUSE_DEF_H

#define MAX_MOUSE_BUTTONS 3

enum ButtonStates
{
	DOWN,
	UP
};

enum MouseButtons
{
	BUTTON_LEFT = 0,
	BUTTON_MIDDLE = 1,
	BUTTON_RIGHT = 2,
};

enum MouseFlags
{
	FLAG_LEFT = 1,
	FLAG_MIDDLE = 2,
	FLAG_RIGHT = 4,
};

#endif