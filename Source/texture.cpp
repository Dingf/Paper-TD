#include "opengl.h"
#include "type.h"
#include "texture.h"

Texture::Texture(uint8 * data, uint32 width, uint32 height, uint8 bpp)
{
	if ((data != NULL) && (width > 0) && (height > 0) && (bpp > 0) && (bpp <= 4))
	{
		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, bpp, width, height, 0, (bpp == 4) ? GL_RGBA: GL_RGB, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		this->width = width;
		this->height = height;
	}
}
