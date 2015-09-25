#include <string>
#include "type.h"
#include "utility.h"
#include "texture.h"
#include "image.h"
#include "raw.h"
#include "tga.h"

char * ExtensionTable[] = 
{
	"RAW",
	"TGA"
};


Image::Image()
{
	imageData = NULL;
	bpp = 0;
	width = 0;
	height = 0;
}

Image::Image(const Image& source)
{
	imageData = NULL;
	bpp = 0;
	width = 0;
	height = 0;
	*this = source;
}

Image::~Image()
{
	Unload();
}

void Image::operator = (const Image& source)
{
	Unload();

	uint32 size = source.width * source.height * source.bpp;
	imageData = new uint8[size];

	if (imageData == NULL)
	{
		return;
	}

	memcpy(imageData, source.imageData, size);
	width = source.width;
	height = source.height;
	bpp = source.bpp;
}

void Image::Desaturate()
{
	if ((bpp > 1) && (imageData != NULL))
	{
		//Image size is not multiplied by bytes per pixel. Instead, it is done in the loop.
		uint32 imageSize = width * height;

		//Create new image data to hold the desaturated image.
		uint8 * desatData = new uint8[imageSize];

		//Desaturate every pixel. The most commonly used method is 0.30R, 0.59G, 0.11B.
		for (uint32 i = 0; i < imageSize; i++)
		{
			desatData[i] = (0.30 * imageData[(i*bpp)]) +
						   (0.59 * imageData[(i*bpp)+1]) + 
						   (0.11 * imageData[(i*bpp)+2]);
		}

		//Get rid of the old image...
		Delete(imageData);

		//...so that we can replace it with the desaturated one.
		imageData = desatData;

		//Reconfigure our settings to match the new image
		bpp = 1;					//1 byte per pixel
	}
}

uint32 Image::GetTextureID() const
{
	//The Texture structure ensures that we have a valid image. If not, the texture ID is NULL.
	Texture texture(imageData, width, height, bpp);
	return texture.GetTextureID();
}

Texture Image::GetTexture() const
{
	return Texture(imageData, width, height, bpp);
}

void Image::Unload()
{
	Delete(imageData);
	width = 0;
	height = 0;
	bpp = 0;
}

uint32 CheckImageExtension(const char * extension)
{
	for (uint32 i = 0; i < sizeof(ExtensionTable)/sizeof(char *); i++)
	{
		if (strcmp(ExtensionTable[i], extension) == 0)
		{
			return i + 1;
		}
	}
	return NULL;
}

RAW * LoadRAW(const char * filename, const char * properties)
{
	RAW * raw = new RAW();

	uint32 values[3];

	for (uint8 i = 0; i < 3; i++)
	{
		values[i] = 0;
	}

	uint8 counter = 0;
	for (uint32 i = 0; i < strlen(properties); i++)
	{
		if (values[2] != 0)
		{
			break;
		}
		else if (properties[i] == 'x')
		{
			counter++;
			continue;
		}
		else
		{
			values[counter] *= 10;
			values[counter] += properties[i] - 48;
		}
	}

	bool result = raw->Load(filename, values[0], values[1], values[2]);

	return (result == true) ? raw : NULL;
}

Image * LoadImageFromFile(const char * filename, const char * properties)
{
	char extension[4];

	for (uint32 i = 0; i < strlen(filename); i++)
	{
		if (filename[i] == '.')
		{
			for (uint32 j = 1; j <= 3; j++)
			{
				extension[j-1] = filename[i+j];
			}
			extension[3] = '\0';

			ToUpper(extension);

			switch (CheckImageExtension(extension))
			{
				case 1:
				{
					return LoadRAW(filename, properties);
				}
				case 2:
				{
					TGA * tga = new TGA();
					tga->Load(filename);
					return tga;
				}
				default:
				{
					return false;
				}
			}

			break;
		}
	}
	return NULL;
}
