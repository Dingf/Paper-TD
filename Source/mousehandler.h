#ifndef INC_MOUSE_HANDLER_H
#define INC_MOUSE_HANDLER_H

#include "type.h"
#include "rect.h"
#include "mousehandlerdef.h"

struct MouseHandler
{
	uint32 GUID;
	uint8 modFlags;
	uint8 button;
	bool state;
	Rect hotSpot;
	bool (*handler)(uint8 button, const Point2D& position);
};

extern MouseHandler * MouseHandlerTable;

#endif