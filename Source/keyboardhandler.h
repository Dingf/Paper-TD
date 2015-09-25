#ifndef INC_KEYBOARD_HANDLER_H
#define INC_KEYBOARD_HANDLER_H

#include "type.h"
#include "keyboardhandlerdef.h"

struct KeyboardHandler
{
	uint32 GUID;
	uint8 modFlags;
	uint8 keyRangeMin;
	uint8 keyRangeMax;
	bool state;
	bool (*handler)();
};

extern KeyboardHandler KeyboardHandlerTable[KEYBOARD_HANDLER_AMOUNT];
extern KeyboardHandler SpecialHandlerTable[SPECIAL_HANDLER_AMOUNT];

#endif