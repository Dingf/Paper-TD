#include "opengl.h"
#include "type.h"
#include "point.h"
#include "rect.h"
#include "color.h"
#include "texture.h"
#include "image.h"
#include "sprite.h"

Sprite::Sprite()
{
	texture = NULL;
}
Sprite::~Sprite()
{
	Unload();
}

bool Sprite::LoadSprite(const char * filename)
{
	Image * image = LoadImageFromFile(filename);
	if (image != NULL)
	{
		bool result = CreateSprite(image->GetTexture());
		delete image;
		return result;
	}
	return false;
}


bool Sprite::CreateSprite(const Texture& texture)
{
	this->texture = texture.GetTextureID();
	width = texture.GetHeight();
	height = texture.GetWidth();
	return (this->texture != NULL);
}

//All sprites must be square with the power of 2, though you may crop any part you wish
//using the bounding Rect.
//i.e. 2x2, 256x256, 1024x1024
void Sprite::DrawSprite(const Rect& bounds, const Point2D& position, const Point2D& scalingCenter, const Point2D& rotationCenter, Float scale, Float rotate, ColorARGB color) const
{
	if (texture != NULL)
	{
		Float x = position.GetX();
		Float y = position.GetY();

		Float left, top, right, bottom;
		bounds.GetValues(left, top, right, bottom);

		Float scaleX = (right - left)/width;
		Float scaleY = (bottom - top)/height;

		Float offsetX = left/width;
		Float offsetY = top/height;

		glPushMatrix();

		glBindTexture(GL_TEXTURE_2D, texture);

		glColor(color);

		if (rotate != NULL)
		{
			Point2D spriteCenter(rotationCenter.GetX() + (right - left)/2, rotationCenter.GetY() + (bottom - top)/2);

			glTranslatev(spriteCenter);
			glRotated(rotate, 0.0, 0.0, 1.0);
			glTranslatev(!spriteCenter);
		}

		if (scale != 1.0)
		{
			Point2D scaleOffset(((scalingCenter.GetX() - position.GetX()) * (scale - 1.0)),
								((scalingCenter.GetY() - position.GetY()) * (scale - 1.0)));

			glTranslatev(!scaleOffset);
		}

		glBegin(GL_QUADS);
		glTexCoord2d(offsetX,			-offsetY);				glVertex2d(x,								y);
		glTexCoord2d(offsetX,			-(offsetY + scaleY));	glVertex2d(x,								y + ((bottom - top) * scale));
		glTexCoord2d(offsetX + scaleX,	-(offsetY + scaleY));	glVertex2d(x + ((right - left) * scale),	y + ((bottom - top) * scale));
		glTexCoord2d(offsetX + scaleX,	-offsetY);				glVertex2d(x + ((right - left) * scale),	y);	
		glEnd();

		glPopMatrix();
	}
}

void Sprite::Unload()
{
	if (texture != NULL)
	{
		glDeleteTextures(1, &texture);
		texture = NULL;
	}
}