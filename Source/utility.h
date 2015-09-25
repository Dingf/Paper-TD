#ifndef INC_UTILITY_H
#define INC_UTILITY_H

#include "type.h"

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

template <class T> 
void Swap(T& a, T& b)
{
	T c(a); 
	a = b; 
	b = c;
}

#define IntSwap(v1, v2) ((v1)^=(v2)^=(v1)^=(v2))

#define EndianSwap32(val) ((((val)&(0x000000FF))<<(24))|(((val)&(0x0000FF00))<<(8))|(((val)&(0x00FF0000))>>(8))|(((val)&(0xFF000000))>>(24)))
#define EndianSwap16(val) ((((val)&(0x00FF))<<(8))|(((val)&(0xFF00))>>(8)))

#define IntMerge8(val1, val2, val3, val4) ((((val1)&(0xFF))<<(24))|(((val2)&(0xFF))<<(16))|(((val3)&(0xFF))<<(8))|((val4)&(0xFF)))
#define IntMerge16(val1, val2) (((val1)&(0xFFFF)<<(16))|((val2)&(0xFFFF)))

#define ZeroFit(val) (((val)<(0))?(0):(val))
#define MaxFit(val, max) (((val)>(max))?(max):(val))
#define MinFit(val, min) (((val)<(min))?(min):(val))
#define IntervalFit(val, min, max) (((val)<(min))?(min):(((val)>(max))?(max):(val)))

#define GetMax(val1, val2) (((val1)>(val2))?(val1):(val2))
#define GetMin(val1, val2) (((val1)<(val2))?(val1):(val2))

#define IntervalCheck(val, min, max) (((val)>=(min))&&((val)<=(max)))

#define CheckFlag(val,flag) ((((val)&(flag))==(flag))?(true):(false))

void Delete(void * data);

char ToLower(char character);
void ToLower(char * string);
char ToUpper(char character);
void ToUpper(char * string);

char * StringCopy(const char * string);
int32 StringChar(const char * string, char character);

uint32 Next2(uint32 value);

uint8 StringToHex8(const char * string, uint32 start);
uint16 StringToHex16(const char * string, uint32 start);
uint32 StringToHex32(const char * string, uint32 start);

char * FloatToString(Float value);
#endif