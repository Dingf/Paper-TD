#ifndef INC_SCRIPT_DEF_H
#define INC_SCRIPT_DEF_H

#include "scriptmathhandler.h"
#include "scriptforhandler.h"
#include "scriptgethandler.h"
#include "scriptmakehandler.h"
#include "scriptsethandler.h"
#include "scriptspellhandler.h"
#include "scripttowerhandler.h"
#include "scriptcreephandler.h"
#include "scriptgobjecthandler.h"
#include "scriptanimhandler.h"
#include "projectile.h"
#include "game.h"

enum KeywordTypes
{
	NUMERIC,
	INT32_VAR,
	UINT32_VAR,
	FLOAT_VAR,
	INT32_CONST,
	UINT32_CONST,
	FLOAT_CONST,
	SUBSCRIPT,
	VARIABLE,
	NORMAL,
	OPERATOR,
	POINTER,
	ACTION,
	TRIGGER,
};

enum ScriptKeywords
{
	KEYWORD_FLAT = 0,
	KEYWORD_PERCENT = 1,

	KEYWORD_END = -1,
	KEYWORD_IF = 1,
	KEYWORD_ELSE = 2,
	KEYWORD_FOR = 3,
	KEYWORD_EVERY = 4,
	KEYWORD_STOP = 5,
	KEYWORD_MAKE = 6,
	KEYWORD_SET = 7,
	KEYWORD_MOD = 8,
	KEYWORD_FLAG = 9,
	KEYWORD_GET = 10,
	KEYWORD_RAND = 11,
	KEYWORD_SELECT = 12,
	KEYWORD_DELETE = 13,
	KEYWORD_REFRESH = 14,
	KEYWORD_TRIGGER = 15,
	KEYWORD_CUSTOM = 16,

	KEYWORD_TARGET_SELF = 1,
	KEYWORD_TARGET_SELECTION = 2,
	KEYWORD_TARGET_TARGET = 3,
	KEYWORD_TARGET_PROJECTILE = 4,
	KEYWORD_TARGET_SOURCE = 5,
	KEYWORD_TARGET_THIS = 6,
	KEYWORD_TARGET_WORLDGAME = 7,

	KEYWORD_EQUALS = 100,
	KEYWORD_NOT_EQUALS = 101,
	KEYWORD_LESS_THAN = 102,
	KEYWORD_LESS_OR_EQUALS = 103,
	KEYWORD_GREATER_THAN = 104,
	KEYWORD_GREATER_OR_EQUALS = 105,
	KEYWORD_ADD = 106,
	KEYWORD_SUBTRACT = 107,
	KEYWORD_MULTIPLY = 108,
	KEYWORD_DIVIDE = 109,
	KEYWORD_INT_DIVIDE = 110,
	KEYWORD_MODULO = 111,
	KEYWORD_TRUNCATE = 112,
	KEYWORD_ROUND = 113,
	KEYWORD_CEILING = 114,
	KEYWORD_FLOOR = 115,
	KEYWORD_ASSIGN = 116,
	KEYWORD_AND = 117,
	KEYWORD_OR = 118,
	KEYWORD_NOT = 119,
	KEYWORD_XOR = 120,

	KEYWORD_SINE_R = 200,
	KEYWORD_SINE_D = 201,
	KEYWORD_COSINE_R = 202,
	KEYWORD_COSINE_D = 203,
	KEYWORD_TANGENT_R = 204,
	KEYWORD_TANGENT_D = 205,
	KEYWORD_ARCSINE_R = 206,
	KEYWORD_ARCSINE_D = 207,
	KEYWORD_ARCCOSINE_R = 208,
	KEYWORD_ARCCOSINE_D = 209,
	KEYWORD_ARCTANGENT_R = 210,
	KEYWORD_ARCTANGENT_D = 211,
	KEYWORD_LOG_2 = 212,
	KEYWORD_LOG_N = 213,
	KEYWORD_LOG_E = 214,
	KEYWORD_LOG_10 = 215,
	KEYWORD_EXP_2 = 216,
	KEYWORD_EXP_N = 217,
	KEYWORD_EXP_E = 218,
	KEYWORD_EXP_10 = 219,
	KEYWORD_ROOT_2 = 220,
	KEYWORD_ROOT_3 = 221,
	KEYWORD_MIN_FIT = 222,
	KEYWORD_MAX_FIT = 223,
	KEYWORD_INTERVAL_FIT = 224,
	KEYWORD_ABS_VAL = 225,
	KEYWORD_ZERO_ANGLE_D = 226,
	KEYWORD_ZERO_ANGLE_R = 227
};

int32 LookupWord(const char * word, bool type);

Float TakeNoAction(Script * script);
Float InvalidHandler(Script * script);
Float EndCommand(Script * script);
Float EvaluateElse(Script * script);
Float EvaluateIf(Script * script);
Float ForLoop(Script * script);
Float StopScript(Script * script);
Float SelectObject(Script * script);
Float AddTrigger(Script * script);

Float EveryTimedScript(Script * script);
Float GenerateRandomValue(Script * script);
Float DeleteObject(Script * script);
Float RefreshObject(Script * script);

Float ActivateCustomScript(Script * script);

#endif