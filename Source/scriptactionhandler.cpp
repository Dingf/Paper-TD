/*#include "type.h"
#include "rand.h"
#include "utility.h"
#include "point.h"
#include "object.h"
#include "script.h"
#include "scripthandler.h"
#include "animation.h"
#include "gameobject.h"
#include "creep.h"
#include "projectile.h"
#include "tower.h"
#include "grid.h"
#include "game.h"

Point2D pointHelper;

Float ActionMakePoint2D(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * xWord = script->GetWord(index + 1);
		Word * yWord = script->GetWord(index + 2);

		pointHelper.SetValues(xWord->value, yWord->value);
		return (uint32)&pointHelper;
	}
	return false;
}

Float ActionGenerateRand(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * minWord = script->GetWord(index + 1);
		Word * maxWord = script->GetWord(index + 2);

		Float min = minWord->value;
		Float max = maxWord->value;

		return RandomRange(min, max);
	}
	return false;
}

Float ActionSetPosition(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 3) == true)
	{
		Word * targetWord = script->GetWord(index + 2);
		Word * posWord = script->GetWord(index + 3);

		Object * target = script->GetTarget(targetWord);
		Point2D * position = (Point2D *)(uint32)posWord->value;

		if ((target != NULL) && (position != NULL))
		{
			if (target->CheckType(OBJ_TYPE_CREEP) == true)
			{
				Creep * creep = (Creep *)target;
				Point2D targetPos(position->GetX(), position->GetY());

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
				target->SetPosition(Point2D(position->GetX(), position->GetY()));
				return true;
			}
		}
	}
	return false;
}

Float ActionSetProjectile(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 1) == true)
	{
		Word * projWord = script->GetWord(index + 2);
		
		Object * target = script->GetSource();
		if ((target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_TOWER) == true))
		{
			Tower * tower = (Tower *)target;
			Projectile * projectile = tower->GetProjectile();

			const Projectile * ProjectileData = GetProjectileData();

			uint32 index = projWord->value;
			if ((index < GetProjectileDataSize()) && (projectile->GetUInt32Value(PROJ_VAR_BASE_TYPE) != index))
			{
				projectile->SetState(false);
				tower->SetUInt32Value(TOWER_VAR_PROJECTILE_ID, index);
				
				return true;
			}
		}
	}
	return RETURN_CONTINUE;
}

Float ActionSetValue(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 4) == true)
	{
		Word * targetWord = script->GetWord(index + 1);
		Word * variable = script->GetWord(index + 2);
		Word * value = script->GetWord(index + 3);
		Word * type = script->GetWord(index + 4);

		Object * target = script->GetTarget(targetWord);
		Float amount;

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

Word SetTable[] =
{
	{KEYWORD_POSITION,				"POSITION",				NORMAL,				&ActionSetPosition			},
	{KEYWORD_LOOP_TYPE_PROJECTILE,	"PROJECTILE",			LOOP_TYPE,			&ActionSetProjectile		},
};

Float ActionSet(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 1) == true)
	{
		Word * target = script->GetWord(index + 1);

		uint16 listSize = sizeof(SetTable)/sizeof(Word);
		for (uint16 i = 0; i < listSize; i++)
		{
			if ((target->value == SetTable[i].value) && (target->type == SetTable[i].type))
			{
				return SetTable[i].handler(script, index);
			}
		}

		return ActionSetValue(script, index);
	}
	return false;
}

Float ActionMod(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 4) == true)
	{
		Word * targetWord = script->GetWord(index + 1);
		Word * variable = script->GetWord(index + 2);
		Word * value = script->GetWord(index + 3);
		Word * type = script->GetWord(index + 4);

		Object * target = script->GetTarget(targetWord);
		Float amount;

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

Float ActionFlag(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 3) == true)
	{
		Word * targetWord = script->GetWord(index + 1);
		Word * variable = script->GetWord(index + 2);
		Word * value = script->GetWord(index + 3);

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

Float ActionNewVariable(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 1) == true)
	{
		Word * varWord = script->GetWord(index + 1);

		if ((varWord != NULL) && (varWord->type == NUMERIC))
		{
			Word * variable = script->AddVariable(varWord->word);
			if (variable != NULL)
			{
				return RETURN_CONTINUE;
			}
		}
	}
	return false;
}

Float ActionSelect(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 1) == true)
	{
		Word * targetWord = script->GetWord(index + 1);
		Object * target = script->GetTarget(targetWord);
	
		if (target != NULL)
		{
			script->SetSelection(target);
			return true;
		}
	}
	return false;
}

Float ActionDelete(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 1) == true)
	{
		Word * targetWord = script->GetWord(index + 1);

		if (targetWord->type == POINTER)
		{
			Object * target = script->GetTarget(targetWord);

			if (target->CheckType(OBJ_TYPE_TOWER) == true)
			{
				Tower * tower = (Tower *)target;
				tower->SetState(false);
			}
			else if (target->CheckType(OBJ_TYPE_PROJECTILE) == true)
			{
				Projectile * projectile = (Projectile *)target;
				projectile->Unload();
			}
			else if (target->CheckType(OBJ_TYPE_CREEP) == true)
			{
				Creep * creep = (Creep *)target;
				creep->Unload();
			}
			else if (target->CheckType(OBJ_TYPE_ANIMATION) == true)
			{
				Animation * animation = (Animation *)target;
				animation->Unload();
			}
			else if (target->CheckType(OBJ_TYPE_GAME_OBJECT) == true)
			{
				GameObject * gameObject = (GameObject *)target;
				gameObject->Unload();
			}
		}
		return true;
	}
	return false;
}

Float ActionRefresh(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 1) == true)
	{
		Word * targetWord = script->GetWord(index + 1);
		SpellObject * target = (SpellObject *)script->GetTarget(targetWord);

		if ((target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_SPELL_OBJ) == true))
		{
			target->RefreshActiveSpells();
			if (target->CheckType(OBJ_TYPE_TOWER) == true)
			{
				Tower * tower = (Tower *)target;
				tower->RefreshUpgradeSpells();
			}
			return true;
		}
	}
	return false;
}

Float ActionForAllLoop(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * targetWord = script->GetWord(index + 1);
		Word * subscriptID = script->GetOriginalWord(index + 2);
		Script * subscript = script->GetSubscript(subscriptID->value);

		if ((targetWord->type == LOOP_TYPE) && (subscript != NULL))
		{
			bool state = false;

			if (targetWord->value == KEYWORD_LOOP_TYPE_TOWER)
			{
				Grid * grid = WorldGame.GetGrid();
				Tower ** TowerData = grid->GetData();

				for (uint32 i = 1; i < GRID_SIZE - 1; i++)
				{
					for (uint32 j = 1; j < GRID_SIZE - 1; j++)
					{
						Tower * tower = TowerData[(i*GRID_SIZE)+j];
						if ((tower != NULL) && (tower->GetState() == true))
						{
							subscript->SetSource(tower);
							Float result = subscript->ActivateScript(false);

							if (result == true)
							{
								state = true;
							}
							else if (result == RETURN_STOP)
							{
								return true;
							}
						}
					}
				}
			}
			else
			{
				List<Object *> * objData = (targetWord->value == KEYWORD_LOOP_TYPE_CREEP) ? 
										   (List<Object *> *)GetActiveCreeps() :
										   (List<Object *> *)GetActiveProjectiles();

			    objData->ToStart();
				for (uint32 i = 0; i < objData->GetSize(); i++)
				{
					Object * object = objData->GetCurrent();
					objData->ToNext();

					if ((object != NULL) && (object->GetState() == true))
					{
						subscript->SetSource(object);
						Float result = subscript->ActivateScript(false);

						if (result == true)
						{
							state = true;
						}
						else if (result == RETURN_STOP)
						{
							return true;
						}
					}
				}
			}

			//delete newScript;
			return state;
		}
	}
	return false;
}

Float ActionPeriodicScript(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * timeInterval = script->GetWord(index + 1);
		Word * subscriptID = script->GetOriginalWord(index + 2);
		Script * subscript = script->GetSubscript(subscriptID->value);

		GameObject * target = (GameObject *)script->GetOriginalSource();

		if ((subscript != NULL) && (target != NULL) && ((target->CheckType(OBJ_TYPE_GAME_OBJECT) == true) || (target->CheckType(OBJ_TYPE_ANIMATION) == true)))
		{
			if (target->CheckType(OBJ_TYPE_GAME_OBJECT) == true)
			{
				GameObject * gameObj = (GameObject *)target;
				while ((GetGameTime() - gameObj->GetUInt32Value(GOBJECT_VAR_PERIODIC_TIME)) > timeInterval->value)
				{
					subscript->ActivateScript(false);
					if (gameObj->GetUInt32Value(GOBJECT_VAR_PERIODIC_TIME) == 0)
					{
						gameObj->SetUInt32Value(GOBJECT_VAR_PERIODIC_TIME, GetGameTime());
					}
					else
					{
						gameObj->ModUInt32Value(GOBJECT_VAR_PERIODIC_TIME, timeInterval->value);
					}
				}
				return true;
			}
			else if (target->CheckType(OBJ_TYPE_ANIMATION) == true)
			{
				Animation * animation = (Animation *)target;
				while ((GetGameTime() - animation->GetUInt32Value(ANIM_VAR_PERIODIC_TIME)) > timeInterval->value)
				{
					subscript->ActivateScript(false);
					if (animation->GetUInt32Value(ANIM_VAR_PERIODIC_TIME) == 0)
					{
						animation->SetUInt32Value(ANIM_VAR_PERIODIC_TIME, GetGameTime());
					}
					else
					{
						animation->ModUInt32Value(ANIM_VAR_PERIODIC_TIME, timeInterval->value);
					}
				}
				return true;
			}
		}
	}
	return false;
}

Float ActionTrigger(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 3) == true)
	{
		Word * targetWord = script->GetWord(index + 1);
		Word * triggerWord = script->GetWord(index + 2);
		Word * subscriptID = script->GetOriginalWord(index + 3);

		Script * subscript = script->GetSubscript(subscriptID->value);
		SpellObject * source = (SpellObject *)script->GetSource();
		SpellObject * target = (SpellObject *)script->GetTarget(targetWord);

		if ((subscript != NULL) && (target != NULL) && (target->GetState() == true) && (triggerWord->type == TRIGGER))
		{
			Spell * spell = (Spell *)script->GetOriginalSource();
			if (spell->CheckType(OBJ_TYPE_SPELL) == true)
			{
				Script * newScript = new Script;
				newScript->LoadScript(subscript->GetScript(), source);

				target->AddTriggeredScript(triggerWord->value, newScript, spell);

				delete newScript;
				return false;
			}
		}
	}
	return false;
}

*/



