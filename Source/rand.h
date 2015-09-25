#ifndef INC_MERSENNE_TWISTER_H
#define INC_MERSENNE_TWISTER_H

#include "type.h"

bool GetGeneratorState();

void SeedGenerator(uint32 seed);
void TimeSeedGenerator();

uint32 RandomUInt32();
Float RandomFloat();
Float RandomRange(Float min, Float max);

#endif