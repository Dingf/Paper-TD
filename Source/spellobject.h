#ifndef INC_SPELL_OBJECT_H
#define INC_SPELL_OBJECT_H

#include "type.h"
#include "point.h"
#include "object.h"
#include "list.h"
#include "script.h"
#include "triggeredscript.h"
#include "spell.h"

class SpellObject : public Object
{
	public:
		SpellObject();
		~SpellObject();

		const Spell * GetCastTarget() const { return castTarget; }
		const Spell * GetActiveSpell() const { return activeSpell; }

			  List<TriggeredScript *> * GetTriggeredScripts()	    { return &triggeredScripts; }
		const List<TriggeredScript *> * GetTriggeredScripts() const { return &triggeredScripts; }
			  List<Spell *> * GetKnownSpells()		 { return &knownSpells; }
		const List<Spell *> * GetKnownSpells() const { return &knownSpells; }
			  List<Spell *> * GetActiveSpells()		  { return &activeSpells; }
		const List<Spell *> * GetActiveSpells() const { return &activeSpells; }

		void ActivateTriggeredScript(uint8 trigger);
		bool AddTriggeredScript(uint8 trigger, Script * script, Spell * source);
		bool RemoveTriggeredScript(uint8 trigger);

		bool ApplySpell(uint32 spellID);
		bool ApplySpell(const Spell * spell, Object * source);
		bool RemoveSpell(uint32 spellID);
		bool RemoveSpell(const Spell * spell, const Object * source);
		uint32 ActiveSpell(uint32 spellID) const;
		uint32 ActiveSpell(const Spell * spell, const Object * source) const;

		bool LearnSpell(uint32 spellID);
		bool LearnSpell(const Spell * spell, Object * source);
		bool ForgetSpell(uint32 spellID);
		bool ForgetSpell(const Spell * spell);
		uint32 KnowsSpell(uint32 spellID) const;
		uint32 KnowsSpell(const Spell * spell) const;

		bool SetSpellValue(uint32 spellID, uint8 type, uint32 valueIndex, Float value);
		bool SetSpellValue(const Spell * spell, uint8 type, uint32 valueIndex, Float value);

		bool ModSpellValue(uint32 spellID, uint8 type, uint32 valueIndex, Float value);
		bool ModSpellValue(const Spell * spell, uint8 type, uint32 valueIndex, Float value);

		bool CastSpell(uint32 spellID);
		bool CastSpell(Spell * spell);
		bool InterruptCast();

		void RefreshActiveSpells();
		void LearnAutomaticSpells();
		void SpellMaintenance();

		virtual void OnCast(Spell * spell);
		virtual void Clear() = 0;

		void ClearGameObjects();

		void ClearTriggeredScripts();
		void ClearActiveSpells();
		void ClearKnownSpells();

		void Copy(const SpellObject& obj);

		void Unload();
		
	protected:
		Spell * activeSpell;		//There can only be 1 active spell at a time
		Spell * castTarget;
		List<TriggeredScript *> triggeredScripts;
		List<Spell *> knownSpells;
		List<Spell *> activeSpells;
};

#endif