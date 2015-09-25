#ifndef INC_FREETYPE_H
#define INC_FREETYPE_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include "type.h"
#include "point.h"
#include "rect.h"
#include "color.h"

#pragma comment (lib, "freetype.lib")

enum FontFormats
{
	FORMAT_LEFT = 0,
	FORMAT_CENTER = 1,
	FORMAT_RIGHT = 2,	
};

struct Font
{
	public:
		Font(const char * filename, Float size);
		~Font();

		void LoadFont(const char * filename, Float size);

		void PrintText(const char * text, const Point2D& pos, const Rect& bounds, uint8 format) const;
		
		Float CalculateTextHeight(const char * text, uint32 textWidth) const;
		Float CalculateLineLength(const char * text, uint32 textWidth, uint32 textPos) const;

		void Unload();
	private:
		void LoadCharacter(const FT_Face& face, uint8 character);

		uint32 GetWordLength(const char * text, uint32 textPos) const;
		uint32 GetWordSpace(const char * text, uint32 textPos) const;

		void ParseSpecial(const char * text, uint32& pos, bool active) const;

		uint32 displayLists;
		uint32 * textures;

		uint32 * width;

		Float baseSize;
};

extern Font * FeltPen[40];

void InitFreeType();
void UnloadFreeType();

#endif