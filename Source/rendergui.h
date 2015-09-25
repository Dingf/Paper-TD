#ifndef INC_RENDER_GUI_H
#define INC_RENDER_GUI_H

#include "type.h"
#include "point.h"
#include "sprite.h"
#include "spell.h"

void GUIDisplayInit();
void GUIDisplayUnload();

void CursorDisplayInit();
void CursorDisplayUnload();
void InfoboxDisplayInit();
void InfoboxDisplayUnload();
void TooltipDisplayInit();
void TooltipDisplayUnload();

char * GetHotkeyedText(const char * text, uint8 hotkey);
char * GetHotkeyedText(const Spell * spell);

void DrawGameIcon(uint8 icon, const Point2D& position, uint32 size = 48, const ColorARGB& color = White);
void DrawBorderLine(uint32 length, const Point2D& position);
//void DrawBackground();
void DrawInkbar();
void DrawWaveInfo();
void DrawCursor();
void DrawInfobox();
void DrawTooltip(const char * text, const Font * font);
void DrawGUI();

#endif