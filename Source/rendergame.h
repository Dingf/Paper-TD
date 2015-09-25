#ifndef INC_RENDER_GAMEPLAY_H
#define INC_RENDER_GAMEPLAY_H

#include "type.h"
#include "point.h"
#include "sprite.h"
#include "font.h"

void GameDisplayInit();
void GameDisplayUnload();

bool LoadGUIScripts();

//void MakeIconDisplayList(uint8 icon);
//void DrawGameIcon(uint8 icon, uint32 size, Point2D position);

void DrawGameplay();

//void InitCursor(const char * filename, const Point2D& hotSpot, const char * properties = NULL);
//void SetMouseOffset(Float offset);
//void DrawCursor();
//void UnloadCursor();

#endif