#ifndef INC_IMAGE_H
#define INC_IMAGE_H

#include "type.h"
#include "texture.h"

struct Image
{
	public:
		Image();
		Image(const Image& source);
		virtual ~Image();

		virtual void operator = (const Image& source);
		virtual void Unload();

		void Desaturate();

		uint32 GetTextureID() const;
		Texture GetTexture() const;
	protected:
		uint8 bpp;
		uint32 width;
		uint32 height;

		uint8 * imageData;
};

Image * LoadImageFromFile(const char * filename, const char * properties = NULL);

#endif