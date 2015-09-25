#include <stdio.h>
#include <string.h>
#include "type.h"
#include "utility.h"
#include "image.h"
#include "tga.h"


// TGA() - Default constructor that sets all values to NULL(0)
//
// Parameters: None;
//
// Returns: None
TGA::TGA()
{
	type = TGA_NONE;
	pxDepth = NULL;
	compressed = false;
}

// TGA(const TGA& source) - Copy constructor for the TGA class. See operator = for more info.
//
// Parameters: 
//							const TGA& source - The image to copy into <this>.
//
// Returns: None
TGA::TGA(const TGA& source)
{
	type = TGA_NONE;
	pxDepth = NULL;
	compressed = false;
	*this = source;
}

// ~TGA() - Default deconstructor that calls Unload()
//
// Parameters: None
//
// Returns: None
TGA::~TGA()
{
	Unload();
}

// operator = - Copy operator for the TGA class. This operator will perform a deep copy of <source>
//				into <this>. Any previous data in <this> is cleared before any assignment occurs.
//				When complete, <this> will have an exact copy of all values of <source>, save for
//				the texture ID.
//
// Parameters: 
//				const TGA& source - The image to copy into <this>.
//
// Returns: None
void TGA::operator = (const TGA& source)
{
	Unload();

	uint32 size = source.width * source.height * source.bpp;
	imageData = new uint8[size];

	if (imageData == NULL)
	{
		return;
	}

	memcpy(imageData, source.imageData, size);

	compressed = source.compressed;
	type = source.type;
	bpp = source.bpp;
	pxDepth = source.pxDepth;
	width = source.width;
	height = source.height;
}

// Load() - Loads a TGA file from <filename> into the TGA struct <this> that called it. This
//          function calls LoadHeader() to obtain information about the TARGA image. It will
//          then call one of two functions, LoadImageData() or LoadCompressedImageData(),
//          depending on whether or not the image has been RLE compressed.
//
// Parameters: 
//			const char * filename - The name of the TARGA file to load.
//
// Returns:
//			true - The image was loaded successfully. The image data is contained in <this>.
//			false - Part(s) of the loading process failed. No image data has been loaded.
bool TGA::Load(const char * filename)
{
	//If it is already initialized, then we leave it as is. If you want to replace the image
	//data, you must fist call Unload() before calling this function.
	if (imageData != NULL)
	{
		return false;
	}

	FILE * file = fopen(filename, "rb");
	if (file == NULL)
	{
		return false;
	}

	//Check to see if either LoadHeader failed(i.e. pixel depth not valid) or if there was an
	//error with the file stream. Since LoadHeader() is called first, ferror(file) will have
	//the results of the LoadHeader() function as well.
	if ((LoadHeader(file) == false) || (ferror(file) != NULL))
	{
		Unload();
		fclose(file);
		return false;
	}

	//Allocate memory the size of our image. This should be deallocated later with a call to
	//Unload() or via default deconstructor, which will also call Unload().
	imageData = new uint8[bpp * width * height];
	if (imageData == NULL)
	{
		Unload();
		fclose(file);
		return false;
	}

	if (compressed == true)
	{
		LoadCompressedImageData(file);
	}
	else
	{
		LoadImageData(file);
	}

	//Check to see if any error occured while loading the image data. Unlike LoadHeader(), both
	//functions that load image data return void, since their data cannot be validated.
	if (ferror(file) != NULL)
	{
		Unload();
		fclose(file);
		return false;
	}
	
	fclose(file);
	return true;
}

// LoadHeader() - This function reads in the TARGA header to obtain information that will be
//				  used later when obtaining the actual image data.
//
// Parameters:
//				  FILE * file - a pointer to the file stream that contains the data for the
//								TARGA image.
//
// Returns:
//				  true - The header data was loaded successfully. All information was stored in
//						 <this>.
//				  false - There was an error while loading, and no information will be stored.
bool TGA::LoadHeader(FILE * file)
{
	//Skips:
	//Image ID length (1 byte)
	//Color map type (1 byte)
	fseek(file, 2, SEEK_CUR);

	fread(&type, sizeof(uint8), 1, file);

	//Flag for RLE compression is the fourth byte (1 << 3, or 8).
	if (CheckFlag(type, 8) == true)
	{
		compressed = true;
	}

	//Skips:
	//Color map offset (2 bytes)
	//Color map length (2 bytes)
	//Color map size (1 byte)
	//X-Origin(2 bytes)
	//Y-Origin(2 bytes)
	fseek(file, 9, SEEK_CUR);
	
	fread(&width, sizeof(uint16), 1, file);
	fread(&height, sizeof(uint16), 1, file);
	fread(&pxDepth, sizeof(uint8), 1, file);

	//Pixel depth must be divisible by 8(the size of a byte) and must be at most 4 bytes.
	if ((pxDepth % 8 != 0) || (pxDepth > 32))
	{
		return false;
	}

	//Bytes per pixel = pixel depth/8.
	bpp = pxDepth/8;

	//Skips:
	//Image descriptor
	fseek(file, 1, SEEK_CUR);

	return true;
}

// LoadImageData() - Loads uncompressed image data from <file>. It will attempt to read until
//					 either the bytes read are greater than the image size (bytes per pixel *
//					 width * height) or until the end of file is reached.
//
// Parameters:
//				     FILE * file - a pointer to the file stream that contains the data for the
//								   TARGA image.
//
// Returns: None
void TGA::LoadImageData(FILE * file)
{
	uint32 imageSize = bpp * width * height;

	fread(imageData, sizeof(uint8), imageSize, file);

	//If RGB or RGBA...
	if (bpp >= 3)
	{
		//...swap the R and B values of every pixel.
		for (uint32 i = 0; i < imageSize; i += bpp)
		{
			RBSwap(i);
		}
	}
}

// LoadImageData() - Loads compressed(RLE) image data from <file>. It will attempt to read until
//					 either the bytes read are greater than the image size (bytes per pixel *
//					 width * height) or until the end of file is reached.
//
// Parameters:
//				     FILE * file - a pointer to the file stream that contains the data for the
//								   TARGA image.
//
// Returns: None
void TGA::LoadCompressedImageData(FILE * file)
{
	uint32 imageSize = width * height * bpp;

	uint8 * image = imageData;		//Pointer to the image data. This serves as both our buffer and our position counter.
	uint8 buffer[4];				//Buffer for RLE bits(same size as if we would have used an uint32 to keep track of 
									//the address of the first pixel in <image>, but this way is easier).

	//Bytes read less than image size and not end of file.
	while ((image < imageData + imageSize) && (!feof(file)))
	{
		uint8 header = 0;
		fread(&header, sizeof(uint8), 1, file);

		//Count is the number of pixels we read after the header...

		//If RLE_BIT is set(header >= 128), count = header - 127.
		//Otherwise(header < 128), count = header + 1.
		uint8 count = (header & (RLE_BIT - 1)) + 1;

		//If header is a RLE packet
		if (CheckFlag(header, RLE_BIT) == true)
		{
			//Read into our buffer the values to be repeated.
			fread(buffer, sizeof(uint8), bpp, file);

			for (uint8 i = 0; i < count; i++)
			{
				//Copy the values into our image data.
				memcpy(image, buffer, sizeof(uint8) * bpp);

				//If it is RGB or RGBA, swap the R and B values.
				if (pxDepth >= 24)
				{
					RBSwap(image);
				}

				//Move the address of <image> up by the size of one pixel.
				image += bpp;	
			}
		}
		else	//Otherwise it is a "raw" packet
		{
			for (uint8 j = 0; j < count; j++)
			{
				//Read the pixel data as is and put it in <image>.
				fread(image, sizeof(uint8), bpp, file);
				
				if (pxDepth >= 24)
				{
					RBSwap(image);
				}

				image += bpp;
			}
		}
	}
}


// RBSwap() - Swaps the red and blue values for the pixel at <index>. This is required because
//			  TARGA images store colors as BGR(A), as opposed to the traditional RGB(A).
//
// Paramters: 
//			  uint32 index - The position on the <imageData> of <this> to read the pixel at. The
//							 blue value will be at [index], the green at [index+1], and the red
//							 at [index+2].
//
// Returns: None
void TGA::RBSwap(uint32 index)
{
	//If <index> is within the bounds of the image...
	if ((index + 2) < (bpp * width * height))
	{
		//Swap the values using XORs. This saves us from having to create a temporary variable.
		imageData[index] ^= imageData[index+2];
		imageData[index+2] ^= imageData[index];
		imageData[index] ^= imageData[index+2];
	}
}

// RBSwap() - Swaps the red and blue values for the pixel at <index>. This is required because
//			  TARGA images store colors as BGR(A), as opposed to the traditional RGB(A).
//
// Paramters: 
//			  uint8 * address - The location of the memory on <imageData> that is to be swapped.
//								The blue value will be at [index], the green at [index+1], and 
//								the red at [index+2];
//
// Returns: None
void TGA::RBSwap(uint8 * address)
{
	//If <index> is within the bounds of the image...
	if (((uint32)address + 2) < ((uint32)imageData + (bpp * width * height)))
	{
		uint32 index = (uint32)address - (uint32)imageData;

		//Swap the values using XORs. This saves us from having to create a temporary variable.
		imageData[index] ^= imageData[index+2];
		imageData[index+2] ^= imageData[index];
		imageData[index] ^= imageData[index+2];
	}
}


// Unload() - Deallocates any memory used by the image, and sets all values to 0.
//
// Paramters: None
//
// Returns: None
void TGA::Unload()
{
	type = TGA_NONE;
	pxDepth = 0;
	compressed = false;
}