#include <time.h>
#include "type.h"
#include "rand.h"
#include "utility.h"

const uint32 N = 624;
const uint32 M = 397;

struct MersenneTwister
{
	bool state;
	uint32 data[N];
	uint32 index;
} MT;

bool GetGeneratorState()
{
	return MT.state;
}

void SeedGenerator(uint32 seed)
{
	MT.state = true;

	MT.data[0] = seed & 0xffffffff;
	
	for (uint32 i = 1; i < N; i++)
	{
		MT.data[i] = (1812433253 * (MT.data[i-1] ^ (MT.data[i-1] >> 30)) + i);
		MT.data[i] &= 0xffffffff;
	}
}

void TimeSeedGenerator()
{
	SeedGenerator(time(NULL));
}

uint32 RandomUInt32()
{
	if (GetGeneratorState() == false)
	{
		return 0;
	}

	uint32 result = 0;
	uint32 matrix[] = { 0x00000000, 0x9908b0df };
	
	uint32 counter = 0;
	for (; counter < (N - M); counter++)
	{
		result = (MT.data[counter] & 0x80000000) | (MT.data[counter+1] & 0x7fffffff);
	    
		MT.data[counter] = MT.data[counter+M] ^ (result >> 1) ^ matrix[(result&0x00000001)];
	}
	for (; counter < (N - 1); counter++) 
	{
		result = (MT.data[counter] & 0x80000000) | (MT.data[counter+1] & 0x7fffffff);

		MT.data[counter] = MT.data[counter+(M-N)] ^ (result >> 1) ^ matrix[(result&0x00000001)];
	}
	
	result = MT.data[1];
	
	result ^= (result >> 11);
    
	result ^= (result << 7) & 0x9d2c5680;
   
	result ^= (result << 15) & 0xefc60000;
    
	result ^= (result >> 18);

	return result;
}

Float RandomFloat()
{
	return (Float)RandomUInt32()/(Float)0xffffffff;
}

Float RandomRange(Float min, Float max)
{
	if (min != max)
	{
		if (min > max)
		{
			Swap(min, max);
		}

		Float difference = max - min;
		return (RandomFloat() * difference) + min;
	}
	return min;
}