#include "type.h"
#include "script.h"
#include "scriptdef.h"
#include "gameobject.h"
#include "creep.h"
#include "projectile.h"
#include "tower.h"
#include "game.h"

Float SetPosition(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * targetWord = script->GetNextWord();
		Word * posWord = script->GetNextWord();

		Object * target = script->GetTarget(targetWord);
		Point2D * position = (Point2D *)(uint32)posWord->value;

		if ((target != NULL) && (position != NULL))
		{
			Point2D targetPos(position->GetX(), position->GetY());

			if (target->CheckType(OBJ_TYPE_CREEP) == true)
			{
				Creep * creep = (Creep *)target;
				
				uint32 x = (targetPos.GetX() - 80)/32;
				uint32 y = (targetPos.GetY() - 80)/32;

				Grid * grid = WorldGame.GetGrid();
				if ((grid->FindPath(Point2D(x, y), Origin) == true) && (IntervalCheck(x, 1, GRID_SIZE - 2) == true) && (IntervalCheck(y, 1, GRID_SIZE - 2) == true))
				{
					creep->SetPosition(targetPos);
					creep->SetMoveTarget(targetPos);
					return true;
				}
			}
			else
			{
				target->SetPosition(targetPos);
				return true;
			}
		}
	}
	return false;
}

Float SetProjectile(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * targetWord = script->GetNextWord();
		Word * projWord = script->GetNextWord();
		
		Object * target = script->GetTarget(targetWord);
		if ((target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_TOWER) == true))
		{
			Tower * tower = (Tower *)target;
			Projectile * projectile = tower->GetProjectile();

			const Projectile * ProjectileData = GetProjectileData();

			uint32 index = projWord->value;
			if ((index < GetProjectileDataSize()) && (index > 0) && (projectile->GetUInt32Value(PROJ_VAR_BASE_TYPE) != index))
			{
				projectile->SetState(false);
				tower->SetUInt32Value(TOWER_VAR_PROJECTILE_ID, index);
				return true;
			}
		}
	}
	return RETURN_CONTINUE;
}

Float SetGameObjectValues(Script * script)
{
	if (script->VerifyArguments(5) == true)
	{
		Word * targetWord = script->GetNextWord();
		Word * value1 = script->GetNextWord();
		Word * value2 = script->GetNextWord();
		Word * value3 = script->GetNextWord();
		Word * value4 = script->GetNextWord();

		Object * source = script->GetTarget(targetWord);
		if ((source != NULL) && (source->GetState() == true) && (source->CheckType(OBJ_TYPE_GAME_OBJECT) == true))
		{
			GameObject * gameObj = (GameObject *)source;
			return gameObj->SetShapeValues(value1->value, value2->value, value3->value, value4->value);
		}
	}
	return false;
}

Float SetNormalValues(Script * script)
{
	if (script->VerifyArguments(4) == true)
	{
		Word * targetWord = script->GetNextWord();
		Word * variable = script->GetNextWord();
		Word * value = script->GetNextWord();
		Word * type = script->GetNextWord();

		Object * target = script->GetTarget(targetWord);
		Float amount = 0;

		if ((target != NULL) && (IntervalCheck(variable->type, INT32_VAR, FLOAT_VAR) == true))
		{
			if (type->value == KEYWORD_FLAT)
			{
				amount = value->value;
			}
			else if (type->value == KEYWORD_PERCENT)
			{
				amount = target->GetValue(variable->type, variable->value) * (value->value/100.0);
			}
			else
			{
				return false;
			}

			target->SetValue(variable->type, variable->value, amount);
			return true;
		}
	}
	return false;
}

Float ModNormalValues(Script * script)
{
	if (script->VerifyArguments(4) == true)
	{
		Word * targetWord = script->GetNextWord();
		Word * variable = script->GetNextWord();
		Word * value = script->GetNextWord();
		Word * type = script->GetNextWord();

		Object * target = script->GetTarget(targetWord);
		Float amount = 0;

		if ((target != NULL) && (IntervalCheck(variable->type, INT32_VAR, FLOAT_VAR) == true))
		{
			if (type->value == KEYWORD_FLAT)
			{
				amount = value->value;
			}
			else if (type->value == KEYWORD_PERCENT)
			{
				amount = target->GetValue(variable->type, variable->value) * (value->value/100.0);
			}
			else
			{
				return false;
			}

			target->ModValue(variable->type, variable->value, amount);
			return true;
		}
	}
	return false;
}

Float FlagNormalValues(Script * script)
{
	if (script->VerifyArguments(3) == true)
	{
		Word * targetWord = script->GetNextWord();
		Word * variable = script->GetNextWord();
		Word * value = script->GetNextWord();

		Object * target = script->GetTarget(targetWord);

		if ((target != NULL) && (IntervalCheck(variable->type, INT32_VAR, FLOAT_VAR) == true))
		{
			Float amount = (uint32)target->GetValue(variable->type, variable->value) ^ (uint32)value->value;
			target->SetValue(variable->type, variable->value, amount);
			return true;
		}
	}
	return false;
}