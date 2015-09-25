#include "type.h"
#include "rect.h"
#include "mouse.h"
#include "mousehandler.h"
#include "creep.h"
#include "selection.h"
#include "game.h"

bool MouseNamespace::OnClickTrigger(uint8 button, const Point2D& position)
{
	if (button == BUTTON_LEFT)
	{
		WorldGame.ActivateTriggeredScript(GAME_TRIGGER_ON_CLICK);
	}
	return WorldGame.RemoveTriggeredScript(GAME_TRIGGER_ON_CLICK);
}

bool MouseNamespace::SelectionAttack(uint8 button, const Point2D& position)
{
	Point2D mousePos = WorldMouse.GetPosition();
	Selection * selection = WorldGame.GetSelection();

	if (selection->GetSize() > 0)
	{
		List<Creep *> * ActiveCreeps = GetActiveCreeps();

		ActiveCreeps->ToStart();
		for (uint32 i = 0; i < ActiveCreeps->GetSize(); i++)
		{
			Creep * creep = ActiveCreeps->GetCurrent();
			ActiveCreeps->ToNext();

			if ((creep != NULL) && (creep->GetState() == true))
			{
				Point2D creepPos = creep->GetPosition();
				Rect creepBounds(creepPos.GetX(), creepPos.GetY(), creepPos.GetX() + 32, creepPos.GetY() + 32);

				if (creepBounds.ContainsPoint(mousePos) == true)
				{
					for (uint32 j = 0; j < selection->GetSize(); j++)
					{
						Tower * tower = selection->GetTower(j);

						if (tower->GetInt32Value(TOWER_VAR_CAN_FIRE) == true)
						{
							tower->SetAttackTarget(creep);
							tower->SetUInt32Value(TOWER_VAR_WAITING, false);
						}
					}
					return true;
				}
			}
		}
	}
	return false;
}

bool MouseNamespace::ClearActiveState(uint8 button, const Point2D& position)
{
	WorldGame.SetUInt32Value(GAME_VAR_ACTIVE_STATE, ACTIVE_STATE_NORMAL);
	return false;
}

bool MouseNamespace::ActiveStateAttack(uint8 button, const Point2D& position)
{
	if (WorldGame.GetUInt32Value(GAME_VAR_ACTIVE_STATE) == ACTIVE_STATE_ATTACK)
	{
		if (SelectionAttack(button, position) == true)
		{
			WorldGame.SetUInt32Value(GAME_VAR_ACTIVE_STATE, ACTIVE_STATE_NORMAL);
			return true;
		}
	}
	return false;
}
//
//	if ((selection->GetSize() > 0) && )
//	{
//		std::vector<Creep *> * CreepData = GetActiveCreeps();
//		for (uint32 i = 0; i < CreepData->size(); i++)
//		{
//			Creep * creep = CreepData->at(i);
//			if ((creep != NULL) && (creep->GetState() == true))
//			{
//				Point2D creepPos = creep->GetPosition();
//				Rect creepBounds(creepPos.GetX(), creepPos.GetY(), creepPos.GetX() + 32, creepPos.GetY() + 32);
//
//				if (creepBounds.Intersect(mousePos) == true)
//				{
//					for (uint32 j = 0; j < selection->GetSize(); j++)
//					{
//						Tower * tower = selection->GetTower(j);
//						tower->SetAttackTarget(creep);
//					}
//					WorldGame.SetUInt32Value(GAME_VAR_ACTIVE_STATE, ACTIVE_STATE_NORMAL);
//					return true;
//				}
//			}
//		}
//	}
//	return false;
//}

bool MouseNamespace::SetActiveStateAttack(uint8 button, const Point2D& position)
{
	Selection * selection = WorldGame.GetSelection();
	if (selection->GetSize() == 1)
	{
		Tower * tower = selection->GetTower(0);

		if (tower->GetUInt32Value(TOWER_VAR_CAN_FIRE) == true)
		{
			WorldGame.SetUInt32Value(GAME_VAR_ACTIVE_STATE, ACTIVE_STATE_ATTACK);
			return true;
		}
	}
	return false;
}