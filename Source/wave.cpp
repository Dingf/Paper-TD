#include "type.h"
#include "math.h"
#include "list.h"
#include "creep.h"
#include "wave.h"
#include "game.h"

List<Wave *> ActiveWaves;
List<Wave *> RemovedWaves;

Wave::Wave()
{
	AllocateUInt32Values(WAVE_UINT32_VARS);
	base = NULL;
}

Wave::~Wave()
{
	Unload();
}

bool Wave::AddWave(uint32 offset, uint32 wave, uint8 type)
{
	if (base == NULL)
	{
		SetState(true);

		SetUInt32Value(WAVE_VAR_NUMBER, wave);
		SetUInt32Value(WAVE_VAR_SIZE, WorldGame.GetUInt32Value(GAME_VAR_WAVE_SIZE));
		SetUInt32Value(WAVE_VAR_INTERVAL, WorldGame.GetUInt32Value(GAME_VAR_WAVE_INTERVAL));

		base = new Creep;

		Float hp = 10000;//ExpN(WorldGame.GetFloatValue(GAME_VAR_CREEP_HP_SCALE), (int32)wave) * WorldGame.GetFloatValue(GAME_VAR_BASE_CREEP_HP);

		base->SetFloatValue(CREEP_VAR_HITPOINTS, hp);
		base->SetFloatValue(CREEP_VAR_MAX_HITPOINTS, hp);
		base->SetUInt32Value(CREEP_VAR_BASE_HP, hp);
		base->SetUInt32Value(CREEP_VAR_BASE_ARMOR, wave * WorldGame.GetFloatValue(GAME_VAR_CREEP_ARMOR_SCALE));
		base->SetUInt32Value(CREEP_VAR_BASE_BOUNTY, MinFit(wave/WorldGame.GetFloatValue(GAME_VAR_CREEP_BOUNTY_SCALE), 1));

		base->SetUInt32Value(CREEP_VAR_BASE_DAMAGE, 1000 - (wave * 5));

		base->SetUInt32Value(CREEP_VAR_BASE_SPEED, 96);
		base->SetUInt32Value(CREEP_VAR_TYPE, type);

		switch (type)
		{
			case CREEP_TYPE_DROP:
			{
				base->SetUInt32Value(CREEP_VAR_BASE_BOUNTY, base->GetUInt32Value(CREEP_VAR_BASE_BOUNTY) * 1.5);
				base->SetUInt32Value(CREEP_VAR_BASE_SPEED, base->GetUInt32Value(CREEP_VAR_BASE_SPEED) * 2);
				break;
			}
			case CREEP_TYPE_SQUARE:
			{
				base->SetUInt32Value(CREEP_VAR_BASE_BOUNTY, base->GetUInt32Value(CREEP_VAR_BASE_BOUNTY) * 1.8);
				base->SetUInt32Value(CREEP_VAR_BASE_ARMOR, base->GetUInt32Value(CREEP_VAR_BASE_ARMOR) * 4);
				break;
			}
			case CREEP_TYPE_SPIKE:
			{
				base->SetUInt32Value(CREEP_VAR_BASE_BOUNTY, base->GetUInt32Value(CREEP_VAR_BASE_BOUNTY) * 2.0);
				base->SetUInt32Value(CREEP_VAR_BASE_DAMAGE, base->GetUInt32Value(CREEP_VAR_BASE_DAMAGE) * 0.5);
				break;
			}
			case CREEP_TYPE_TRIANGLE:
			{
				base->SetUInt32Value(CREEP_VAR_BASE_BOUNTY, base->GetUInt32Value(CREEP_VAR_BASE_BOUNTY) * 1.5);
				base->SetInt32Value(CREEP_VAR_FLYING, true);
				break;
			}
			case CREEP_TYPE_DIAMOND:
			{
				base->SetUInt32Value(CREEP_VAR_BASE_BOUNTY, base->GetUInt32Value(CREEP_VAR_BASE_BOUNTY) * 2.5);
				base->SetInt32Value(CREEP_VAR_PROJECTILE_IMMUNE, true);
				break;
			}
			case CREEP_TYPE_OVAL:
			{
				base->SetUInt32Value(CREEP_VAR_BASE_BOUNTY, base->GetUInt32Value(CREEP_VAR_BASE_BOUNTY) * 2.5);
				base->SetInt32Value(CREEP_VAR_SPLASH_IMMUNE, true);
				break;
			}
			case CREEP_TYPE_STAR:
			{
				base->SetUInt32Value(CREEP_VAR_BASE_BOUNTY, base->GetUInt32Value(CREEP_VAR_BASE_BOUNTY) * 2.5);
				base->SetInt32Value(CREEP_VAR_SPELL_IMMUNE, true);
				break;
			}
			case CREEP_TYPE_HEART:
			{
				base->SetUInt32Value(CREEP_VAR_BASE_BOUNTY, base->GetUInt32Value(CREEP_VAR_BASE_BOUNTY) * 2.0);
				base->SetFloatValue(CREEP_VAR_HITPOINTS, hp * 2.5);
				base->SetFloatValue(CREEP_VAR_MAX_HITPOINTS, hp * 2.5);
				base->SetUInt32Value(CREEP_VAR_BASE_HP, hp * 2.5);
				break;
			}
		}

		SetUInt32Value(WAVE_VAR_ADD_TIME, GetGameTime() + offset);
		return true;
	}
	return false;
}

void Wave::AddCreep()
{
	int32 addTime = GetUInt32Value(WAVE_VAR_ADD_TIME);
	int32 interval = GetUInt32Value(WAVE_VAR_INTERVAL);
	if ((((int32)GetGameTime() - addTime) >= interval) && (GetState() == true))
	{
		if (GetCreepCount() == 0)
		{
			WorldGame.ModUInt32Value(GAME_VAR_WAVE_NUMBER, 1);
			WorldGame.ModFloatValue(GAME_VAR_MAX_INK, WorldGame.GetFloatValue(GAME_VAR_MAX_INK_INCREMENT));

			ActiveWaves.Append(new Wave);
			Wave * wave = ActiveWaves.GetLast();

			uint32 preloadCount = WorldGame.GetUInt32Value(GAME_VAR_WAVE_PRELOAD_COUNT);
			Float type = RandomRange(1, 15) - 5;
			wave->AddWave(WorldGame.GetUInt32Value(GAME_VAR_WAVE_TIME_DIFFERENCE) * preloadCount, WorldGame.GetUInt32Value(GAME_VAR_WAVE_NUMBER) + preloadCount, IntervalFit(type, 1, 9));
		}

		List<Creep *> * ActiveCreeps = GetActiveCreeps();

		ActiveCreeps->Append(new Creep);

		Creep * creep = ActiveCreeps->GetLast();
		creep->Copy(*base);
		creep->SetState(true);

		creep->SetPosition(369 + (RandomUInt32() % 32), -32);

		uint8 moveTargets = 0;
		Grid * grid = WorldGame.GetGrid();
		for (uint8 i = 1; i <= 2; i++)
		{
			if (grid->GetPath(Point2D(i + 8, 1)) != 1)
			{
				moveTargets += i;
			}
		}
		moveTargets -= (moveTargets > 2) ? ((RandomUInt32() % 2) + 1) : 0;

		creep->CalculateNextMove(Point2D(336 + (moveTargets * 32), 112));

		ModUInt32Value(WAVE_VAR_CREEP_COUNT, 1);
		SetUInt32Value(WAVE_VAR_ADD_TIME, GetGameTime());
	}
}

void Wave::Unload()
{
	if (base != NULL)
	{
		delete base;
		base = NULL;
	}
}

List<Wave *> * GetActiveWaves()
{
	return &ActiveWaves;
}

void WaveMaintenance()
{
	while (RemovedWaves.GetSize() > 0)
	{
		if (RemovedWaves[0] != NULL)
		{
			delete RemovedWaves[0];
		}
		RemovedWaves.Remove(0);
	}

	ActiveWaves.ToStart();
	for (uint32 i = 0; i < ActiveWaves.GetSize(); i++)
	{
		Wave * wave = ActiveWaves.GetCurrent();
		ActiveWaves.ToNext();

		wave->AddCreep();
		if (wave->GetState() == false)
		{
			wave->Unload();
			RemovedWaves.Append(wave);
			ActiveWaves.Remove(i);
			i--;
		}
	}
}

void ClearActiveWaves()
{
	while (ActiveWaves.GetSize() > 0)
	{
		if (ActiveWaves[0] != NULL)
		{
			delete ActiveWaves[0];
		}
		ActiveWaves.Remove(0);
	}
	while (RemovedWaves.GetSize() > 0)
	{
		if (RemovedWaves[0] != NULL)
		{
			delete RemovedWaves[0];
		}
		RemovedWaves.Remove(0);
	}
}