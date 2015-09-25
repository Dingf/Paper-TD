#include "type.h"
#include "keyboard.h"
#include "keyboardhandler.h"
#include "creep.h"
#include "wave.h"
#include "selection.h"
#include "game.h"

bool KeyboardNamespace::InitWaves()
{
	List<Wave *> * ActiveWaves = GetActiveWaves();

	if (ActiveWaves->GetSize() == 0)
	{
		uint32 preloadCount = WorldGame.GetUInt32Value(GAME_VAR_WAVE_PRELOAD_COUNT);
		for (uint32 i = 0; i < preloadCount; i++)
		{
			ActiveWaves->Append(new Wave);
			Wave * wave = ActiveWaves->GetLast();

			wave->AddWave(i * WorldGame.GetUInt32Value(GAME_VAR_WAVE_TIME_DIFFERENCE), i + 1, 1);
		}
		WorldGame.SetUInt32Value(GAME_VAR_INSTANT_CAST_FLAG, false);
		return true;
	}
	return false;
}

bool KeyboardNamespace::SendNextWave()
{
	List<Creep *> * ActiveCreeps = GetActiveCreeps();
	List<Wave *> * ActiveWaves = GetActiveWaves();

	if (ActiveCreeps->GetSize() == 0)
	{
		Wave * wave = ActiveWaves->GetFirst();
		int32 timeDif = GetGameTime() - wave->GetUInt32Value(WAVE_VAR_ADD_TIME);
		
		ActiveWaves->ToStart();
		for (uint32 i = 0; i < ActiveWaves->GetSize(); i++)
		{
			wave = ActiveWaves->GetCurrent();
			ActiveWaves->ToNext();

			wave->ModUInt32Value(WAVE_VAR_ADD_TIME, timeDif);
		}
		return true;
	}
	return false;
}

bool KeyboardNamespace::ClearActiveState()
{
	WorldGame.SetUInt32Value(GAME_VAR_ACTIVE_STATE, ACTIVE_STATE_NORMAL);
	return false;
}

bool KeyboardNamespace::SetActiveStateAttack()
{
	Selection * selection = WorldGame.GetSelection();
	if (selection->GetSize() > 0)
	{
		WorldGame.SetUInt32Value(GAME_VAR_ACTIVE_STATE, ACTIVE_STATE_ATTACK);
		return true;
	}
	return false;
}

bool KeyboardNamespace::ToggleTowerWait()
{
	Selection * selection = WorldGame.GetSelection();
	if (selection->GetSize() > 0)
	{
		for (uint32 i = 0; i < selection->GetSize(); i++)
		{
			Tower * tower = selection->GetTower(i);
			if ((tower != NULL) && (tower->GetState() == true) && (tower->GetInt32Value(TOWER_VAR_CAN_FIRE) == true) && (tower->GetActiveSpell() != NULL))
			{
				tower->SetUInt32Value(TOWER_VAR_WAITING, 1 - tower->GetUInt32Value(TOWER_VAR_WAITING));
			}
		}
		return true;
	}
	return false;
}


bool KeyboardNamespace::AssignSelection()
{
	WorldGame.SetHotkey(GetLastSpecialKey());
	return true;
}

bool KeyboardNamespace::GetHotkeySelection()
{
	Selection * hotkey = WorldGame.GetHotkey(GetLastSpecialKey());
	WorldGame.SetSelection(*hotkey);
	return true;
}

bool KeyboardNamespace::ClearSelection()
{
	WorldGame.ClearSelection();
	return true;
}

//Testing function only. Delete me soon
bool KeyboardNamespace::Delete()
{
	Selection * selection = WorldGame.GetSelection();
	Tower * tower = selection->GetTower(0);

	if (tower != NULL)
	{
		tower->SetState(false);
	}
	return true;
}
