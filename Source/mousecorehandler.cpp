#include "type.h"
#include "mouse.h"
#include "mousehandler.h"

bool MouseNamespace::TakeNoAction(uint8 button, const Point2D& position)
{
	return true;
}