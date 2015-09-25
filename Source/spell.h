#ifndef INC_SPELL_H
#define INC_SPELL_H

#include "type.h"
#include "object.h"
#include "script.h"

enum SpellInt32Variables
{
	SPELL_VAR_COST,
	SPELL_VAR_CAST_TIME,
	SPELL_VAR_COOLDOWN,
	SPELL_VAR_CHARGES,
	SPELL_VAR_DURATION,
	SPELL_VAR_STACKS,
	SPELL_VAR_ICON_ID,
	SPELL_VAR_HOTKEY,
	SPELL_VAR_LAST_CAST_TIME,
	SPELL_INT32_VARS
};

enum SpellUInt32Variables
{
	SPELL_VAR_ID,
	SPELL_VAR_VISIBILITY,
	SPELL_VAR_MODE,
	SPELL_VAR_LEVEL,
	SPELL_VAR_PREREQUISITE,
	SPELL_VAR_AUTOMATIC,
	SPELL_VAR_CATEGORY,
	SPELL_VAR_REMAINING_CHARGES,
	SPELL_UINT32_VARS
};

enum SpellModes
{
	SPELL_MODE_ACTIVE = 1,
	SPELL_MODE_PASSIVE = 2,
	SPELL_MODE_TRIGGERED = 3,
	SPELL_MODE_LEARN = 4,
};

class Spell : public Object
{
	public:
		Spell();
		~Spell();

		Spell * CreateInstance(Object * source, Object * target) const;
		Float Activate();

		uint32 GetID() const { return GetUInt32Value(SPELL_VAR_ID); }
		uint32 GetMode() const { return GetUInt32Value(SPELL_VAR_MODE); }
		uint32 GetLevel() const { return GetUInt32Value(SPELL_VAR_LEVEL); }
		uint32 GetCategory() const { return GetUInt32Value(SPELL_VAR_CATEGORY); }
		uint32 GetCastTime() const { return GetInt32Value(SPELL_VAR_CAST_TIME); }
		uint32 GetLastCastTime() const { return GetInt32Value(SPELL_VAR_LAST_CAST_TIME); }
		uint32 GetCooldown() const { return GetInt32Value(SPELL_VAR_COOLDOWN); }
		uint32 GetDuration() const { return GetInt32Value(SPELL_VAR_DURATION); }
		uint32 GetSpellCost() const { return GetInt32Value(SPELL_VAR_COST); }

		bool CheckCharges() const;
		bool CheckCooldown() const;

		const Object * GetSource() const { return source; }
			  Object * GetSource()       { return source; }
		const Object * GetTarget() const { return target; }
			  Object * GetTarget()	     { return target; }

		const char * GetText() const { return text; }

		//void SetSource(Object * source) { this->source = source; }

		void Unload();

		friend bool LoadSpellData();
	protected:
		char * text;
		Object * source;
		Object * target;
		Script script;
};

bool LoadSpellData();
void UnloadSpellData();

const Spell * GetSpellData();
const Spell * GetSpell(uint32 ID);
uint32 GetSpellDataSize();

#endif