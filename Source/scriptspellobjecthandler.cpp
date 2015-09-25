#include "type.h"
#include "utility.h"
#include "object.h"
#include "script.h"
#include "scripthandler.h"
#include "spell.h"
#include "spellobject.h"

Float ActionTriggerAmount(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * triggerWord = script->GetWord(index + 1);
		Word * amount = script->GetWord(index + 2);

		SpellObject * target = (SpellObject *)script->GetSource();
		if ((target != NULL) && (target->CheckType(OBJ_TYPE_SPELL_OBJ) == true) && (triggerWord->type == TRIGGER))
		{
			return target->SetTriggerAmount(triggerWord->value, ZeroFit(amount->value));
		}	
	}
	return false;
}