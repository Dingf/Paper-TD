#ifndef INC_RENDER_H
#define INC_RENDER_H

#include "type.h"

void DisplayInit();
void DisplayUnload();

void RenderFrame();
void ResizeWindow(int32 width, int32 height);

extern int32 screenWidth;
extern int32 screenHeight;

#endif