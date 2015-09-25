#ifndef INC_KEYBOARD_H
#define INC_KEYBOARD_H

#include "type.h"
#include "keydef.h"

void HandleNormalKeyDown(uint8 key, int32 x, int32 y);
void HandleSpecialKeyDown(int32 key, int32 x, int32 y);
void HandleNormalKeyUp(uint8 key, int32 x, int32 y);
void HandleSpecialKeyUp(int32 key, int32 x, int32 y);

void HandleNormalKeyPressed();
void HandleSpecialKeyPressed();

bool GetNormalKeyState(uint8 key);
bool GetSpecialKeyState(uint8 key);

uint8 GetLastNormalKey();
uint8 GetLastSpecialKey();

bool CheckKeyModifier(uint8 key);
bool CheckModifierFlag(uint8 flag);

#endif