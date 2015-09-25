#include "type.h"
#include "script.h"
#include "scriptdef.h"

Float TakeNoAction(Script * script)
{
	return RETURN_CONTINUE;
}

Float InvalidHandler(Script * script)
{
	return false;
}

Float EndCommand(Script * script)
{
	return true;
}

Float EvaluateElse(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * subscriptID = script->GetNextOriginalWord();
		Script * subscript = script->GetSubscript(subscriptID->value);
		if (subscript != NULL)
		{
			return subscript->ActivateScript(false);
		}
	}
	return false;
}

Float EvaluateIf(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * term = script->GetNextWord();
		Word * subscriptID = script->GetNextOriginalWord();

		if ((bool)term->value == true)
		{
			Script * subscript = script->GetSubscript(subscriptID->value);
			if (subscript != NULL)
			{
				return subscript->ActivateScript(false);
			}
		}
		else
		{
			Word * nextWord = script->GetNextWord();
			if ((nextWord != NULL) && (nextWord->type == ACTION) && (nextWord->value == KEYWORD_ELSE))
			{
				return EvaluateElse(script);
			}
		}
	}
	return false;
}

Float ForLoop(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * amount = script->GetNextWord();
		Word * subscriptID = script->GetNextOriginalWord();
		Script * subscript = script->GetSubscript(subscriptID->value);

		if (subscript != NULL)
		{
			for (uint32 i = 0; i < (int32)amount->value; i++)
			{
				subscript->ActivateScript(false);
			}
		}
		return true;
	}
	return false;
}

Float StopScript(Script * script)
{
	return RETURN_STOP;
}

Float SelectObject(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * targetWord = script->GetNextWord();
		Object * target = script->GetTarget(targetWord);
	
		if (target != NULL)
		{
			script->SetSelection(target);
			return true;
		}
	}
	return false;
}

Float AddTrigger(Script * script)
{
	if (script->VerifyArguments(3) == true)
	{
		Word * targetWord = script->GetNextWord();
		Word * triggerWord = script->GetNextWord();
		Word * subscriptID = script->GetNextOriginalWord();

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

