#include <stdio.h>
#include <string.h>
#include "type.h"
#include "image.h"
#include "raw.h"

// Load() = - Loads a RAW file from <filename> into the RAW struct <this> that called it. Since
//			  RAW files do not have any headers or footers to give us information about the 
//			  file, the dimensions and bpp of the image must be known beforehand and used as 
//			  parameters to this function. 
//
// Parameters: 
//			  const char * source - The name of the RAW file to load.
//			  uint32 width - The width of the image, in pixels
//			  uint32 height - The height of the image, in pixels
//			  uint8 bpp - The number of bytes(NOTE: not bits) per pixel
//
// Returns: 
//			  true - The image was loaded successfully. The image data is contained in <this>.
//			  false - Part(s) of the loading process failed. No image data has been loaded.
bool RAW::Load(const char * filename, uint32 width, uint32 height, uint8 bpp)
{
	//If it is already initialized, then we leave it as is. If you want to replace the image
	//data, you must fist call Unload() before calling this function.
	if (imageData != NULL)
	{
		return false;
	}

	//Attempt to allocate enough memory for our image...
	imageData = new uint8[width*height*bpp];
	if (imageData == NULL)
	{
		//...if we can't then return false to signal failure.
		return false;
	}

	//Attempt to open the file...
	FILE * file = fopen(filename, "rb");
	if (file == NULL)
	{
		//...and if we can't then return false.
		return false;
	}

	//Set our image values from the parameters
	this->width = width;
	this->height = height;
	this->bpp = bpp;

	//Read the data from the image. Since RAW files are merely repeated RGB(A) values, we can
	//read the entire block at once, and then check to see if we read it all. If we didn't read
	//it all(i.e. we hit EOF before we finished), then return false.

	if (fread(imageData, sizeof(uint8), width * height * bpp, file) != (width * height * bpp))
	{
		Unload();
		fclose(file);
		return false;
	}
	
	//Close our file stream and return true for success.
	fclose(file);
	return true;
}
