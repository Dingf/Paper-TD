#include "type.h"
#include "script.h"
#include "scriptdef.h"
#include "gameobject.h"
#include "creep.h"
#include "tower.h"

Float DamageCreep(Script * script)
{
	if (script->VerifyArguments(4) == true)
	{
		Word * targetWord = script->GetNextWord();
		Word * sourceWord = script->GetNextWord();
		Word * damageWord = script->GetNextWord();
		Word * armorIgnore = script->GetNextWord();

		Creep * creep = (Creep *)script->GetTarget(targetWord);
		if ((creep != NULL) && (creep->CheckType(OBJ_TYPE_CREEP) == true) && (creep->GetState() == true))
		{
			if (creep->GetInt32Value(CREEP_VAR_SPELL_IMMUNE) == false)
			{
				creep->InflictDamage(damageWord->value, armorIgnore->value);
			}
			else
			{
				return false;
			}
			if (creep->GetState() == false)
			{
				Object * source = script->GetTarget(sourceWord);
				AddTowerKill(source);
			}
			return true;
		}
	}
	return false;
}