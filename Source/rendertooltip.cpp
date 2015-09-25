#include <string>
#include "type.h"
#include "utility.h"
#include "point.h"
#include "mouse.h"
#include "sprite.h"
#include "font.h"
#include "spell.h"

Sprite Tooltip;

void TooltipDisplayInit()
{
	Tooltip.LoadSprite("Assets\\GUI\\tooltip.tga");
}

void DrawTooltip(const char * text, const Font * font)
{
	uint32 textHeight = font->CalculateTextHeight(text, 240);

	Point2D pos(ZeroFit(WorldMouse.GetX() - 256), 
		        ZeroFit(WorldMouse.GetY() - textHeight - 16));
	Point2D textPos(pos.GetX() + 8, pos.GetY() + 20);

	Tooltip.DrawSprite(Rect(0, 0, 256, 8), pos, NullPoint, NullPoint, 1.0, 0.0, White);
	pos.SetValues(pos.GetX(), pos.GetY() + 8);

	uint32 textHeightDummy = textHeight;
	while (textHeightDummy > 240)
	{
		Tooltip.DrawSprite(Rect(0, 8, 256, 248), pos, NullPoint, NullPoint, 1.0, 0.0, White);
		pos.SetValues(pos.GetX(), pos.GetY() + 240);
		textHeightDummy -= 240;
	}
	Tooltip.DrawSprite(Rect(0, 8, 256, textHeightDummy + 8), pos, NullPoint, NullPoint, 1.0, 0.0, White);
	pos.SetValues(pos.GetX(), pos.GetY() + textHeightDummy);
	Tooltip.DrawSprite(Rect(0, 248, 256, 256), pos, NullPoint, NullPoint, 1.0, 0.0, White);

	font->PrintText(text, textPos, Rect(0, 0, 240, textHeight), FORMAT_LEFT);
}

char * GetHotkeyedText(const char * text, uint8 hotkey)
{
	if (hotkey != NULL)
	{
		char * newName = new char[strlen(text)+25];

		for (uint32 i = 0; i < strlen(text) + 25; i++)
		{
			newName[i] = NULL;
		}

		char hotkeyString[2] = { hotkey, NULL };
		int32 index = StringChar(text, hotkey);

		if (index != -1)
		{
			for (uint32 k = 0; k < index; k++)
			{
				newName[k] = text[k];
			}
			strcat(newName, "&cffff0000");
			newName[index+10] = text[index];
			strcat(newName, "&cff000000");
			for (uint32 k = index; k < strlen(text) - 1; k++)
			{
				newName[k+21] = text[k+1];
			}
		}
		else
		{
			strcat(newName, text);
			strcat(newName, " [&cffff0000");
			strcat(newName, hotkeyString);
			strcat(newName, "&cff000000]");
		}

		return newName;
	}
	else
	{
		return StringCopy(text);
	}
}

char * GetHotkeyedText(const Spell * spell)
{
	char * hotkeyText = GetHotkeyedText(spell->GetName(), spell->GetInt32Value(SPELL_VAR_HOTKEY));

	if (spell->GetMode() == SPELL_MODE_LEARN)
	{
		char * newHotkeyText = new char[strlen(hotkeyText)+7];
		for (uint32 i = 0; i < strlen(hotkeyText) + 7; i++)
		{
			newHotkeyText[i] = NULL;
		}

		strcat(newHotkeyText, "Learn ");
		strcat(newHotkeyText, hotkeyText);

		delete hotkeyText;
		return newHotkeyText;
	}
	else
	{
		return hotkeyText;
	}
}

void TooltipDisplayUnload()
{
	Tooltip.Unload();
}