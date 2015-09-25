#ifndef INC_RAW_H
#define INC_RAW_H

#include "type.h"
#include "image.h"

//The RAW image structure. See the individual functions in raw.cpp for more detail about a
//specific function(s).
struct RAW : public Image
{
	//Note that there isn't much here. This is because a RAW image is essentially the same
	//as the Image struct. All we need is a way to load it.
	public:
		bool Load(const char * filename, uint32 width, uint32 height, uint8 bpp);
	private:	
};

#endif