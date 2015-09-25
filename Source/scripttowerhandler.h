#ifndef INC_SCRIPT_TOWER_HANDLER_H
#define INC_SCRIPT_TOWER_HANDLER_H

#include "type.h"
#include "tower.h"

enum TowerKeywords
{
	TOWER_KEYWORD_FIRE_TOWER,
	TOWER_KEYWORD_BUILD_TOWER,
};

Float FireTower(Script * script);
Float BuildTower(Script * script);

#endif