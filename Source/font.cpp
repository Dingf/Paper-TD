#include <string.h>
#include <ft2build.h>
#include "opengl.h"
#include "type.h"
#include "utility.h"
#include "point.h"
#include "rect.h"
#include "color.h"
#include "font.h"

FT_Library FreeType;

Font * FeltPen[40];

void InitFreeType()
{
	FT_Init_FreeType(&FreeType);
	for (uint8 i = 0; i < 40; i++)
	{
		FeltPen[i] = new Font("Assets\\FELTPEN_.TTF", i + 8);
	}
}

void UnloadFreeType()
{
	for (uint8 i = 0; i < 40; i++)
	{
		delete FeltPen[i];
	}
	FT_Done_FreeType(FreeType);
}

Font::Font(const char * filename, Float size)
{
	width = new uint32[128];
	textures = new uint32[128];

	glGenTextures(128, textures);
	displayLists = glGenLists(128);

	LoadFont(filename, size);
}

Font::~Font()
{
	Unload();
}

void Font::LoadFont(const char * filename, Float size)
{
	baseSize = size;
	//baseSize /= 16.0;

	FT_Face face;

	FT_New_Face(FreeType, filename, 0, &face);
	FT_Set_Char_Size(face, 16 * baseSize, 16 * baseSize, 300, 300); 

	FT_GlyphSlot slot = face->glyph;

	for (uint8 i = 0; i < 128; i++)
	{
		FT_Load_Glyph(face, FT_Get_Char_Index(face, i), FT_LOAD_DEFAULT);
		FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);

		width[i] = slot->advance.x >> 6;

		LoadCharacter(face, i);
	}
}

void Font::LoadCharacter(const FT_Face& face, uint8 character)
{
	FT_Bitmap bitmap = face->glyph->bitmap;

	uint32 width = Next2(bitmap.width);
	uint32 height = Next2(bitmap.rows);

	uint8 * imageData = new uint8[2*width*height];

	for (uint32 i = 0; i < height; i++) 
	{
		for (uint32 j = 0; j < width; j++)
		{
			uint32 index = 2 * (j + (i * width));
			if ((j >= bitmap.width) || (i >= bitmap.rows))
			{
				imageData[index] = 0;
			}
			else
			{
				imageData[index] = bitmap.buffer[j+(bitmap.width*i)];
			}
			imageData[index+1] = imageData[index];
		}
	}

	glBindTexture(GL_TEXTURE_2D, textures[character]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, imageData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	Delete(imageData);

	int32 left = face->glyph->bitmap_left;
	int32 top = face->glyph->bitmap_top;

	glNewList(displayLists + character, GL_COMPILE);

	glBindTexture(GL_TEXTURE_2D, textures[character]);

	glTranslated(left, -top, 0);

	Float x = (Float)bitmap.width/(Float)width;
	Float y = (Float)bitmap.rows/(Float)height;

	glBegin(GL_QUADS);
		glTexCoord2d(0, 0); glVertex2f(0, 0);
		glTexCoord2d(0, y); glVertex2f(0,  bitmap.rows);
		glTexCoord2d(x, y); glVertex2f(bitmap.width,  bitmap.rows);
		glTexCoord2d(x, 0); glVertex2f(bitmap.width, 0);
	glEnd();

	glTranslated(-left, top, 0);

	glTranslated(face->glyph->advance.x >> 6, 0, 0);

	glEndList();

	glEndList();
}

uint32 Font::GetWordLength(const char * text, uint32 textPos) const
{
	if (textPos >= strlen(text))
	{
		return NULL;
	}

	uint32 length = 0;
	for (;text[textPos] != ' '; textPos++)
	{
		/*if (text[textPos] == '&')
		{
			ParseSpecial(text, textPos, false);
			continue;
		}
		else */if (textPos >= strlen(text))
		{
			break;
		}
		length++;
	}

	return length;
}

uint32 Font::GetWordSpace(const char * text, uint32 textPos) const
{
	if (text == NULL)
	{
		return NULL;
	}

	uint32 space = 0;
	for (uint32 i = textPos; text[i] != ' '; i++)
	{
		if (text[i] == '&')
		{
			ParseSpecial(text, i, false);
			continue;
		}
		else if ((text[i] == '\n') || ((text[i] == '\\') && (text[i+1] == 'n')) || (text[i] == NULL) )
		{
			break;
		}
		space += width[text[i]];
	}

	return space;
}

void Font::ParseSpecial(const char * text, uint32& pos, bool active) const
{
	switch (text[pos+1])
	{
		case 'c':
		{
			for (uint8 i = 1; i <= 8; i++)
			{
				char c = text[pos+i+1];
				if (!((c >= '0') && (c <= '9')) && !((c >= 'a') && (c <= 'f')) && !((c >= 'A') && (c <= 'F')))
				{
					pos += i;
					break;
				}
				else if (i == 8)
				{
					if (active == true)
					{
						uint32 colorValues = StringToHex32(text, pos + 2);
						ColorARGB newColor(colorValues);

						glColor(newColor);
					}
					pos += 9;
				}
			}
			break;
		}
		default:
		{
			pos++;
			break;
		}
	}
}

void Font::PrintText(const char * text, const Point2D& pos, const Rect& bounds, uint8 format) const
{
	Point2D offset;

	glPushMatrix();
	glLoadIdentity();
	glColor(Black);
	glTranslated(pos.GetX(), pos.GetY(), 0);

	uint8 justify = format & 0x03;

	Float lineOffset = (int32)(CalculateLineLength(text, bounds.GetWidth(), 0)/2.0 * justify);
	offset.SetValues(offset.GetX() + lineOffset, offset.GetY());
	glTranslated(lineOffset, 0.0, 0);

	uint32 textPos = 0;
	while (textPos < strlen(text))
	{
		bool wordBegin = true;
		uint32 wordLength = GetWordLength(text, textPos);
		uint32 wordSpace = GetWordSpace(text, textPos);

		for (uint32 i = textPos; i < textPos + wordLength; i++)
		{
			if (text[i] == '&')
			{
				ParseSpecial(text, i, true);
				continue;
			}
			else if (((text[i] == '\n') || ((text[i] == '\\') && (text[i+1] == 'n')) || (offset.GetX() > bounds.GetWidth())) || ((wordSpace + offset.GetX() >= bounds.GetWidth()) && (wordSpace < bounds.GetWidth()) && (wordBegin == true)))
			{
				glTranslated(-offset.GetX(), baseSize, 0);
				offset.SetValues(0, offset.GetY() + baseSize);
				if (offset.GetY() > bounds.GetHeight())
				{
					glPopMatrix();
					return;
				}
				lineOffset = (int32)(CalculateLineLength(text, bounds.GetWidth(), i + 1)/2.0 * justify);
				offset.SetValues(offset.GetX() + lineOffset, offset.GetY());
				glTranslated(lineOffset, 0.0, 0);

				if (text[i] == '\n')
				{
					continue;
				}
				else if ((text[i] == '\\') && (text[i+1] == 'n'))
				{
					i++;
					continue;
				}
			}
			glCallList(text[i] + displayLists);
			offset.SetValues(offset.GetX() + width[text[i]], offset.GetY());
			wordBegin = false;
		}

		offset.SetValues(offset.GetX() + baseSize, offset.GetY());
		glTranslated(baseSize, 0, 0);

		textPos += wordLength + 1;
	}

	glPopMatrix();
}

Float Font::CalculateTextHeight(const char * text, uint32 textWidth) const
{
	Point2D offset(0, baseSize);

	uint32 textPos = 0;
	while (textPos < strlen(text))
	{
		bool wordBegin = true;
		uint32 wordLength = GetWordLength(text, textPos);
		uint32 wordSpace = GetWordSpace(text, textPos);

		for (uint32 i = textPos; i < textPos + wordLength; i++)
		{
			if (text[i] == '&')
			{
				ParseSpecial(text, i, false);
				continue;
			}
			else if (((text[i] == '\n') || ((text[i] == '\\') && (text[i+1] == 'n')) || (offset.GetX() > textWidth)) || ((wordSpace + offset.GetX() >= textWidth) && (wordSpace < textWidth) && (wordBegin == true)))
			{
				offset.SetValues(0, offset.GetY() + baseSize);
				if (text[i] == '\n')
				{
					continue;
				}
				else if ((text[i] == '\\') && (text[i+1] == 'n'))
				{
					i++;
					continue;
				}
			}
			offset.SetValues(offset.GetX() + width[text[i]], offset.GetY());
			wordBegin = false;
		}

		offset.SetValues(offset.GetX() + baseSize, offset.GetY());
		textPos += wordLength + 1;
	}

	return offset.GetY();
}


Float Font::CalculateLineLength(const char * text, uint32 textWidth, uint32 textPos) const
{
	Float lineLength = 0;

	while (textPos < strlen(text))
	{
		bool wordBegin = true;
		uint32 wordLength = GetWordLength(text, textPos);
		uint32 wordSpace = GetWordSpace(text, textPos);

		for (uint32 i = textPos; i < textPos + wordLength; i++)
		{
			if (text[i] == '&')
			{
				ParseSpecial(text, i, false);
				continue;
			}
			else if (((text[i] == '\n') || ((text[i] == '\\') && (text[i+1] == 'n')) || (lineLength > textWidth)) || ((wordSpace + lineLength + baseSize >= textWidth) && (wordSpace < textWidth) && (wordBegin == true)))
			{
				return textWidth - lineLength;
				if (text[i] == '\n')
				{
					continue;
				}
				else if ((text[i] == '\\') && (text[i+1] == 'n'))
				{
					i++;
					continue;
				}
			}
			lineLength += width[text[i]];
			wordBegin = false;
		}

		textPos += wordLength + 1;
		if (textPos < strlen(text))
		{
			lineLength += baseSize;
		}
	}

	return textWidth - lineLength;
}

void Font::Unload()
{
	if (displayLists != NULL)
	{
		glDeleteLists(displayLists, 128);
		displayLists = NULL;
	}
	if (textures != NULL)
	{
		glDeleteTextures(128, textures);
	}

	Delete(width);
	Delete(textures);
}

