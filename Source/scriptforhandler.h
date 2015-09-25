#ifndef INC_SCRIPT_FOR_HANDLER_H
#define INC_SCRIPT_FOR_HANDLER_H

#include "type.h"

enum ForKeywords
{
	FOR_KEYWORD_ALL,
};

enum ForAllKeywords
{
	FOR_ALL_KEYWORD_TOWERS,
	FOR_ALL_KEYWORD_PROJECTILES,
	FOR_ALL_KEYWORD_CREEPS,
	FOR_ALL_KEYWORD_GAME_OBJS,
	FOR_ALL_KEYWORD_ANIMATIONS,
	FOR_ALL_KEYWORD_EMITTERS,
	FOR_ALL_KEYWORD_KNOWN_SPELLS,
	FOR_ALL_KEYWORD_ACTIVE_SPELLS,
};

Float ForAllTowers(Script * script);
Float ForAllProjectiles(Script * script);
Float ForAllCreeps(Script * script);
Float ForAllGameObjects(Script * script);
Float ForAllAnimations(Script * script);
Float ForAllEmitters(Script * script);
Float ForAllKnownSpells(Script * script);
Float ForAllActiveSpells(Script * script);

#endif