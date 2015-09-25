#ifndef INC_TYPE_H
#define INC_TYPE_H

#include <limits.h>
#include <float.h>

#define FLOAT_STRUCT_SIZE 8

#if FLOAT_STRUCT_SIZE == 8
typedef double Float;
#	define glNormal3v(v) glNormal3dv(v)
#	define glVertex3v(v) glVertex3dv(v)
#	define glTexCoord2(s,t) glTexCoord2d(s,t)
#	define glTexCoord2v(v) glTexCoord2dv(v)
#	define FLOAT_EPSILON DBL_EPSILON;
#elif FLOAT_STRUCT_SIZE == 4
typedef float Float;
#	define glNormal3v(v) glNormal3fv(v)
#	define glVertex3v(v) glVertex3fv(v)
#	define glTexCoord2(s,t) glTexCoord2f(s,t)
#	define FLOAT_EPSILON FLT_EPSILON
#else
#	error Undefined length for type 'Float'
#endif

#ifndef SIZEOF_SHORT
#	if USHRT_MAX == (1 << 8) - 1
#		define SIZEOF_SHORT 1
#	elif USHRT_MAX == (1 << 16) - 1
#		define SIZEOF_SHORT 2
#	elif USHRT_MAX == (1 << 32) - 1
#		define SIZEOF_SHORT 4
#	elif USHRT_MAX == (1 << 64) - 1
#		define SIZEOF_SHORT 8
#	else
#		error Undefined length for type 'short'
#	endif//USHRT_MAX
#endif//SIZEOF_SHORT

#ifndef SIZEOF_INT
#	if UINT_MAX == (1 << 8) - 1
#		define SIZEOF_INT 1
#	elif UINT_MAX == (1 << 16) - 1
#		define SIZEOF_INT 2
#	elif UINT_MAX == (1 << 32) - 1
#		define SIZEOF_INT 4
#	elif UINT_MAX == (1 << 64) - 1
#		define SIZEOF_INT 8
#	else
#		error Undefined length for type 'int'
#	endif//UINT_MAX
#endif//SIZEOF_INT

#ifndef SIZEOF_LONG
#	if ULONG_MAX == (1 << 8) - 1
#		define SIZEOF_LONG 1
#	elif ULONG_MAX == (1 << 16) - 1
#		define SIZEOF_LONG 2
#	elif ULONG_MAX == (1 << 32) - 1
#		define SIZEOF_LONG 4
#	elif ULONG_MAX == (1 << 64) - 1
#		define SIZEOF_LONG 8
#	else
#		error Undefined length for type 'long'
#	endif//ULONG_MAX
#endif//SIZEOF_LONG

#ifndef SIZEOF_LONGLONG
#	if ULLONG_MAX == (1 << 8) - 1
#		define SIZEOF_LONGLONG 1
#	elif ULLONG_MAX == (1 << 16) - 1
#		define SIZEOF_LONGLONG 2
#	elif ULLONG_MAX == (1 << 32) - 1
#		define SIZEOF_LONGLONG 4
#	elif ULLONG_MAX == (1 << 64) - 1
#		define SIZEOF_LONGLONG 8
#	else
#		error Undefined length for type 'long long'
#	endif//ULLONG_MAX
#endif//SIZEOF_LONGLONG

#ifdef INT8_TYPE
	typedef INT8_TYPE int8;
#else
	typedef char int8;
#endif//INT8_TYPE

#ifdef UINT8_TYPE
	typedef UINT8_TYPE uint8;
#else
	typedef unsigned char uint8;
#endif//UINT8_TYPE

#ifdef INT16_TYPE
	typedef INT16_TYPE int16;
#elif SIZEOF_SHORT == 2
	typedef short int16;
#elif SIZEOF_INT == 2
	typedef int int16;
#else
	#error No standard type found with a length of 16 bytes required for the 'INT16' type
#endif//INT16_TYPE

#ifdef UINT16_TYPE
	typedef UINT16_TYPE uint16;
#elif SIZEOF_SHORT == 2
	typedef unsigned short uint16;
#elif SIZEOF_INT == 2
	typedef unsigned int uint16;
#else
	#error No standard type found with a length of 16 bytes required for the 'UINT16' type
#endif//UINT16_TYPE

#ifdef INT32_TYPE
	typedef INT32_TYPE int32;
#elif SIZEOF_INT == 4
	typedef int int32;
#elif SIZEOF_LONG == 4
	typedef long int32;
#elif SIZEOF_LONGLONG == 4
	typedef long long int32;
#else
	#error No standard type found with a length of 32 bytes required for the 'INT32' type
#endif//INT32_TYPE

#ifdef UINT32_TYPE
	typedef UINT32_TYPE uint32;
#elif SIZEOF_INT == 4
	typedef unsigned int uint32;
#elif SIZEOF_LONG == 4
	typedef unsigned long uint32;
#elif SIZEOF_LONGLONG == 4
	typedef unsigned long long uint32;
#else
	#error No standard type found with a length of 32 bytes required for the 'UINT32' type
#endif//UINT32_TYPE

#ifdef INT64_TYPE
	typedef INT64_TYPE int64;
#elif SIZEOF_LONG == 8
	typedef long int64;
#elif SIZEOF_LONGLONG == 8
	typedef long long int64;
#else
	#error No standard type found with a length of 64 bytes required for the 'INT64' type
#endif//INT64_TYPE

#ifdef UINT64_TYPE
	typedef UINT64_TYPE uint64;
#elif SIZEOF_LONG == 8
	typedef unsigned long uint64;
#elif SIZEOF_LONGLONG == 8
	typedef unsigned long long uint64;
#else
	#error No standard type found with a length of 64 bytes required for the 'UINT64' type
#endif//UINT64_TYPE

#ifndef NULL
#define NULL 0
#endif

#ifndef NOT_FOUND
#define NOT_FOUND -1
#endif

#pragma warning (disable : 4244 4305 4800 4805 4996 4018)
//Ignore the following warnings:
//4344: 'argument' conversion from 'type1' to 'type2', possible loss of data (i.e. double to float)
//4305: 'argument' truncation from 'type1' to 'type2'
//4800: 'int' forcing value to bool 'true' or 'false' (performance warning)
//4805: 'operator' unsafe mix of type 'type1' and type 'type2' in operation (mostly bitwise operations between bools and ints)
//4996: 'function': This function of variable may be unsafe. Consider using function_s instead. etc.
//4018: 'operator' signed/unsigned mismatch

#endif