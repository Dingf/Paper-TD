#ifndef INC_TEXTURE_H
#define INC_TEXTURE_H

#include "type.h"

struct Texture
{
	public:
		Texture(uint8 * data, uint32 width, uint32 height, uint8 bpp);

		uint32 GetTextureID() const { return texture; }
		uint32 GetWidth() const { return width; }
		uint32 GetHeight() const { return height; }
	private:
		uint32 texture;
		uint32 width;
		uint32 height;
};

#endif