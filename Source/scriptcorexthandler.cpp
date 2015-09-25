#include "type.h"
#include "rand.h"
#include "script.h"
#include "scriptdef.h"
#include "animation.h"
#include "gameobject.h"
#include "creep.h"
#include "projectile.h"
#include "tower.h"

Float EveryTimedScript(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * timeInterval = script->GetNextWord();
		Word * subscriptID = script->GetNextOriginalWord();
		Script * subscript = script->GetSubscript(subscriptID->value);

		uint32 valueIndex = NULL;
		Object * target = script->GetSource();
		if ((subscript != NULL) && (target != NULL) && ((target->CheckType(OBJ_TYPE_GAME_OBJECT) == true) || (target->CheckType(OBJ_TYPE_ANIMATION) == true)))
		{
			if (target->CheckType(OBJ_TYPE_GAME_OBJECT) == true)
			{
				valueIndex = GOBJECT_VAR_PERIODIC_TIME;
			}
			else if (target->CheckType(OBJ_TYPE_ANIMATION) == true)
			{
				valueIndex = ANIM_VAR_PERIODIC_TIME;
			}

			if (valueIndex != NULL)
			{
				while ((GetGameTime() - target->GetUInt32Value(valueIndex)) > timeInterval->value)
				{
					subscript->ActivateScript(false);
					if (target->GetUInt32Value(valueIndex) == 0)
					{
						target->SetUInt32Value(valueIndex, GetGameTime());
					}
					else
					{
						target->ModUInt32Value(valueIndex, timeInterval->value);
					}
				}
				return true;
			}
		}
	}
	return false;
}

Float GenerateRandomValue(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * minWord = script->GetNextWord();
		Word * maxWord = script->GetNextWord();

		return RandomRange(minWord->value, maxWord->value);
	}
	return false;
}

Float DeleteObject(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * targetWord = script->GetNextWord();
		Object * target = script->GetTarget(targetWord);

		if (target->CheckType(OBJ_TYPE_TOWER) == true)
		{
			Tower * tower = (Tower *)target;
			tower->Unload();
			return true;
		}
		else if (target->CheckType(OBJ_TYPE_PROJECTILE) == true)
		{
			Projectile * projectile = (Projectile *)target;
			projectile->Unload();
			return true;
		}
		else if (target->CheckType(OBJ_TYPE_CREEP) == true)
		{
			Creep * creep = (Creep *)target;
			creep->Unload();
			return true;
		}
		else if (target->CheckType(OBJ_TYPE_ANIMATION) == true)
		{
			Animation * animation = (Animation *)target;
			animation->Unload();
			return true;
		}
		else if (target->CheckType(OBJ_TYPE_GAME_OBJECT) == true)
		{
			GameObject * gameObject = (GameObject *)target;
			gameObject->Unload();
			return true;
		}
		else if (target->CheckType(OBJ_TYPE_SPELL) == true)
		{
			Spell * spell = (Spell *)target;
			spell->SetState(false);
			return true;
		}
	}
	return false;
}

Float RefreshObject(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * targetWord = script->GetNextWord();
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