#ifndef INC_SCRIPT_SET_HANDLER_H
#define INC_SCRIPT_SET_HANDLER_H

#include "type.h"

enum SetKeywords
{
	SET_KEYWORD_POSITION,
	SET_KEYWORD_PROJECTILE,
	SET_KEYWORD_GOBJECT_VALUES,
};

Float SetPosition(Script * script);
Float SetProjectile(Script * script);
Float SetGameObjectValues(Script * script);

Float SetNormalValues(Script * script);
Float ModNormalValues(Script * script);
Float FlagNormalValues(Script * script);

#endif