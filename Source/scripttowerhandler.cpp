#include "type.h"
#include "script.h"
#include "scriptdef.h"
#include "tower.h"
#include "grid.h"
#include "game.h"

Float FireTower(Script * script)
{
	Tower * tower = (Tower *)script->GetSource();
	if ((tower != NULL) && (tower->GetState() == true) && (tower->CheckType(OBJ_TYPE_TOWER) == true))
	{
		AddProjectile(tower);
		return true;
	}
	return false;
}

Float BuildTower(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * targetWord = script->GetNextWord();
		Word * posWord = script->GetNextWord();

		Point2D position(0, 0);
		Point2D * pos = (Point2D *)(uint32)posWord->value;
		if (pos != NULL)
		{
			position.SetValues(pos->GetX(), pos->GetY());
		}
		else
		{
			return false;
		}

		if ((IntervalCheck(position.GetX(), 1, GRID_SIZE - 2) == true) && (IntervalCheck(position.GetY(), 1, GRID_SIZE - 2) == true))
		{
			if (targetWord->type == POINTER)
			{
				Object * target = script->GetTarget(targetWord);
				if ((target != NULL) && (target->CheckType(OBJ_TYPE_TOWER) == true))
				{
					Grid * grid = WorldGame.GetGrid();
					Tower * tower = (Tower *)target;

					//To bypass grid auto-filling
					tower->SetState(false);

					Point2D towerCoords = tower->GetPosition();
					towerCoords.SetValues((towerCoords.GetX() - 80)/32, (towerCoords.GetY() - 80)/32);
					grid->SetPath(towerCoords, 0);	

					if (BuildTower(tower->GetTowerType(), position, true) == true)
					{
						Tower * newTower = grid->GetData(Point2D(position.GetX(), position.GetY()));
						tower->SetState(true);
						grid->FindPath(Origin);
						newTower->Copy(*tower);
						newTower->SetUInt32Value(TOWER_VAR_BUILD_TIME, GetGameTime());
						return true;
					}
					else
					{
						tower->SetState(true);
						grid->FindPath(Origin);
					}
				}
			}
			else if ((targetWord->type == NUMERIC) && (IntervalCheck(targetWord->value, 1, GetTowerDataSize()) == true))
			{
				return (BuildTower(targetWord->value, position, true));
			}
		}
	}
	return false;
}