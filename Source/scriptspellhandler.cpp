#include "type.h"
#include "object.h"
#include "script.h"
#include "scriptdef.h"
#include "spell.h"
#include "spellobject.h"
#include "aura.h"
#include "game.h"

Float LearnSpell(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * spellWord = script->GetNextWord();

		SpellObject * target = (SpellObject *)script->GetSource();
		if ((target != NULL) && (target->CheckType(OBJ_TYPE_SPELL_OBJ) == true) && (spellWord->value > 0) && (spellWord->value < GetSpellDataSize()))
		{
			const Spell * spell = GetSpell(spellWord->value);

			if (target->LearnSpell(spellWord->value) == true)
			{
				if ((spell->GetMode() != SPELL_MODE_TRIGGERED) && (spell->GetMode() != SPELL_MODE_LEARN))
				{
					target->ApplySpell(spellWord->value);
				}
				return true;
			}
		}
	}
	return false;
}

Float ForgetSpell(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * spellWord = script->GetNextWord();

		SpellObject * target = (SpellObject *)script->GetSource();
		if ((target != NULL) && (target->CheckType(OBJ_TYPE_SPELL_OBJ) == true) && (spellWord->value > 0) && (spellWord->value < GetSpellDataSize()))
		{		
			return (target->ForgetSpell(GetSpell(spellWord->value)) == true) ? true : RETURN_CONTINUE;
		}
	}
	return false;
}

Float ApplySpell(Script * script)
{
	if (script->VerifyArguments(3) == true)
	{
		Word * spellWord = script->GetNextWord();
		Word * targetWord = script->GetNextWord();
		Word * sourceWord = script->GetNextWord();

		SpellObject * target = (SpellObject *)script->GetTarget(targetWord);
		SpellObject * source = (SpellObject *)script->GetTarget(sourceWord);
		if ((target != NULL) && (source != NULL) && (spellWord->value > 0) && (spellWord->value < GetSpellDataSize()))
		{
			if (target->ApplySpell(GetSpell(spellWord->value), source) == true)
			{
				return true;
			}
			else
			{
				return RETURN_CONTINUE;
			}
		}
	}
	return false;
}

Float RemoveSpell(Script * script)
{
	if (script->VerifyArguments(3) == true)
	{
		Word * spellWord = script->GetNextWord();
		Word * targetWord = script->GetNextWord();
		Word * sourceWord = script->GetNextWord();

		SpellObject * target = (SpellObject *)script->GetTarget(targetWord);
		SpellObject * source = (SpellObject *)script->GetTarget(sourceWord);
		if ((target != NULL) && (spellWord->value > 0) && (spellWord->value < GetSpellDataSize()))
		{
			return (target->RemoveSpell(GetSpell(spellWord->value), source) == true) ? true : RETURN_CONTINUE;
		}
	}
	return false;
}

Float ActiveSpell(Script * script)
{
	if (script->VerifyArguments(3) == true)
	{
		Word * spellWord = script->GetNextWord();
		Word * targetWord = script->GetNextWord();
		Word * sourceWord = script->GetNextWord();

		SpellObject * target = (SpellObject *)script->GetTarget(targetWord);
		SpellObject * source = (SpellObject *)script->GetTarget(sourceWord);
		if ((target != NULL) && (spellWord->value > 0) && (spellWord->value < GetSpellDataSize()))
		{
			return target->ActiveSpell(GetSpell(spellWord->value), source);
		}
	}
	return false;
}

Float KnowsSpell(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * spellWord = script->GetNextWord();
		Word * targetWord = script->GetNextWord();

		SpellObject * target = (SpellObject *)script->GetTarget(targetWord);
		if ((target != NULL) && (spellWord->value > 0) && (spellWord->value < GetSpellDataSize()))
		{
			return target->KnowsSpell(GetSpell(spellWord->value));
		}
	}
	return false;
}

Float ShowSpell(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * spellWord = script->GetNextWord();
		Word * stateWord = script->GetNextWord();

		SpellObject * target = (SpellObject *)script->GetSource();
		if ((target != NULL) && (target->CheckType(OBJ_TYPE_SPELL_OBJ) == true) && (spellWord->value > 0) && (spellWord->value < GetSpellDataSize()))
		{
			bool state = (bool)stateWord->value;
			return (target->SetSpellValue(GetSpell(spellWord->value), OBJ_VAL_UINT32, SPELL_VAR_VISIBILITY, state) == true) ? true : RETURN_CONTINUE;
		}
	}
	return false;
}

Float CastSpell(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * spellWord = script->GetNextWord();

		SpellObject * target = (SpellObject *)script->GetSource();
		if ((target != NULL) && (target->CheckType(OBJ_TYPE_SPELL_OBJ) == true) && (spellWord->value > 0) && (spellWord->value < GetSpellDataSize()))
		{
			return target->CastSpell(spellWord->value);
		}
	}
	return false;
}
//
//Float CheckCast(Script * script, uint32 index)
//{
//	if (VerifyArguments(script, index, 1) == true)
//	{
//		Word * targetWord = script->GetWord(index + 1);
//
//		SpellObject * target = (SpellObject *)script->GetTarget(targetWord);
//		if ((target != NULL) && (target->CheckType(OBJ_TYPE_SPELL_OBJ) == true))
//		{
//			return (target->GetCastTarget() != NULL);
//		}
//	}
//	return false;
//}
//
//Float ActionPurgeSpells(Script * script, uint32 index)
//{
//	if (VerifyArguments(script, index, 1) == true)
//	{
//		Word * targetWord = script->GetWord(index + 1);
//
//		SpellObject * target = (SpellObject *)script->GetTarget(targetWord);
//		if ((target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_SPELL_OBJ) == true))
//		{
//			target->ClearActiveSpells();
//			target->Clear();
//			return true;
//		}
//	}
//	return false;
//}

Float CheckSpellCooldown(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * spellWord = script->GetNextWord();

		SpellObject * source = (SpellObject *)script->GetSource();
		if ((source != NULL) && (source->GetState() == true) && (source->CheckType(OBJ_TYPE_SPELL_OBJ) == true) && (spellWord->value > 0) && (spellWord->value < GetSpellDataSize()))
		{
			List<Spell *> * KnownSpells = source->GetKnownSpells();

			KnownSpells->ToStart();
			for (uint32 i = 0; i < KnownSpells->GetSize(); i++)
			{
				Spell * spell = KnownSpells->GetCurrent();
				KnownSpells->ToNext();

				if (spell->GetID() == spellWord->value)
				{
					if (spell->CheckCooldown() == false)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

Float CheckSpellCharges(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * spellWord = script->GetNextWord();

		SpellObject * source = (SpellObject *)script->GetSource();
		if ((source != NULL) && (source->GetState() == true) && (source->CheckType(OBJ_TYPE_SPELL_OBJ) == true) && (spellWord->value > 0) && (spellWord->value < GetSpellDataSize()))
		{
			List<Spell *> * KnownSpells = source->GetKnownSpells();

			KnownSpells->ToStart();
			for (uint32 i = 0; i < KnownSpells->GetSize(); i++)
			{
				Spell * spell = KnownSpells->GetCurrent();
				KnownSpells->ToNext();

				if (spell->GetID() == spellWord->value)
				{
					if (spell->CheckCharges() == false)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

Float ApplyAura(Script * script)
{
	if (script->VerifyArguments(3) == true)
	{
		Word * spellWord = script->GetNextWord();
		Word * radius = script->GetNextWord();
		Word * animID = script->GetNextWord();

		SpellObject * source = (SpellObject *)script->GetSource();
		if ((source != NULL) && (source->GetState() == true) && (source->CheckType(OBJ_TYPE_SPELL_OBJ) == true) && (spellWord->value >= 0) && (spellWord->value < GetSpellDataSize()))
		{
			Spell * auraSpell = (Spell *)script->GetOriginalSource();
			if ((auraSpell != NULL) && (auraSpell->CheckType(OBJ_TYPE_SPELL) == true))
			{
				List<Aura *> * ActiveAuras = GetActiveAuras();

				ActiveAuras->ToStart();
				for (uint32 i = 0; i < ActiveAuras->GetSize(); i++)
				{
					Aura * aura = ActiveAuras->GetCurrent();
					ActiveAuras->ToNext();

					if ((aura->GetEffect()->GetID() == spellWord->value) && (aura->GetSource() == source))
					{
						return false;
					}
				}
				ActiveAuras->Append(new Aura(GetSpell(spellWord->value), GetSpell(auraSpell->GetID()), GetAnimation(animID->value), source, radius->value));

				//if (animID->value > 0)
				//{
				//	const Animation * animation = ;
				//	if (animation != NULL)
				//	{
				//		animation->AddAnimation(source);
				//	}
				//}

				RefreshActiveAuras();
			}
			return true;
		}
	}
	return false;
}
