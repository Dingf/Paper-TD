#ifndef INC_TRIGGERED_SCRIPT_H
#define INC_TRIGGERED_SCRIPT_H

#include "type.h"
#include "script.h"
#include "spell.h"

enum ScriptTriggers
{
	SPELL_OBJ_TRIGGER_ON_CAST,
	PROJ_TRIGGER_ON_HIT,
	PROJ_TRIGGER_ON_EXPLODE,
	TOWER_TRIGGER_ON_FIRE,
	TOWER_TRIGGER_ON_UPGRADE,
	TOWER_TRIGGER_ON_SELL,
	TOWER_TRIGGER_ON_BUILD,
	TOWER_TRIGGER_ON_KILL,
	CREEP_TRIGGER_ON_DEATH,
	GAME_TRIGGER_ON_CLICK,
	MAX_SCRIPT_TRIGGERS
};

struct TriggeredScript
{
	public:
		TriggeredScript(uint8 trigger, Script * script, Spell * source)
		{
			state = true;
			this->trigger = trigger;
			this->script = new Script;
			this->script->Copy(*script);
			this->source = source->CreateInstance(source->GetSource(), source->GetTarget());
		}
		TriggeredScript(const TriggeredScript& triggeredScript)
		{
			state = true;
			trigger = triggeredScript.trigger;
			script = new Script;
			script->Copy(*triggeredScript.script);
			source = triggeredScript.source->CreateInstance(triggeredScript.source->GetSource(), triggeredScript.source->GetTarget());
		}
		~TriggeredScript()
		{
			state = false;
			script->Unload();
			delete script;
			delete source;
		}

		bool GetState() const { return state; }
		uint8 GetTrigger() const { return trigger; }

		void SetState(bool state) { this->state = state; }

			  Script * GetScript()       { return script; }
		const Script * GetScript() const { return script; }
			  Spell * GetSource()       { return source; }
		const Spell * GetSource() const { return source; }
	private:
		bool state;
		uint8 trigger;
		Script * script;
		Spell * source;
};

#endif