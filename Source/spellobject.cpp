#include "type.h"
#include "object.h"
#include "list.h"
#include "gameobject.h"
#include "spell.h"
#include "spellobject.h"
#include "game.h"

SpellObject::SpellObject()
{
	castTarget = NULL;
	activeSpell = NULL;
}

SpellObject::~SpellObject()
{
	Unload();
}

void SpellObject::ActivateTriggeredScript(uint8 trigger)
{
	if ((trigger < MAX_SCRIPT_TRIGGERS) && (triggeredScripts.GetSize() > 0))
	{
		triggeredScripts.ToStart();
		for (uint32 i = 0; i < triggeredScripts.GetSize(); i++)
		{
			TriggeredScript * triggeredScript = triggeredScripts.GetCurrent();
			triggeredScripts.ToNext();

			if ((triggeredScript->GetTrigger() == trigger) && (triggeredScript->GetState() == true))
			{
				Spell * spell = triggeredScript->GetSource();
				SpellObject * spellSource = (SpellObject *)spell->GetSource();

				if ((spell->CheckCooldown() == true) && (spell->CheckCharges() == true) && (spellSource != NULL) && (spellSource->GetState() == true))
				{
					//We set cooldowns and charges beforehand because spells like Teleport and Mirage that duplicate
					//towers would otherwise have cleared cooldowns on the new tower.

					uint32 castTime = GetGameTime();

					spellSource->SetSpellValue(spell, OBJ_VAL_INT32, SPELL_VAR_LAST_CAST_TIME, castTime);
					spellSource->ModSpellValue(spell, OBJ_VAL_UINT32, SPELL_VAR_REMAINING_CHARGES, -1);

					Script * script = triggeredScript->GetScript();
					if (script->ActivateScript(true) == false)
					{
						spellSource->SetSpellValue(spell, OBJ_VAL_INT32, SPELL_VAR_LAST_CAST_TIME, 0);
						spellSource->ModSpellValue(spell, OBJ_VAL_UINT32, SPELL_VAR_REMAINING_CHARGES, 1);
					}
					else
					{
						List<Spell *> * ActiveSpells = spellSource->GetActiveSpells();

						ActiveSpells->ToStart();
						for (uint32 j = 0; j < ActiveSpells->GetSize(); j++)
						{
							Spell * categorySpell = ActiveSpells->GetCurrent();
							ActiveSpells->ToNext();

							if ((categorySpell->GetCategory() != 0) && (categorySpell->GetCategory() == spell->GetCategory()))
							{
								spellSource->SetSpellValue(categorySpell, OBJ_VAL_INT32, SPELL_VAR_LAST_CAST_TIME, castTime);
								spellSource->ModSpellValue(categorySpell, OBJ_VAL_UINT32, SPELL_VAR_REMAINING_CHARGES, -1);
							}
						}
						if (trigger != SPELL_OBJ_TRIGGER_ON_CAST)
						{
							spellSource->OnCast(spell);
						}
					}
				}
			}
		}
	}
}

bool SpellObject::AddTriggeredScript(uint8 trigger, Script * script, Spell * source)
{ 
	if (trigger < MAX_SCRIPT_TRIGGERS)
	{
		if (GetType() == NULL)	//Invalid objects or game cannot have more than 1 trigger
		{
			RemoveTriggeredScript(trigger);
		}
		else
		{
			triggeredScripts.ToStart();
			for (uint32 i = 0; i < triggeredScripts.GetSize(); i++)
			{
				TriggeredScript * triggeredScript = triggeredScripts.GetCurrent();
				triggeredScripts.ToNext();

				if ((triggeredScript->GetState() == true) && (triggeredScript->GetTrigger() == trigger) && (triggeredScript->GetSource()->GetID() == source->GetID()))
				{
					return false;
				}
			}
		}
		triggeredScripts.Append(new TriggeredScript(trigger, script, source));
		return true;
	}
	return false;
}

bool SpellObject::RemoveTriggeredScript(uint8 trigger)
{
	bool state = false;

	triggeredScripts.ToStart();
	for (uint32 i = 0; i < triggeredScripts.GetSize(); i++)
	{
		TriggeredScript * script = triggeredScripts.GetCurrent();
		triggeredScripts.ToNext();

		if (script->GetTrigger() == trigger)
		{
			script->SetState(false);
			state = true;
		}
	}
	return state;
}


bool SpellObject::ApplySpell(uint32 spellID)
{
	if (spellID < GetSpellDataSize())
	{
		return ApplySpell(GetSpell(spellID), this);
	}
	return false;
}

bool SpellObject::ApplySpell(const Spell * spell, Object * source)
{
	if (spell != NULL)
	{
		int32 stackAmount = spell->GetInt32Value(SPELL_VAR_STACKS);
		if ((stackAmount > 0) && (spell->GetMode() != SPELL_MODE_TRIGGERED))
		{
			uint32 currentStacks = 0;

			activeSpells.ToStart();
			for (uint32 i = 0; i < activeSpells.GetSize(); i++)
			{
				Spell * currentSpell = activeSpells.GetCurrent();
				activeSpells.ToNext();

				if ((currentSpell->GetState() == true) && (currentSpell->GetID() == spell->GetID()) || ((currentSpell->GetCategory() == spell->GetCategory()) && (spell->GetCategory() != 0)))
				{
					if (currentStacks < (stackAmount - 1))		//i.e. in a spell that stacks twice, there can only be one (2 - 1) other spell with the same ID in the activeSpells
					{
						currentStacks++;
					}
					else
					{
						return false;
					}
				}
			}
		}

		if ((spell->GetMode() == SPELL_MODE_TRIGGERED) || (spell->GetMode() == SPELL_MODE_LEARN))
		{
			Spell * spellInst = spell->CreateInstance(source, this);
			if (spellInst->Activate() == true)
			{
				Object * source = spellInst->GetSource();
				SpellObject * spellSource = (SpellObject *)source;

				uint32 castTime = GetGameTime();
				spellSource->SetSpellValue(spellInst, OBJ_VAL_INT32, SPELL_VAR_LAST_CAST_TIME, castTime);
				spellSource->ModSpellValue(spellInst, OBJ_VAL_UINT32, SPELL_VAR_REMAINING_CHARGES, -1);

				List<Spell *> * ActiveSpells = spellSource->GetActiveSpells();

				ActiveSpells->ToStart();
				for (uint32 i = 0; i < ActiveSpells->GetSize(); i++)
				{
					Spell * categorySpell = ActiveSpells->GetCurrent();
					ActiveSpells->ToNext();

					if ((categorySpell->GetCategory() != 0) && (categorySpell->GetCategory() == spellInst->GetCategory()))
					{
						spellSource->SetSpellValue(categorySpell, OBJ_VAL_INT32, SPELL_VAR_LAST_CAST_TIME, castTime);
						spellSource->ModSpellValue(categorySpell, OBJ_VAL_UINT32, SPELL_VAR_REMAINING_CHARGES, -1);
					}
				}
				delete spellInst;
				return true;
			}
			delete spellInst;
			return false;
		}
		else
		{
			Spell * spellInst = spell->CreateInstance(source, this);
			spellInst->SetInt32Value(SPELL_VAR_LAST_CAST_TIME, GetGameTime());

			if (spellInst->GetDuration() == 0)
			{
				activeSpells.Append(spellInst);
			}
			else
			{
				uint32 endTime = spellInst->GetLastCastTime() + spellInst->GetDuration();

				uint32 index = 0;

				activeSpells.ToStart();
				for (; index < activeSpells.GetSize(); index++)
				{
					Spell * currentSpell = activeSpells.GetCurrent();
					activeSpells.ToNext();

					if (((currentSpell->GetLastCastTime() + currentSpell->GetDuration()) >= endTime) || (currentSpell->GetDuration() == 0))
					{
						break;
					}
				}
				activeSpells.Insert(spellInst, index);
			}

			if ((spell->GetMode() == SPELL_MODE_ACTIVE) && (activeSpell == NULL))
			{
				Spell * newSpell = activeSpells.GetLast();
				activeSpell = newSpell;
			}
			RefreshActiveSpells();
		}
		return true;
	}
	return false;
}

bool SpellObject::RemoveSpell(uint32 spellID)
{
	if (spellID < activeSpells.GetSize())
	{
		activeSpells[spellID]->SetState(false);
		return true;
	}
	return false;
}

bool SpellObject::RemoveSpell(const Spell * spell, const Object * source)
{
	bool state = false;

	activeSpells.ToStart();
	for (uint32 i = 0; i < activeSpells.GetSize(); i++)
	{
		Spell * currentSpell = activeSpells.GetCurrent();
		activeSpells.ToNext();

		if ((currentSpell->GetID() == spell->GetID()) && ((currentSpell->GetSource() == source) || (source == NULL)))
		{
			if (currentSpell == activeSpell)
			{
				activeSpell = NULL;
			}
			currentSpell->SetState(false);
			state = true;
		}
	}
	return state;
}

uint32 SpellObject::ActiveSpell(uint32 spellID) const
{
	uint32 counter = 0;
	for (uint32 i = 0; i < activeSpells.GetSize(); i++)
	{
		Spell * currentSpell = activeSpells.GetData(i);

		if (currentSpell->GetID() == spellID)
		{
			counter++;
		}
	}
	return counter;
}

uint32 SpellObject::ActiveSpell(const Spell * spell, const Object * source) const
{
	uint32 counter = 0;
	for (uint32 i = 0; i < activeSpells.GetSize(); i++)
	{
		Spell * currentSpell = activeSpells.GetData(i);

		if ((currentSpell->GetID() == spell->GetID()) && ((currentSpell->GetSource() == source) || (source == NULL)))
		{
			counter++;
		}
	}
	return counter;
}

bool SpellObject::LearnSpell(uint32 spellID)
{
	if (spellID < GetSpellDataSize())
	{
		return LearnSpell(GetSpell(spellID), this);
	}
	return false;
}

bool SpellObject::LearnSpell(const Spell * spell, Object * source)
{
	knownSpells.ToStart();
	for (uint32 i = 0; i < knownSpells.GetSize(); i++)
	{
		Spell * currentSpell = knownSpells.GetCurrent();
		knownSpells.ToNext();

		if (currentSpell->GetID() == spell->GetID())		//Cannot learn same spell twice
		{
			return false;
		}
	}
	knownSpells.Append(spell->CreateInstance(source, this));
	return true;
}

bool SpellObject::ForgetSpell(uint32 spellID)
{
	if (spellID < knownSpells.GetSize())
	{
		return ForgetSpell(GetSpell(spellID));
	}
	return false;
}

bool SpellObject::ForgetSpell(const Spell * spell)
{
	knownSpells.ToStart();
	for (uint32 i = 0; i < knownSpells.GetSize(); i++)
	{
		Spell * currentSpell = knownSpells.GetCurrent();
		knownSpells.ToNext();

		if (currentSpell->GetID() == spell->GetID())
		{
			delete currentSpell;
			knownSpells.Remove(i);
			RemoveSpell(spell, this);
			return true;
		}
	}
	return false;
}

uint32 SpellObject::KnowsSpell(uint32 spellID) const
{
	uint32 counter = 0;
	for (uint32 i = 0; i < knownSpells.GetSize(); i++)
	{
		Spell * currentSpell = knownSpells.GetData(i);

		if (currentSpell->GetID() == spellID)
		{
			counter++;
		}
	}
	return counter;
}

uint32 SpellObject::KnowsSpell(const Spell * spell) const
{
	uint32 counter = 0;
	for (uint32 i = 0; i < knownSpells.GetSize(); i++)
	{
		Spell * currentSpell = knownSpells.GetData(i);

		if (currentSpell->GetID() == spell->GetID())
		{
			counter++;
		}
	}
	return counter;
}

bool SpellObject::SetSpellValue(uint32 spellID, uint8 type, uint32 valueIndex, Float value)
{
	if (spellID < GetSpellDataSize())
	{
		return SetSpellValue(GetSpell(spellID), type, valueIndex, value);
	}
	return false;
}

bool SpellObject::SetSpellValue(const Spell * spell, uint8 type, uint32 valueIndex, Float value)
{
	bool state = false;
	List<Spell *> * spellLists[] = { GetKnownSpells(), GetActiveSpells() };

	for (uint8 i = 0; i <= 1; i++)
	{
		spellLists[i]->ToStart();
		for (uint32 j = 0; j < spellLists[i]->GetSize(); j++)
		{
			Spell * listSpell = spellLists[i]->GetCurrent();
			spellLists[i]->ToNext();

			if (listSpell->GetID() == spell->GetID())
			{
				listSpell->SetValue(type, valueIndex, value);
				state = true;
			}
		}
	}
	return state;
}

bool SpellObject::ModSpellValue(uint32 spellID, uint8 type, uint32 valueIndex, Float value)
{
	if (spellID < GetSpellDataSize())
	{
		return SetSpellValue(GetSpell(spellID), type, valueIndex, value);
	}
	return false;
}

bool SpellObject::ModSpellValue(const Spell * spell, uint8 type, uint32 valueIndex, Float value)
{
	bool state = false;
	List<Spell *> * spellLists[] = { GetKnownSpells(), GetActiveSpells() };
	for (uint8 i = 0; i <= 1; i++)
	{
		spellLists[i]->ToStart();
		for (uint32 j = 0; j < spellLists[i]->GetSize(); j++)
		{
			Spell * listSpell = spellLists[i]->GetCurrent();
			spellLists[i]->ToNext();

			if (listSpell->GetID() == spell->GetID())
			{
				listSpell->ModValue(type, valueIndex, value);
				state = true;
			}
		}
	}
	return state;
}

//
//bool SpellObject::SetCooldown(uint32 index)
//{
//	if (index < GetSpellDataSize())
//	{
//		const Spell * SpellData = GetSpellData();
//		return SetCooldown(&SpellData[index]);
//	}
//	return false;
//}
//
//bool SpellObject::SetCooldown(const Spell * spell)
//{
//	bool state = false;
//	for (uint32 i = 0; i < knownSpells.size(); i++)
//	{
//		if (knownSpells[i]->GetID() == spell->GetID())
//		{
//			knownSpells[i]->SetInt32Value(SPELL_VAR_LAST_CAST_TIME, GetGameTime());
//			state = true;
//		}
//	}
//	for (uint32 i = 0; i < activeSpells.size(); i++)
//	{
//		if (activeSpells[i]->GetID() == spell->GetID())
//		{
//			activeSpells[i]->SetInt32Value(SPELL_VAR_LAST_CAST_TIME, GetGameTime());
//			state = true;
//		}
//	}
//	return state;
//}
//
//bool SpellObject::ClearCooldown(uint32 index)
//{
//	if (index < GetSpellDataSize())
//	{
//		const Spell * SpellData = GetSpellData();
//		return ClearCooldown(&SpellData[index]);
//	}
//	return false;
//}
//
//bool SpellObject::ClearCooldown(const Spell * spell)
//{
//	bool state = false;
//	for (uint32 i = 0; i < knownSpells.size(); i++)
//	{
//		if (knownSpells[i]->GetID() == spell->GetID())
//		{
//			knownSpells[i]->SetInt32Value(SPELL_VAR_LAST_CAST_TIME, 0);
//			state = true;
//		}
//	}
//	for (uint32 i = 0; i < activeSpells.size(); i++)
//	{
//		if (activeSpells[i]->GetID() == spell->GetID())
//		{
//			activeSpells[i]->SetInt32Value(SPELL_VAR_LAST_CAST_TIME, 0);
//			state = true;
//		}
//	}
//	return state;
//}
//
//bool SpellObject::ModCharges(uint32 index, int32 amount)
//{
//	if (index < GetSpellDataSize())
//	{
//		const Spell * SpellData = GetSpellData();
//		return ModCharges(&SpellData[index], amount);
//	}
//	return false;
//}
//
//bool SpellObject::ModCharges(const Spell * spell, int32 amount)
//{
//	bool state = false;
//	for (uint32 i = 0; i < knownSpells.size(); i++)
//	{
//		if (knownSpells[i]->GetID() == spell->GetID())
//		{
//			knownSpells[i]->ModUInt32Value(SPELL_VAR_REMAINING_CHARGES, amount);
//			state = true;
//		}
//	}
//	for (uint32 i = 0; i < activeSpells.size(); i++)
//	{
//		if (activeSpells[i]->GetID() == spell->GetID())
//		{
//			activeSpells[i]->ModUInt32Value(SPELL_VAR_REMAINING_CHARGES, amount);
//			state = true;
//		}
//	}
//	return state;
//}
//
//bool SpellObject::ShowSpell(uint32 index, bool visibility)
//{
//	if (index < GetSpellDataSize())
//	{
//		const Spell * SpellData = GetSpellData();
//		return ShowSpell(&SpellData[index], visibility);
//	}
//	return false;
//}
//
//bool SpellObject::ShowSpell(const Spell * spell, bool visibility)
//{
//	bool state = false;
//	for (uint32 i = 0; i < knownSpells.size(); i++)
//	{
//		if (knownSpells[i]->GetID() == spell->GetID())
//		{
//			knownSpells[i]->SetUInt32Value(SPELL_VAR_VISIBILITY, visibility);
//			RemoveSpell(spell, this);
//			state = true;
//		}
//	}
//	return state;
//}

bool SpellObject::CastSpell(uint32 spellID)
{
	if (KnowsSpell(spellID) > 0)
	{
		knownSpells.ToStart();
		for (uint32 i = 0; i < knownSpells.GetSize(); i++)
		{
			Spell * spell = knownSpells.GetCurrent();
			knownSpells.ToNext();

			if (spell->GetID() == spellID)
			{
				return CastSpell(spell);
			}
		}
	}
	/*if (ActiveSpell(spellID) == true)
	{
		for (uint32 i = 0; i < activeSpells.GetSize(); i++)
		{
			if (activeSpells[i]->GetID() == spellID)
			{
				return CastSpell(activeSpells[i]);
			}
		}
	}*/
	return false;
}

bool SpellObject::CastSpell(Spell * spell)
{
	if (WorldGame.GetInk() > spell->GetSpellCost())
	{
		if (spell->GetSpellCost() != 0)
		{
			WorldGame.AddInk(-1.0 * (int32)spell->GetSpellCost());
		}

		if ((spell->GetCastTime() > 0) && (WorldGame.GetUInt32Value(GAME_VAR_INSTANT_CAST_FLAG) == false))
		{
			spell->SetInt32Value(SPELL_VAR_LAST_CAST_TIME, GetGameTime());
			castTarget = spell;
			return true;
		}
		else
		{
			if (spell->GetMode() == SPELL_MODE_LEARN)
			{
				LearnAutomaticSpells();
				LearnSpell(spell, this);
			}
			else if (spell->GetMode() == SPELL_MODE_ACTIVE)
			{
				activeSpell = NULL;
			}

			if (ApplySpell(spell, this) == true)
			{
				OnCast(spell);
				return true;
			}
		}
	}
	return false;
}

bool SpellObject::InterruptCast()
{
	if (castTarget != NULL)
	{
		castTarget->SetInt32Value(SPELL_VAR_LAST_CAST_TIME, 0);
		castTarget = NULL;
		return true;
	}
	return false;
}

void SpellObject::RefreshActiveSpells()
{
	Clear();
	if (activeSpells.GetSize() > 0)
	{
		List<Spell *> currentSpells;

		currentSpells.Copy(activeSpells);

		currentSpells.ToStart();
		for (uint32 i = 0; i < currentSpells.GetSize(); i++)
		{
			Spell * spell = currentSpells.GetCurrent();
			currentSpells.ToNext();

			if ((spell != NULL) && (spell->GetState() == true) && (spell != activeSpell))
			{
				spell->Activate();
			}
		}
		if (activeSpell != NULL)
		{
			activeSpell->Activate();
		}
		currentSpells.Unload();
	}
}

void SpellObject::LearnAutomaticSpells()
{
	const Spell * SpellData = GetSpellData();
	const Spell * oldActiveSpell = GetActiveSpell();

	List<const Spell *> newSpells;
	List<const Spell *> oldSpells;

	for (uint32 i = 0; i < GetSpellDataSize(); i++)
	{
		const Spell * baseSpell = &SpellData[i];
		if ((baseSpell->GetUInt32Value(SPELL_VAR_AUTOMATIC) == true) && (KnowsSpell(baseSpell->GetUInt32Value(SPELL_VAR_PREREQUISITE)) > 0))//CheckSpellPrerequisites(SpellData[i].GetUInt32Value(SPELL_VAR_PREREQUISITE)) == true))
		{
			if (KnowsSpell(baseSpell) == false)
			{
				newSpells.Append(baseSpell);
				oldSpells.Append(&SpellData[baseSpell->GetUInt32Value(SPELL_VAR_PREREQUISITE)]);
			}
		}
	}

	newSpells.ToStart();
	for (uint32 i = 0; i < newSpells.GetSize(); i++)
	{
		const Spell * newSpell = newSpells.GetCurrent();
		newSpells.ToNext();

		LearnSpell(newSpell, this);
		if ((newSpell->GetMode() != SPELL_MODE_TRIGGERED) && (ApplySpell(newSpell, this) == true) && (newSpell->GetMode() == SPELL_MODE_ACTIVE))
		{
			if ((oldActiveSpell == NULL) || (newSpell->GetUInt32Value(SPELL_VAR_PREREQUISITE) == oldActiveSpell->GetID()))
			{
				List<Spell *> * ActiveSpells = GetActiveSpells();
				activeSpell = ActiveSpells->GetLast();
			}
		}
	}

	oldSpells.ToStart();
	for (uint32 i = 0; i < oldSpells.GetSize(); i++)
	{
		const Spell * oldSpell = oldSpells.GetCurrent();
		oldSpells.ToNext();

		ForgetSpell(oldSpell);
	}
}

void SpellObject::SpellMaintenance()
{
	bool refreshFlag = false;
	uint32 maintenanceTime = GetGameTime();

	if (castTarget != NULL)
	{
		int32 timeDif = maintenanceTime - castTarget->GetLastCastTime();
		if (timeDif >= castTarget->GetCastTime())
		{
			if (castTarget->GetMode() == SPELL_MODE_LEARN)
			{
				LearnAutomaticSpells();
				LearnSpell(castTarget, this);
			}
			else if (castTarget->GetMode() == SPELL_MODE_ACTIVE)
			{
				activeSpell = NULL;
			}

			if (ApplySpell(castTarget, this) == true)
			{
				OnCast(castTarget);
			}
			castTarget = NULL;
		}
	}

	triggeredScripts.ToStart();
	for (uint32 i = 0; i < triggeredScripts.GetSize(); i++)
	{
		TriggeredScript * script = triggeredScripts.GetCurrent();
		triggeredScripts.ToNext();

		Spell * spell = triggeredScripts[i]->GetSource();
		SpellObject * spellSource = (SpellObject *)spell->GetSource();

		if ((spellSource->GetState() == false) || (script->GetState() == false))
		{
			delete script;
			triggeredScripts.Remove(i);
			i--;
		}
	}

	knownSpells.ToStart();
	for (uint32 i = 0; i < knownSpells.GetSize(); i++)
	{
		Spell * spell = knownSpells.GetCurrent();
		Object * source = spell->GetSource();
		knownSpells.ToNext();

		if (spell != NULL)
		{
			if (spell->GetState() == false)
			{
				delete spell;
				knownSpells.Remove(i);
				i--;
			}
			else
			{
				if ((spell->GetUInt32Value(SPELL_VAR_VISIBILITY) == false) && (ActiveSpell(spell, source) > 0))
				{
					RemoveSpell(spell, source);
				}
				else if ((spell->GetUInt32Value(SPELL_VAR_VISIBILITY) == true) && (spell->GetMode() == SPELL_MODE_PASSIVE) && (ActiveSpell(spell, source) == false))
				{
					ApplySpell(spell, source);
				}
			}
		}
	}

	if (activeSpell == NULL)
	{
		activeSpells.ToStart();
		for (uint32 i = 0; i < activeSpells.GetSize(); i++)
		{
			Spell * spell = activeSpells.GetCurrent();
			activeSpells.ToNext();
			if ((spell != NULL) && (spell->GetMode() == SPELL_MODE_ACTIVE))
			{
				activeSpell = spell;
				break;
			}
		}
	}
	else
	{
		activeSpells.ToStart();
		for (uint32 i = 0; i < activeSpells.GetSize(); i++)
		{
			Spell * spell = activeSpells.GetCurrent();
			activeSpells.ToNext();
			if ((spell->GetMode() == SPELL_MODE_ACTIVE) && (spell != activeSpell))
			{
				RemoveSpell(i);
			}
		}
	}

	activeSpells.ToStart();
	for (uint32 i = 0; i < activeSpells.GetSize(); i++)
	{
		Spell * spell = activeSpells.GetCurrent();
		activeSpells.ToNext();

		if (spell != NULL)
		{
			if ((spell->GetDuration() != 0) && (maintenanceTime > (spell->GetDuration() + spell->GetLastCastTime())))
			{
				RemoveSpell(i);
			}
			else
			{
				break;
			}
		}
	}

	activeSpells.ToStart();
	for (uint32 i = 0; i < activeSpells.GetSize(); i++)
	{
		Spell * spell = activeSpells.GetCurrent();
		activeSpells.ToNext();

		spell = activeSpells[i];
		Object * source = spell->GetSource();

		if (((spell != NULL) && (spell->GetState() == false)) || (((source == NULL) || (source->GetState() == false)) && (spell->GetDuration() == 0)))
		{
			if (spell == activeSpell)
			{
				activeSpell = NULL;
			}
			delete spell;
			activeSpells.Remove(i);
			i--;
			refreshFlag = true;
		}
	}

	if (refreshFlag == true)
	{
		RefreshActiveSpells();
	}
}

void SpellObject::OnCast(Spell * spell)
{
	ActivateTriggeredScript(SPELL_OBJ_TRIGGER_ON_CAST);
}

void SpellObject::ClearGameObjects()
{
	List<GameObject *> * ActiveGameObjects = GetActiveGameObjects();

	ActiveGameObjects->ToStart();
	for (uint32 i = 0; i < ActiveGameObjects->GetSize(); i++)
	{
		GameObject * gameObj = ActiveGameObjects->GetCurrent();
		ActiveGameObjects->ToNext();

		if ((gameObj != NULL) && (gameObj->GetState() == true) && (gameObj->GetInt32Value(GOBJECT_VAR_DURATION) == 0) && (gameObj->GetSource() == this))
		{
			gameObj->SetState(false);
		}
	}
}

void SpellObject::ClearTriggeredScripts()
{
	while (triggeredScripts.GetSize() > 0)
	{
		delete triggeredScripts[0];
		triggeredScripts.Remove(0);
	}
}

void SpellObject::ClearActiveSpells()
{
	while (activeSpells.GetSize() > 0)
	{
		if (activeSpells[0] != NULL)
		{
			delete activeSpells[0];
		}
		activeSpells.Remove(0);
	}
}

void SpellObject::ClearKnownSpells()
{
	while (knownSpells.GetSize() > 0)
	{
		if (knownSpells[0] != NULL)
		{
			delete knownSpells[0];
		}
		knownSpells.Remove(0);
	}
}

void SpellObject::Copy(const SpellObject& obj)
{
	Object::Copy(obj);
	castTarget = NULL;

	ClearTriggeredScripts();
	const List<TriggeredScript *> * objTriggeredScripts = obj.GetTriggeredScripts();
	uint32 x = objTriggeredScripts->GetSize();
	for (uint32 i = 0; i < objTriggeredScripts->GetSize(); i++)
	{
		TriggeredScript * script = objTriggeredScripts->GetData(i);

		if (script->GetState() == true)
		{
			triggeredScripts.Append(new TriggeredScript(*script));
		}
	}

	ClearKnownSpells();
	const List<Spell *> * objKnownSpells = obj.GetKnownSpells();
	for (uint32 i = 0; i < objKnownSpells->GetSize(); i++)
	{
		Spell * spell = objKnownSpells->GetData(i);
		Object * source = (spell->GetSource() == &obj) ? this : spell->GetSource();

		if ((spell->GetState() == true) && (source != NULL) && (source->GetState() == true))
		{
			LearnSpell(spell, source);
		}
	}

	ClearActiveSpells();
	const List<Spell *> * objActiveSpells = obj.GetActiveSpells();
	for (uint32 i = 0; i < objActiveSpells->GetSize(); i++)
	{
		Spell * spell = objActiveSpells->GetData(i);
		Object * source = (spell->GetSource() == &obj) ? this : spell->GetSource();
		if ((spell->GetState() == true) && (source != NULL) && (source->GetState() == true))
		{
			ApplySpell(spell, source);
		}
	}
}

void SpellObject::Unload()
{
	castTarget = NULL;
	ClearTriggeredScripts();
	ClearActiveSpells();
	ClearKnownSpells();
}