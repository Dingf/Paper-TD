#ifndef INC_TGA_H
#define INC_TGA_H

#include <stdio.h>
#include "type.h"
#include "image.h"

//If the seventh bit of a header is set, it is a RLE packet.
#define RLE_BIT (1 << 7)

//Enumeration of the possible values for <type>
enum TGATypes
{
	TGA_NONE =				0,
	TGA_COLOR_MAPPED = 		1,
	TGA_BGR	=				2,
	TGA_GREYSCALE =			3,
	TGA_COLOR_MAPPED_RLE =	9,
	TGA_BGR_RLE =	 		10,
	TGA_GREYSCALE_RLE =		11
};

//The TARGA image structure. See the individual functions in tga.cpp for more detail about a
//specific function(s).
struct TGA : public Image
{
	public:
		TGA();
		TGA(const TGA& source);
		~TGA();

		void operator = (const TGA& source);

		bool Load(const char * filename);
		void Unload();
	private:
		//These functions are only used during operations on the TARGA file, so it is not necessary
		//for other functions to be able to call them.
		void LoadCompressedImageData(FILE * file);
		void LoadImageData(FILE * file);
		bool LoadHeader(FILE * file);
		void RBSwap(uint32 index);
		void RBSwap(uint8 * address);

		//Records whether or not the image is compressed using RLE.
		bool compressed;

		//Contains a value from TGATypes that describes the TARGA image.
		uint8 type;

		//Image specifications
		uint8 pxDepth;
};

#endif//INC_TGA_H