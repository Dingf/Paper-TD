#ifndef INC_SPRITE_H
#define INC_SPRITE_H

#include "type.h"
#include "point.h"
#include "rect.h"
#include "color.h"
#include "texture.h"
#include "image.h"

struct Sprite
{
	public:
		Sprite();
		~Sprite();

		bool LoadSprite(const char * filename);
		bool CreateSprite(const Texture& texture);

		void DrawSprite(const Rect& bounds, const Point2D& position, const Point2D& scalingCenter, const Point2D& rotationCenter, Float scale, Float rotate, ColorARGB color) const;

		void Unload();

		uint32 GetWidth() const { return width; }
		uint32 GetHeight() const { return height; }
	private:
		uint32 texture;
		uint32 width;
		uint32 height;
};

#endif