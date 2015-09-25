#ifndef INC_SCRIPT_GET_HANDLER_H
#define INC_SCRIPT_GET_HANDLER_H

#include "type.h"

enum GetKeywords
{
	GET_KEYWORD_POSITION,
	GET_KEYWORD_X_VAL,
	GET_KEYWORD_Y_VAL,
	GET_KEYWORD_DISTANCE,
	GET_KEYWORD_INTERSECTION,
	GET_KEYWORD_STATE,
	GET_KEYWORD_OBJ_TYPE,
	GET_KEYWORD_GAME_TIME,
	GET_KEYWORD_SELECTED,
	GET_KEYWORD_WAVE,
	GET_KEYWORD_TEXT,
	GET_KEYWORD_CASTING,
	GET_KEYWORD_PATH,
	GET_KEYWORD_DAMAGE,
	GET_KEYWORD_RANGE,
	GET_KEYWORD_ATTACK_DELAY,
	GET_KEYWORD_ARMOR,
};

enum GetPositionKeywords
{
	GET_MOVE_TARGET,
	GET_LAST_POSITION,
	GET_TARGET_POSITION,
	GET_MOUSE,
};

enum GetIntersectionKeywords
{
	GET_INTERSECTION_POINT,
};

enum GetSelectionKeywords
{
	GET_SELECTION_SIZE,
};

enum ScriptGetWaveKeywords
{
	GET_WAVE_TIME,
	GET_WAVE_TYPE,
};

Float GetMoveTarget(Script * script);
Float GetLastPosition(Script * script);
Float GetTargetPosition(Script * script);
Float GetMousePosition(Script * script);

Float GetPointIntersection(Script * script);

Float GetSelectionSize(Script * script);

Float GetWaveTime(Script * script);
Float GetWaveType(Script * script);

Float GetObjectPosition(Script * script);
Float GetDistance(Script * script);
Float GetShapeIntersection(Script * script);
Float GetObjectState(Script * script);
Float GetObjectType(Script * script);
Float GetGameTime(Script * script);
Float GetSelectedIndex(Script * script);
Float GetObjectText(Script * script);
Float GetCastingState(Script * script);
Float GetGridPath(Script * script);
Float GetObjectDamage(Script * script);
Float GetTowerRange(Script * script);
Float GetTowerAttackDelay(Script * script);
Float GetCreepArmor(Script * script);

Float GetNormalValues(Script * script);

#endif