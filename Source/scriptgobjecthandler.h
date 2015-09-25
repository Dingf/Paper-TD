#ifndef INC_SCRIPT_GOBJECT_HANDLER_H
#define INC_SCRIPT_GOBJECT_HANDLER_H

#include "type.h"
#include "gameobject.h"

enum GameObjectKeywords
{
	GOBJECT_KEYWORD_ROTATE,
	GOBJECT_KEYWORD_DAMAGE_AREA,
	GOBJECT_KEYWORD_SPELL_AREA,
};

enum SpellAreaKeywords
{
	SPELL_AREA_TOWERS,
	SPELL_AREA_PROJECTILES,
	SPELL_AREA_CREEPS,
};

Float RotateGameObject(Script * script);
Float DamageArea(Script * script);

Float SpellAreaTowers(Script * script);
Float SpellAreaProjectiles(Script * script);
Float SpellAreaCreeps(Script * script);

#endif