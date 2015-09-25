#include "opengl.h"
#include "type.h"
#include "utility.h"
#include "keyboard.h"
#include "keyboardhandler.h"

bool NormalKeyStates[256];
bool SpecialKeyStates[256];

uint8 lastNormalKey = 0;
uint8 lastSpecialKey = 0;

void HandleNormalKeyDown(uint8 key, int32 x, int32 y)
{
	NormalKeyStates[key] = KEY_STATE_DOWN;
	lastNormalKey = key;

	for (uint32 i = 0; i < KEYBOARD_HANDLER_AMOUNT; i++)
	{
		if ((KeyboardHandlerTable[i].state == KEY_STATE_DOWN) && (IntervalCheck(key, KeyboardHandlerTable[i].keyRangeMin, KeyboardHandlerTable[i].keyRangeMax) == true))
		{
			if (CheckModifierFlag(KeyboardHandlerTable[i].modFlags) == true)
			{
				if (KeyboardHandlerTable[i].handler() == true)
				{
					break;
				}
			}
		}
	}
}

void HandleSpecialKeyDown(int32 key, int32 x, int32 y)
{
	SpecialKeyStates[(uint8)key] = KEY_STATE_DOWN;
	lastSpecialKey = key;

	NormalKeyStates[key] = KEY_STATE_DOWN;

	for (uint32 i = 0; i < SPECIAL_HANDLER_AMOUNT; i++)
	{
		if ((SpecialHandlerTable[i].state == KEY_STATE_DOWN) && (IntervalCheck(key, SpecialHandlerTable[i].keyRangeMin, KeyboardHandlerTable[i].keyRangeMax) == true))
		{
			if (CheckModifierFlag(SpecialHandlerTable[i].modFlags) == true)
			{
				if (SpecialHandlerTable[i].handler() == true)
				{
					break;
				}
			}
		}
	}
}

void HandleNormalKeyUp(uint8 key, int32 x, int32 y)
{
	NormalKeyStates[key] = KEY_STATE_UP;
	switch (key)
	{
		default:
		{
			break;
		}
	}
}

void HandleSpecialKeyUp(int32 key, int32 x, int32 y)
{
	SpecialKeyStates[(uint8)key] = KEY_STATE_UP;
	switch (key)
	{
		default:
		{
			break;
		}
	}
};

void HandleNormalKeyPressed()
{

}

void HandleSpecialKeyPressed()
{
	if (GetSpecialKeyState(KEY_UP) == true)
	{
		//MoveCamera(1);
	}
	if (GetSpecialKeyState(KEY_DOWN) == true)
	{
		//MoveCamera(-1);
	}
}


bool GetNormalKeyState(uint8 key)
{
	return NormalKeyStates[key];
}

bool GetSpecialKeyState(uint8 key)
{
	return SpecialKeyStates[key];
}

uint8 GetLastNormalKey()
{
	return lastNormalKey;
}

uint8 GetLastSpecialKey()
{
	return lastSpecialKey;
}

bool CheckKeyModifier(uint8 key)
{
	if ((key < KEY_SHIFT) || (key > KEY_ALT))
	{
		return false;
	}
	if (CheckFlag(glutGetModifiers(), 1 << (key - 0x10)) == true)
	{
		return true;
	}

	return false;
}

bool CheckModifierFlag(uint8 flag)
{
	if (flag > 7)		//1 + 2 + 4
	{
		return false;
	}
	if (flag != NULL)
	{
		for (uint8 i = 0; i < 3; i++)
		{
			if ((CheckFlag(flag, (1 << i)) == true) && (CheckKeyModifier(i + 16) == false))
			{
				return false;
			}
		}
	}
	else
	{
		for (uint8 i = 0; i < 3; i++)
		{
			if (CheckKeyModifier(i + 16) == true)
			{
				return false;
			}
		}
	}
	return true;
}

