#ifndef INC_WAVE_H
#define INC_WAVE_H

#include "type.h"
#include "object.h"
#include "creep.h"

#define WAVE_SIZE 25;
#define WAVE_INTERVAL 250;

enum WaveVars
{
	WAVE_VAR_NUMBER,
	WAVE_VAR_SIZE,
	WAVE_VAR_INTERVAL,
	WAVE_VAR_ADD_TIME,
	WAVE_VAR_CREEP_COUNT,
	WAVE_UINT32_VARS,
};

struct Wave : public Object
{
	public:
		Wave();
		~Wave();

		bool GetState() const { return ((GetCreepCount() < GetSize()) && (base != NULL)); }

		uint8 GetWaveType() const { return (base != NULL) ? base->GetCreepType() : NULL; }
		uint32 GetSize() const { return GetUInt32Value(WAVE_VAR_SIZE); }
		uint32 GetCreepCount() const { return GetUInt32Value(WAVE_VAR_CREEP_COUNT); }

		bool AddWave(uint32 offset, uint32 wave, uint8 type);
		void AddCreep();

		void SendWave() { SetUInt32Value(WAVE_VAR_ADD_TIME, NULL); }

		void Unload();
	private:
		Creep * base;
};

List<Wave *> * GetActiveWaves();

void WaveMaintenance();
void ClearActiveWaves();

#endif