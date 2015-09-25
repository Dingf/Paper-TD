#ifndef INC_SCRIPT_SPELL_HANDLER_H
#define INC_SCRIPT_SPELL_HANDLER_H

#include "type.h"
#include "spell.h"

enum SpellKeywords
{
	SPELL_KEYWORD_LEARN_SPELL,
	SPELL_KEYWORD_FORGET_SPELL,
	SPELL_KEYWORD_APPLY_SPELL,
	SPELL_KEYWORD_REMOVE_SPELL,
	SPELL_KEYWORD_ACTIVE_SPELL,
	SPELL_KEYWORD_KNOWS_SPELL,
	SPELL_KEYWORD_SHOW_SPELL,
	SPELL_KEYWORD_CAST_SPELL,
	SPELL_KEYWORD_SPELL_COOLDOWN,
	SPELL_KEYWORD_SPELL_CHARGES,
	SPELL_KEYWORD_APPLY_AURA,
};

Float LearnSpell(Script * script);
Float ForgetSpell(Script * script);
Float ApplySpell(Script * script);
Float RemoveSpell(Script * script);
Float ActiveSpell(Script * script);
Float KnowsSpell(Script * script);
Float ShowSpell(Script * script);
Float CastSpell(Script * script);
Float CheckSpellCooldown(Script * script);
Float CheckSpellCharges(Script * script);
Float ApplyAura(Script * script);

#endif