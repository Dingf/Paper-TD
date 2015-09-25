#include "type.h"
#include "script.h"
#include "scriptdef.h"

static Word ForAllList[] = 
{
	{"TOWERS",							NORMAL,				FOR_ALL_KEYWORD_TOWERS,				&ForAllTowers,					NULL					},
	{"PROJECTILES",						NORMAL,				FOR_ALL_KEYWORD_PROJECTILES,		&ForAllProjectiles,				NULL					},
	{"CREEPS",							NORMAL,				FOR_ALL_KEYWORD_CREEPS,				&ForAllCreeps,					NULL					},
	{"GAME_OBEJCTS",					NORMAL,				FOR_ALL_KEYWORD_GAME_OBJS,			&ForAllGameObjects,				NULL					},
	{"ANIMATIONS",						NORMAL,				FOR_ALL_KEYWORD_ANIMATIONS,			&ForAllAnimations,				NULL					},
	{"EMITTERS",						NORMAL,				FOR_ALL_KEYWORD_EMITTERS,			&ForAllEmitters,				NULL					},
	{"KNOWN_SPELLS",					NORMAL,				FOR_ALL_KEYWORD_KNOWN_SPELLS,		&ForAllKnownSpells,				NULL					},
	{"ACTIVE_SPELLS",					NORMAL,				FOR_ALL_KEYWORD_ACTIVE_SPELLS,		&ForAllActiveSpells,			NULL					},
	{NULL,								NULL,				NULL,								NULL,							NULL					},
};

static Word ForList[] = 
{
	{"ALL",								NORMAL,				FOR_KEYWORD_ALL,					&InvalidHandler,				ForAllList				},
	{NULL,								NULL,				NULL,								NULL,							NULL					},
};

static Word MakeList[] =
{
	{"POINT2D",							NORMAL,				MAKE_KEYWORD_POINT2D,				&MakePoint2D,					NULL					},
	{"RECT",							NORMAL,				MAKE_KEYWORD_RECT,					&MakeRect,						NULL					},
	{"VARIABLE",						NORMAL,				MAKE_KEYWORD_VARIABLE,				&MakeNewVariable,				NULL					},
	{"ANIMATION",						NORMAL,				MAKE_KEYWORD_ANIMATION,				&MakeNewAnimation,				NULL					},
	{"GAME_OBJECT",						NORMAL,				MAKE_KEYWORD_GAME_OBJECT,			&MakeNewGameObject,				NULL					},
	{"EMITTER",							NORMAL,				MAKE_KEYWORD_EMITTER,				&MakeNewParticleEmitter,		NULL					},
	{NULL,								NULL,				NULL,								NULL,							NULL					},
};

static Word SetList[] = 
{
	{"POSITION",						NORMAL,				SET_KEYWORD_POSITION,				&SetPosition,					NULL					},
	{"PROJECTILE",						NORMAL,				SET_KEYWORD_PROJECTILE,				&SetProjectile,					NULL					},
	{"VALUES",							NORMAL,				SET_KEYWORD_GOBJECT_VALUES,			&SetGameObjectValues,			NULL					},
	{NULL,								NULL,				NULL,								NULL,							NULL					},
};

static Word GetPositionList[] =
{
	{"MOVE_TARGET",						NORMAL,				GET_MOVE_TARGET,					&GetMoveTarget,					NULL					},
	{"LAST_POS",						NORMAL,				GET_LAST_POSITION,					&GetLastPosition,				NULL					},
	{"TARGET_POS",						NORMAL,				GET_TARGET_POSITION,				&GetTargetPosition,				NULL					},
	{"MOUSE",							NORMAL,				GET_MOUSE,							&GetMousePosition,				NULL					},
	{NULL,								NULL,				NULL,								NULL,							NULL					},
};

static Word GetIntersectionList[] =
{
	{"POINT",							NORMAL,				GET_INTERSECTION_POINT,				&GetPointIntersection,			NULL					},
	{NULL,								NULL,				NULL,								NULL,							NULL					},
};

static Word GetSelectedList[] = 
{
	{"SIZE",							NORMAL,				GET_SELECTION_SIZE,					&GetSelectionSize,				NULL					},
	{NULL,								NULL,				NULL,								NULL,							NULL					},
};

static Word GetWaveList[] = 
{
	{"TIME",							NORMAL,				GET_WAVE_TIME,						&GetWaveTime,					NULL					},
	{"TYPE",							NORMAL,				GET_WAVE_TYPE,						&GetWaveType,					NULL					},
	{NULL,								NULL,				NULL,								NULL,							NULL					},
};

static Word GetList[] = 
{
	{"POSITION",						NORMAL,				GET_KEYWORD_POSITION,				&GetObjectPosition,				GetPositionList			},
	{"DISTANCE",						NORMAL,				GET_KEYWORD_DISTANCE,				&GetDistance,					NULL					},
	{"INTERSECTION",					NORMAL,				GET_KEYWORD_INTERSECTION,			&GetShapeIntersection,			GetIntersectionList		},
	{"STATE",							NORMAL,				GET_KEYWORD_STATE,					&GetObjectState,				NULL					},
	{"OBJ_TYPE",						NORMAL,				GET_KEYWORD_OBJ_TYPE,				&GetObjectType,					NULL					},
	{"GAME_TIME",						NORMAL,				GET_KEYWORD_GAME_TIME,				&GetGameTime,					NULL					},
	{"SELECTED",						NORMAL,				GET_KEYWORD_SELECTED,				&GetSelectedIndex,				GetSelectedList			},
	{"WAVE",							NORMAL,				GET_KEYWORD_WAVE,					&InvalidHandler,				GetWaveList				},
	{"TEXT",							NORMAL,				GET_KEYWORD_TEXT,					&GetObjectText,					NULL					},
	{"CASTING",							NORMAL,				GET_KEYWORD_CASTING,				&GetCastingState,				NULL					},
	{"PATH",							NORMAL,				GET_KEYWORD_PATH,					&GetGridPath,					NULL					},
	{"DAMAGE",							NORMAL,				GET_KEYWORD_DAMAGE,					&GetObjectDamage,				NULL					},
	{"RANGE",							NORMAL,				GET_KEYWORD_RANGE,					&GetTowerRange,					NULL					},
	{"ATTACK_DELAY",					NORMAL,				GET_KEYWORD_ATTACK_DELAY,			&GetTowerAttackDelay,			NULL					},
	{"ARMOR",							NORMAL,				GET_KEYWORD_ARMOR,					&GetCreepArmor,					NULL					},
	{NULL,								NULL,				NULL,								NULL,							NULL					},
};

static Word GetGlobalList[] =
{
	{"X_VAL",							NORMAL,				GET_KEYWORD_X_VAL,					&TakeNoAction,					NULL					},
	{"Y_VAL",							NORMAL,				GET_KEYWORD_Y_VAL,					&TakeNoAction,					NULL					},
	{NULL,								NULL,				NULL,								NULL,							NULL					},
};

static Word TriggerList[] =
{
	{"ON_CAST",							TRIGGER,			SPELL_OBJ_TRIGGER_ON_CAST,			&TakeNoAction,					NULL					},
	{"ON_HIT",							TRIGGER,			PROJ_TRIGGER_ON_HIT,				&TakeNoAction,					NULL					},
	{"ON_EXPLODE",						TRIGGER,			PROJ_TRIGGER_ON_EXPLODE,			&TakeNoAction,					NULL					},
	{"ON_FIRE",							TRIGGER,			TOWER_TRIGGER_ON_FIRE,				&TakeNoAction,					NULL					},
	{"ON_UPGRADE",						TRIGGER,			TOWER_TRIGGER_ON_UPGRADE,			&TakeNoAction,					NULL					},
	{"ON_SELL",							TRIGGER,			TOWER_TRIGGER_ON_SELL,				&TakeNoAction,					NULL					},
	{"ON_BUILD",						TRIGGER,			TOWER_TRIGGER_ON_BUILD,				&TakeNoAction,					NULL					},
	{"ON_KILL",							TRIGGER,			TOWER_TRIGGER_ON_KILL,				&TakeNoAction,					NULL					},
	{"ON_DEATH",						TRIGGER,			CREEP_TRIGGER_ON_DEATH,				&TakeNoAction,					NULL					},
	{"ON_CLICK",						TRIGGER,			GAME_TRIGGER_ON_CLICK,				&TakeNoAction,					NULL					},
	{NULL,								NULL,				NULL,								NULL,							NULL					},
};

static Word BaseList[] = 
{
	{"NONE",							NORMAL,				0,									&TakeNoAction,					NULL					},
	{"FALSE",							NORMAL,				0,									&TakeNoAction,					NULL					},
	{"TRUE",							NORMAL,				1,									&TakeNoAction,					NULL					},
	{"NO",								NORMAL,				0,									&TakeNoAction,					NULL					},
	{"YES",								NORMAL,				1,									&TakeNoAction,					NULL					},
	{"INVALID",							NORMAL,				0,									&TakeNoAction,					NULL					},
	{"VALID",							NORMAL,				1,									&TakeNoAction,					NULL					},
	{"INFINITE",						NORMAL,				-2,									&TakeNoAction,					NULL					},

	{"FLAT",							NORMAL,				KEYWORD_FLAT,						&TakeNoAction,					NULL					},
	{"PERCENT",							NORMAL,				KEYWORD_PERCENT,					&TakeNoAction,					NULL					},

	{";",								ACTION,				KEYWORD_END,						&EndCommand,					NULL					},
	{"IF",								ACTION,				KEYWORD_IF,							&EvaluateIf,					NULL					},
	{"ELSE",							ACTION,				KEYWORD_ELSE,						&TakeNoAction,					NULL					},
	{"FOR",								ACTION,				KEYWORD_FOR,						&ForLoop,						ForList					},
	{"EVERY",							ACTION,				KEYWORD_EVERY,						&EveryTimedScript,				NULL					},
	{"STOP",							ACTION,				KEYWORD_STOP,						&StopScript,					NULL					},
	{"MAKE",							ACTION,				KEYWORD_MAKE,						&InvalidHandler,				MakeList				},
	{"SET",								ACTION,				KEYWORD_SET,						&SetNormalValues,				SetList					},
	{"MOD",								ACTION,				KEYWORD_MOD,						&ModNormalValues,				NULL					},
	{"FLAG",							ACTION,				KEYWORD_FLAG,						&FlagNormalValues,				NULL					},
	{"GET",								ACTION,				KEYWORD_GET,						&GetNormalValues,				GetList					},
	{"RAND",							ACTION,				KEYWORD_RAND,						&GenerateRandomValue,			NULL					},
	{"SELECT",							ACTION,				KEYWORD_SELECT,						&SelectObject,					NULL					},
	{"DELETE",							ACTION,				KEYWORD_DELETE,						&DeleteObject,					NULL					},
	{"REFRESH",							ACTION,				KEYWORD_REFRESH,					&RefreshObject,					NULL					},
	{"TRIGGER",							ACTION,				KEYWORD_TRIGGER,					&AddTrigger,					NULL					},
	{"CUSTOM",							ACTION,				KEYWORD_CUSTOM,						&ActivateCustomScript,			NULL					},

	{"SELF",							POINTER,			KEYWORD_TARGET_SELF,				&TakeNoAction,					NULL					},
	{"SELECTION",						POINTER,			KEYWORD_TARGET_SELECTION,			&TakeNoAction,					NULL					},
	{"TARGET",							POINTER,			KEYWORD_TARGET_TARGET,				&TakeNoAction,					NULL					},
	{"TOWER_PROJECTILE",				POINTER,			KEYWORD_TARGET_PROJECTILE,			&TakeNoAction,					NULL					},
	{"SOURCE",							POINTER,			KEYWORD_TARGET_SOURCE,				&TakeNoAction,					NULL					},
	{"THIS",							POINTER,			KEYWORD_TARGET_THIS,				&TakeNoAction,					NULL					},
	{"WORLDGAME",						POINTER,			KEYWORD_TARGET_WORLDGAME,			&TakeNoAction,					NULL					},

	{"==",								OPERATOR,			KEYWORD_EQUALS,						&OperatorEquals,				NULL					},
	{"!=",								OPERATOR,			KEYWORD_NOT_EQUALS,					&OperatorNotEquals,				NULL					},
	{"<",								OPERATOR,			KEYWORD_LESS_THAN,					&OperatorLessThan,				NULL					},
	{"<=",								OPERATOR,			KEYWORD_LESS_OR_EQUALS,				&OperatorLessOrEqual,			NULL					},
	{">",								OPERATOR,			KEYWORD_GREATER_THAN,				&OperatorGreaterThan,			NULL					},
	{">=",								OPERATOR,			KEYWORD_GREATER_OR_EQUALS,			&OperatorGreaterOrEqual,		NULL					},
	{"+",								OPERATOR,			KEYWORD_ADD,						&OperatorAdd,					NULL					},
	{"-",								OPERATOR,			KEYWORD_SUBTRACT,					&OperatorSubtract,				NULL					},
	{"*",								OPERATOR,			KEYWORD_MULTIPLY,					&OperatorMultiply,				NULL					},
	{"/",								OPERATOR,			KEYWORD_DIVIDE,						&OperatorDivide,				NULL					},
	{"/%",								OPERATOR,			KEYWORD_INT_DIVIDE,					&OperatorIntDivide,				NULL					},
	{"%",								OPERATOR,			KEYWORD_MODULO,						&OperatorModulo,				NULL					},
	{"=",								OPERATOR,			KEYWORD_ASSIGN,						&OperatorAssign,				NULL					},
	{"EQUALS",							OPERATOR,			KEYWORD_EQUALS,						&OperatorEquals,				NULL					},
	{"NOT_EQUALS",						OPERATOR,			KEYWORD_NOT_EQUALS,					&OperatorNotEquals,				NULL					},
	{"LESS_THAN",						OPERATOR,			KEYWORD_LESS_THAN,					&OperatorLessThan,				NULL					},
	{"LESS_OR_EQUAL",					OPERATOR,			KEYWORD_LESS_OR_EQUALS,				&OperatorLessOrEqual,			NULL					},
	{"GREATER_THAN",					OPERATOR,			KEYWORD_GREATER_THAN,				&OperatorGreaterThan,			NULL					},
	{"GREATER_OR_EQUAL",				OPERATOR,			KEYWORD_GREATER_OR_EQUALS,			&OperatorGreaterOrEqual,		NULL					},
	{"ADD",								OPERATOR,			KEYWORD_ADD,						&OperatorAdd,					NULL					},
	{"PLUS",							OPERATOR,			KEYWORD_ADD,						&OperatorAdd,					NULL					},
	{"SUBTRACT",						OPERATOR,			KEYWORD_SUBTRACT,					&OperatorSubtract,				NULL					},
	{"MINUS",							OPERATOR,			KEYWORD_SUBTRACT,					&OperatorSubtract,				NULL					},
	{"MULTIPLY",						OPERATOR,			KEYWORD_MULTIPLY,					&OperatorMultiply,				NULL					},
	{"TIMES",							OPERATOR,			KEYWORD_MULTIPLY,					&OperatorMultiply,				NULL					},
	{"DIVIDE",							OPERATOR,			KEYWORD_DIVIDE,						&OperatorDivide,				NULL					},
	{"INT_DIVIDE",						OPERATOR,			KEYWORD_INT_DIVIDE,					&OperatorIntDivide,				NULL					},
	{"MODULO",							OPERATOR,			KEYWORD_MODULO,						&OperatorModulo,				NULL					},
	{"TRUNCATE",						OPERATOR,			KEYWORD_TRUNCATE,					&OperatorTruncate,				NULL					},
	{"ROUND",							OPERATOR,			KEYWORD_ROUND,						&OperatorRound,					NULL					},
	{"CEILING",							OPERATOR,			KEYWORD_CEILING,					&OperatorCeiling,				NULL					},
	{"FLOOR",							OPERATOR,			KEYWORD_FLOOR,						&OperatorFloor,					NULL					},
	{"&&",								OPERATOR,			KEYWORD_AND,						&OperatorLogicalAnd,			NULL					},
	{"AND",								OPERATOR,			KEYWORD_AND,						&OperatorLogicalAnd,			NULL					},
	{"||",								OPERATOR,			KEYWORD_OR,							&OperatorLogicalOr,				NULL					},
	{"OR",								OPERATOR,			KEYWORD_OR,							&OperatorLogicalOr,				NULL					},
	{"!!",								OPERATOR,			KEYWORD_NOT,						&OperatorLogicalNot,			NULL					},
	{"NOT",								OPERATOR,			KEYWORD_NOT,						&OperatorLogicalNot,			NULL					},
	{"^^",								OPERATOR,			KEYWORD_XOR,						&OperatorLogicalXOr,			NULL					},
	{"XOR",								OPERATOR,			KEYWORD_XOR,						&OperatorLogicalXOr,			NULL					},

	{"SIN_R",							OPERATOR,			KEYWORD_SINE_R,						&OperatorSineR,					NULL					},
	{"SIN_D",							OPERATOR,			KEYWORD_SINE_D,						&OperatorSineD,					NULL					},
	{"COS_R",							OPERATOR,			KEYWORD_COSINE_R,					&OperatorCosineR,				NULL					},
	{"COS_D",							OPERATOR,			KEYWORD_COSINE_D,					&OperatorCosineD,				NULL					},
	{"TAN_R",							OPERATOR,			KEYWORD_TANGENT_R,					&OperatorTangentR,				NULL					},
	{"TAN_D",							OPERATOR,			KEYWORD_TANGENT_D,					&OperatorTangentD,				NULL					},
	{"ASIN_R",							OPERATOR,			KEYWORD_ARCSINE_R,					&OperatorArcSineR,				NULL					},
	{"ASIN_D",							OPERATOR,			KEYWORD_ARCSINE_D,					&OperatorArcSineD,				NULL					},
	{"ACOS_R",							OPERATOR,			KEYWORD_ARCCOSINE_R,				&OperatorArcCosineR,			NULL					},
	{"ACOS_D",							OPERATOR,			KEYWORD_ARCCOSINE_D,				&OperatorArcCosineD,			NULL					},
	{"ATAN_R",							OPERATOR,			KEYWORD_ARCTANGENT_R,				&OperatorArcTangentR,			NULL					},
	{"ATAN_D",							OPERATOR,			KEYWORD_ARCTANGENT_D,				&OperatorArcTangentD,			NULL					},
	{"LOG_2",							OPERATOR,			KEYWORD_LOG_2,						&OperatorLog2,					NULL					},
	{"LOG_N",							OPERATOR,			KEYWORD_LOG_N,						&OperatorLogN,					NULL					},
	{"LOG_E",							OPERATOR,			KEYWORD_LOG_E,						&OperatorLogE,					NULL					},
	{"LN",								OPERATOR,			KEYWORD_LOG_E,						&OperatorLogE,					NULL					},
	{"LOG",								OPERATOR,			KEYWORD_LOG_10,						&OperatorLog10,					NULL					},
	{"LOG_10",							OPERATOR,			KEYWORD_LOG_10,						&OperatorLog10,					NULL					},
	{"EXP_2",							OPERATOR,			KEYWORD_EXP_2,						&OperatorExp2,					NULL					},
	{"EXP_N",							OPERATOR,			KEYWORD_EXP_N,						&OperatorExpN,					NULL					},
	{"EXP_E",							OPERATOR,			KEYWORD_EXP_E,						&OperatorExpE,					NULL					},
	{"EXP",								OPERATOR,			KEYWORD_EXP_E,						&OperatorExpE,					NULL					},
	{"EXP_10",							OPERATOR,			KEYWORD_EXP_10,						&OperatorExp10,					NULL					},
	{"ROOT_2",							OPERATOR,			KEYWORD_ROOT_2,						&OperatorRoot2,					NULL					},
	{"SQRT",							OPERATOR,			KEYWORD_ROOT_2,						&OperatorRoot2,					NULL					},
	{"SQUARE_ROOT",						OPERATOR,			KEYWORD_ROOT_2,						&OperatorRoot2,					NULL					},
	{"ROOT_3",							OPERATOR,			KEYWORD_ROOT_3,						&OperatorRoot3,					NULL					},
	{"CUBE_ROOT",						OPERATOR,			KEYWORD_ROOT_3,						&OperatorRoot3,					NULL					},
	{"MIN_FIT",							OPERATOR,			KEYWORD_MIN_FIT,					&OperatorMinFit,				NULL					},
	{"MAX_FIT",							OPERATOR,			KEYWORD_MAX_FIT,					&OperatorMaxFit,				NULL					},
	{"INTERVAL_FIT",					OPERATOR,			KEYWORD_INTERVAL_FIT,				&OperatorIntervalFit,			NULL					},
	{"ABS_VAL",							OPERATOR,			KEYWORD_ABS_VAL,					&OperatorAbsVal,				NULL					},
	{"ZERO_ANGLE_D",					OPERATOR,			KEYWORD_ZERO_ANGLE_D,				&OperatorGetZeroAngleD,			NULL					},
	{"ZERO_ANGLE_R",					OPERATOR,			KEYWORD_ZERO_ANGLE_R,				&OperatorGetZeroAngleR,			NULL					},
	{NULL,								NULL,				NULL,								NULL,							NULL					},
};

static Word SpellList[] =
{
	{"ACTIVE",							NORMAL,				SPELL_MODE_ACTIVE,					&TakeNoAction,					NULL					},
	{"PASSIVE",							NORMAL,				SPELL_MODE_PASSIVE,					&TakeNoAction,					NULL					},
	{"TRIGGERED",						NORMAL,				SPELL_MODE_TRIGGERED,				&TakeNoAction,					NULL					},
	{"LEARN",							NORMAL,				SPELL_MODE_LEARN,					&TakeNoAction,					NULL					},

	{"SPELL_ID",						UINT32_CONST,		SPELL_VAR_ID,						&TakeNoAction,					NULL					},
	{"SPELL_VISIBILITY",				UINT32_VAR,			SPELL_VAR_VISIBILITY,				&TakeNoAction,					NULL					},
	{"SPELL_MODE",						UINT32_CONST,		SPELL_VAR_MODE,						&TakeNoAction,					NULL					},
	{"SPELL_LEVEL",						UINT32_CONST,		SPELL_VAR_LEVEL,					&TakeNoAction,					NULL					},
	{"SPELL_PREREQUISITE",				UINT32_CONST,		SPELL_VAR_PREREQUISITE,				&TakeNoAction,					NULL					},
	{"SPELL_AUTOMATIC_FLAG",			UINT32_CONST,		SPELL_VAR_AUTOMATIC,				&TakeNoAction,					NULL					},
	{"SPELL_CATEGORY",					UINT32_CONST,		SPELL_VAR_CATEGORY,					&TakeNoAction,					NULL					},
	{"SPELL_CHARGES_LEFT",				UINT32_VAR,			SPELL_VAR_REMAINING_CHARGES,		&TakeNoAction,					NULL					},
	{"SPELL_COST",						INT32_VAR,			SPELL_VAR_COST,						&TakeNoAction,					NULL					},
	{"SPELL_CAST_TIME",					INT32_VAR,			SPELL_VAR_CAST_TIME,				&TakeNoAction,					NULL					},
	{"SPELL_COOLDOWN",					INT32_VAR,			SPELL_VAR_COOLDOWN,					&TakeNoAction,					NULL					},
	{"SPELL_CHARGES",					INT32_VAR,			SPELL_VAR_CHARGES,					&TakeNoAction,					NULL					},
	{"SPELL_DURATION",					INT32_VAR,			SPELL_VAR_DURATION,					&TakeNoAction,					NULL					},
	{"SPELL_STACKS",					INT32_VAR,			SPELL_VAR_STACKS,					&TakeNoAction,					NULL					},
	{"SPELL_ICON_ID",					INT32_CONST,		SPELL_VAR_ICON_ID,					&TakeNoAction,					NULL					},
	{"SPELL_HOTKEY",					INT32_CONST,		SPELL_VAR_HOTKEY,					&TakeNoAction,					NULL					},
	{"SPELL_LAST_CAST_TIME",			INT32_VAR,			SPELL_VAR_LAST_CAST_TIME,			&TakeNoAction,					NULL					},

	{"LEARN_SPELL",						ACTION,				SPELL_KEYWORD_LEARN_SPELL,			&LearnSpell,					NULL					},
	{"FORGET_SPELL",					ACTION,				SPELL_KEYWORD_FORGET_SPELL,			&ForgetSpell,					NULL					},
	{"APPLY_SPELL",						ACTION,				SPELL_KEYWORD_APPLY_SPELL,			&ApplySpell,					NULL					},
	{"REMOVE_SPELL",					ACTION,				SPELL_KEYWORD_REMOVE_SPELL,			&RemoveSpell,					NULL					},
	{"ACTIVE_SPELL",					ACTION,				SPELL_KEYWORD_ACTIVE_SPELL,			&ActiveSpell,					NULL					},
	{"KNOWS_SPELL",						ACTION,				SPELL_KEYWORD_KNOWS_SPELL,			&KnowsSpell,					NULL					},
	{"SHOW_SPELL",						ACTION,				SPELL_KEYWORD_SHOW_SPELL,			&ShowSpell,						NULL					},
	{"CAST_SPELL",						ACTION,				SPELL_KEYWORD_CAST_SPELL,			&CastSpell,						NULL					},
	{"SPELL_COOLDOWN",					ACTION,				SPELL_KEYWORD_SPELL_COOLDOWN,		&CheckSpellCooldown,			NULL					},
	{"SPELL_CHARGES",					ACTION,				SPELL_KEYWORD_SPELL_CHARGES,		&CheckSpellCharges,				NULL					},
	{"APPLY_AURA",						ACTION,				SPELL_KEYWORD_APPLY_AURA,			&ApplyAura,						NULL					},
	{NULL,								NULL,				NULL,								NULL,							NULL					},
};

static Word GameList[] =
{
	{"GAME_SCREEN",						UINT32_CONST,		GAME_VAR_SCREEN,					&TakeNoAction,					NULL					},
	{"GAME_BUILD",						UINT32_CONST,		GAME_VAR_BUILD,						&TakeNoAction,					NULL					},
	{"GAME_WAVE_NUMBER",				UINT32_CONST,		GAME_VAR_WAVE_NUMBER,				&TakeNoAction,					NULL					},
	{"GAME_WAVE_SIZE",					UINT32_CONST,		GAME_VAR_WAVE_SIZE,					&TakeNoAction,					NULL					},
	{"GAME_WAVE_INTERVAL",				UINT32_CONST,		GAME_VAR_WAVE_INTERVAL,				&TakeNoAction,					NULL					},
	{"GAME_WAVE_TIME_DIF",				UINT32_CONST,		GAME_VAR_WAVE_TIME_DIFFERENCE,		&TakeNoAction,					NULL					},
	{"GAME_WAVE_PRELOAD",				UINT32_CONST,		GAME_VAR_WAVE_PRELOAD_COUNT,		&TakeNoAction,					NULL					},
	{"GAME_WAVE_TIMER",					UINT32_CONST,		GAME_VAR_WAVE_TIMER,				&TakeNoAction,					NULL					},
	{"GAME_DIFFICULTY",					UINT32_CONST,		GAME_VAR_DIFFICULTY,				&TakeNoAction,					NULL					},
	{"GAME_ACTIVE_STATE",				UINT32_CONST,		GAME_VAR_ACTIVE_STATE,				&TakeNoAction,					NULL					},
	{"GAME_INK",						FLOAT_VAR,			GAME_VAR_INK,						&TakeNoAction,					NULL					},
	{"GAME_MAX_INK",					FLOAT_VAR,			GAME_VAR_MAX_INK,					&TakeNoAction,					NULL					},
	{"GAME_SCORE",						INT32_VAR,			GAME_VAR_SCORE,						&TakeNoAction,					NULL					},
	{"LIGHT_COUNTER",					INT32_VAR,			GAME_VAR_LIGHT_COUNTER,				&TakeNoAction,					NULL					},
	{NULL,								NULL,				NULL,								NULL,							NULL					},
};

static Word TowerList[] =
{
	{"TOWER_TYPE",						UINT32_CONST,		TOWER_VAR_TYPE,						&TakeNoAction,					NULL					},
	{"TOWER_ICON",						UINT32_CONST,		TOWER_VAR_ICON_ID,					&TakeNoAction,					NULL					},
	{"TOWER_LEVEL",						UINT32_CONST,		TOWER_VAR_LEVEL,					&TakeNoAction,					NULL					},
	{"TOWER_BASE_DAMAGE",				UINT32_CONST,		TOWER_VAR_BASE_DAMAGE,				&TakeNoAction,					NULL					},
	{"TOWER_BASE_COST",					UINT32_CONST,		TOWER_VAR_BASE_COST,				&TakeNoAction,					NULL					},
	{"TOWER_BASE_RANGE",				UINT32_CONST,		TOWER_VAR_BASE_RANGE,				&TakeNoAction,					NULL					},
	{"TOWER_BAT",						UINT32_CONST,		TOWER_VAR_BASE_ATTACK_TIME,			&TakeNoAction,					NULL					},
	{"TOWER_CAN_BUILD",					UINT32_CONST,		TOWER_VAR_CAN_BUILD,				&TakeNoAction,					NULL					},
	{"TOWER_CREEP_KILLS",				UINT32_CONST,		TOWER_VAR_CREEP_KILLS,				&TakeNoAction,					NULL					},
	{"TOWER_BUILD_TIME",				UINT32_CONST,		TOWER_VAR_BUILD_TIME,				&TakeNoAction,					NULL					},
	{"TOWER_ATTACK_TIME",				UINT32_VAR,			TOWER_VAR_LAST_ATTACK_TIME,			&TakeNoAction,					NULL					},
	{"TOWER_ATTACK_DIF",				UINT32_CONST,		TOWER_VAR_ATTACK_TIME_DIF,			&TakeNoAction,					NULL					},
	{"TOWER_WAITING",					UINT32_VAR,			TOWER_VAR_WAITING,					&TakeNoAction,					NULL					},
	{"TOWER_DAMAGE_FLAT",				INT32_VAR,			TOWER_VAR_DAMAGE_FLAT,				&TakeNoAction,					NULL					},
	{"TOWER_RANGE_FLAT",				INT32_VAR,			TOWER_VAR_RANGE_FLAT,				&TakeNoAction,					NULL					},
	{"TOWER_BAT_FLAT",					INT32_VAR,			TOWER_VAR_BAT_FLAT,					&TakeNoAction,					NULL					},
	{"TOWER_CAN_FIRE",					INT32_VAR,			TOWER_VAR_CAN_FIRE,					&TakeNoAction,					NULL					},
	{"TOWER_CAN_SELL",					INT32_VAR,			TOWER_VAR_CAN_SELL,					&TakeNoAction,					NULL					},
	{"TOWER_SPECIAL_RANGE",				INT32_VAR,			TOWER_VAR_SPECIAL_RANGE,			&TakeNoAction,					NULL					},
	{"TOWER_GHOST_STATE",				INT32_VAR,			TOWER_VAR_GHOST_STATE,				&TakeNoAction,					NULL					},
	{"TOWER_SPELL_COEFF",				FLOAT_VAR,			TOWER_VAR_SPELL_COEFFICIENT,		&TakeNoAction,					NULL					},
	{"TOWER_DAMAGE_BONUS",				FLOAT_VAR,			TOWER_VAR_DAMAGE_MULTIPLIER,		&TakeNoAction,					NULL					},
	{"TOWER_RANGE_BONUS",				FLOAT_VAR,			TOWER_VAR_RANGE_MULTIPLIER,			&TakeNoAction,					NULL					},
	{"TOWER_BAT_BONUS",					FLOAT_VAR,			TOWER_VAR_BAT_MULTIPLIER,			&TakeNoAction,					NULL					},

	{"FIRE_TOWER",						ACTION,				TOWER_KEYWORD_FIRE_TOWER,			&FireTower,						NULL					},
	{"BUILD_TOWER",						ACTION,				TOWER_KEYWORD_BUILD_TOWER,			&BuildTower,					NULL					},
	{NULL,								NULL,				NULL,								NULL,							NULL					},
};

static Word ProjectileList[] =
{
	{"PROJ_TYPE",						UINT32_CONST,		PROJ_VAR_BASE_TYPE,					&TakeNoAction,					NULL					},
	{"PROJ_SIZE",						UINT32_CONST,		PROJ_VAR_BASE_SIZE,					&TakeNoAction,					NULL					},
	{"PROJ_BASE_DAMAGE",				UINT32_CONST,		PROJ_VAR_BASE_DAMAGE,				&TakeNoAction,					NULL					},
	{"PROJ_BASE_SPEED",					UINT32_CONST,		PROJ_VAR_BASE_SPEED,				&TakeNoAction,					NULL					},
	{"PROJ_BASE_SPLASH",				UINT32_CONST,		PROJ_VAR_BASE_SPLASH,				&TakeNoAction,					NULL					},
	{"PROJ_DURATION",					UINT32_CONST,		PROJ_VAR_DURATION,					&TakeNoAction,					NULL					},
	{"PROJ_ANIMATION_ID",				UINT32_CONST,		PROJ_VAR_ANIMATION_ID,				&TakeNoAction,					NULL					},
	{"PROJ_EXPLOSION_ID",				UINT32_CONST,		PROJ_VAR_EXPLOSION_ID,				&TakeNoAction,					NULL					},
	{"PROJ_START_TIME",					UINT32_CONST,		PROJ_VAR_START_TIME,				&TakeNoAction,					NULL					},
	{"PROJ_LAST_MOVE_TIME",				UINT32_CONST,		PROJ_VAR_LAST_MOVE_TIME,			&TakeNoAction,					NULL					},
	{"PROJ_DAMAGE_FLAT",				INT32_VAR,			PROJ_VAR_DAMAGE_FLAT,				&TakeNoAction,					NULL					},
	{"PROJ_SPEED_FLAT",					INT32_VAR,			PROJ_VAR_SPEED_FLAT,				&TakeNoAction,					NULL					},
	{"PROJ_SPLASH_FLAT",				INT32_VAR,			PROJ_VAR_SPLASH_FLAT,				&TakeNoAction,					NULL					},
	{"PROJ_ARMOR_IGNORE_F",				INT32_VAR,			PROJ_VAR_ARMOR_IGNORE_FLAT,			&TakeNoAction,					NULL					},
	{"PROJ_FLAGS",						INT32_VAR,			PROJ_VAR_FLAGS,						&TakeNoAction,					NULL					},
	{"PROJ_CAN_MOVE",					INT32_VAR,			PROJ_VAR_CAN_MOVE,					&TakeNoAction,					NULL					},
	{"PROJ_DAMAGE_BONUS",				FLOAT_VAR,			PROJ_VAR_DAMAGE_MULTIPLIER,			&TakeNoAction,					NULL					},
	{"PROJ_SPEED_BONUS",				FLOAT_VAR,			PROJ_VAR_SPEED_MULTIPLIER,			&TakeNoAction,					NULL					},
	{"PROJ_SPLASH_BONUS",				FLOAT_VAR,			PROJ_VAR_SPLASH_MULTIPLIER,			&TakeNoAction,					NULL					},
	{"PROJ_ARMOR_IGNORE_P",				FLOAT_VAR,			PROJ_VAR_ARMOR_IGNORE_PERCENT,		&TakeNoAction,					NULL					},
	{"PROJ_PHASE_PERCENT",				FLOAT_VAR,			PROJ_VAR_PHASE_PERCENT,				&TakeNoAction,					NULL					},

	{"PFLAG_COLLIDE",					NORMAL,				PFLAG_COLLIDE_WITH_CREEPS,			&TakeNoAction,					NULL					},
	{"PFLAG_INSTANTANEOUS",				NORMAL,				PFLAG_INSTANTANEOUS,				&TakeNoAction,					NULL					},
	{"PFLAG_LINEAR",					NORMAL,				PFLAG_LINEAR,						&TakeNoAction,					NULL					},
	{NULL,								NULL,				NULL,								NULL,							NULL					},
};

static Word CreepList[] = 
{
	{"CREEP_TYPE",						UINT32_CONST,		CREEP_VAR_TYPE,						&TakeNoAction,					NULL					},
	{"CREEP_WAVE",						UINT32_CONST,		CREEP_VAR_WAVE,						&TakeNoAction,					NULL					},
	{"CREEP_BASE_HP",					UINT32_CONST,		CREEP_VAR_BASE_HP,					&TakeNoAction,					NULL					},
	{"CREEP_BASE_ARMOR",				UINT32_CONST,		CREEP_VAR_BASE_ARMOR,				&TakeNoAction,					NULL					},
	{"CREEP_BASE_SPEED",				UINT32_CONST,		CREEP_VAR_BASE_SPEED,				&TakeNoAction,					NULL					},
	{"CREEP_BASE_BOUNTY",				UINT32_CONST,		CREEP_VAR_BASE_BOUNTY,				&TakeNoAction,					NULL					},
	{"CREEP_BASE_DAMAGE",				UINT32_CONST,		CREEP_VAR_BASE_DAMAGE,				&TakeNoAction,					NULL					},
	{"CREEP_MOVE_TIMER",				UINT32_CONST,		CREEP_VAR_MOVE_TIMER,				&TakeNoAction,					NULL					},
	{"CREEP_ARMOR_FLAT",				INT32_VAR,			CREEP_VAR_ARMOR_FLAT,				&TakeNoAction,					NULL					},
	{"CREEP_SPEED_FLAT",				INT32_VAR,			CREEP_VAR_SPEED_FLAT,				&TakeNoAction,					NULL					},
	{"CREEP_BOUNTY_FLAT",				INT32_VAR,			CREEP_VAR_BOUNTY_FLAT,				&TakeNoAction,					NULL					},
	{"CREEP_CAN_MOVE",					INT32_VAR,			CREEP_VAR_CAN_MOVE,					&TakeNoAction,					NULL					},
	{"CREEP_PROJ_IMMUNE",				INT32_VAR,			CREEP_VAR_PROJECTILE_IMMUNE,		&TakeNoAction,					NULL					},
	{"CREEP_SPLASH_IMMUNE",				INT32_VAR,			CREEP_VAR_SPLASH_IMMUNE,			&TakeNoAction,					NULL					},
	{"CREEP_SPELL_IMMUNE",				INT32_VAR,			CREEP_VAR_SPELL_IMMUNE,				&TakeNoAction,					NULL					},
	{"CREEP_FLYING",					INT32_VAR,			CREEP_VAR_FLYING,					&TakeNoAction,					NULL					},
	{"CREEP_HITPOINTS",					FLOAT_VAR,			CREEP_VAR_HITPOINTS,				&TakeNoAction,					NULL					},
	{"CREEP_MAX_HITPOINTS",				FLOAT_VAR,			CREEP_VAR_MAX_HITPOINTS,			&TakeNoAction,					NULL					},
	{"CREEP_ARMOR_BONUS",				FLOAT_VAR,			CREEP_VAR_ARMOR_MULTIPLIER,			&TakeNoAction,					NULL					},
	{"CREEP_SPEED_BONUS",				FLOAT_VAR,			CREEP_VAR_SPEED_MULTIPLIER,			&TakeNoAction,					NULL					},
	{"CREEP_BOUNTY_BONUS",				FLOAT_VAR,			CREEP_VAR_BOUNTY_MULTIPLIER,		&TakeNoAction,					NULL					},
	{"CREEP_DAMAGE_BONUS",				FLOAT_VAR,			CREEP_VAR_DAMAGE_MULTIPLIER,		&TakeNoAction,					NULL					},

	{"DAMAGE_CREEP",					ACTION,				CREEP_KEYWORD_DAMAGE_CREEP,			&DamageCreep,					NULL					},
	{NULL,								NULL,				NULL,								NULL,							NULL					},
};

static Word SpellAreaList[] =
{
	{"TOWERS",							NORMAL,				SPELL_AREA_TOWERS,					&SpellAreaTowers,				NULL					},
	{"PROJECTILES",						NORMAL,				SPELL_AREA_PROJECTILES,				&SpellAreaProjectiles,			NULL					},
	{"CREEPS",							NORMAL,				SPELL_AREA_CREEPS,					&SpellAreaCreeps,				NULL					},
};

static Word GameObjectList[] =
{
	{"GOBJECT_ID",						UINT32_CONST,		GOBJECT_VAR_ID,						&TakeNoAction,					NULL					},
	{"GOBJECT_START_TIME",				UINT32_CONST,		GOBJECT_VAR_START_TIME,				&TakeNoAction,					NULL					},
	{"GOBJECT_UPDATE_TIME",				UINT32_CONST,		GOBJECT_VAR_LAST_UPDATE_TIME,		&TakeNoAction,					NULL					},
	{"GOBJECT_TIME_DIF",				UINT32_CONST,		GOBJECT_VAR_TIME_DIFFERENCE,		&TakeNoAction,					NULL					},
	{"GOBJECT_ANIMATION_ID",			UINT32_CONST,		GOBJECT_VAR_ANIMATION_ID,			&TakeNoAction,					NULL					},
	{"GOBJECT_DURATION",				INT32_VAR,			GOBJECT_VAR_DURATION,				&TakeNoAction,					NULL					},

	{"ROTATE_GOBJECT",					ACTION,				GOBJECT_KEYWORD_ROTATE,				&RotateGameObject,				NULL					},
	{"DAMAGE_AREA",						ACTION,				GOBJECT_KEYWORD_DAMAGE_AREA,		&DamageArea,					NULL					},
	{"SPELL_AREA",						ACTION,				GOBJECT_KEYWORD_SPELL_AREA,			&InvalidHandler,				SpellAreaList			},

	{"LINE",							NORMAL,				SHAPE_TYPE_LINE,					&TakeNoAction,					NULL					},
	{"LINE_SEGMENT",					NORMAL,				SHAPE_TYPE_LINE_SEGMENT,			&TakeNoAction,					NULL					},
	{"RECT",							NORMAL,				SHAPE_TYPE_RECT,					&TakeNoAction,					NULL					},
	{"CIRCLE",							NORMAL,				SHAPE_TYPE_CIRCLE,					&TakeNoAction,					NULL					},
	{"CONE",							NORMAL,				SHAPE_TYPE_CONE,					&TakeNoAction,					NULL					},
	{NULL,								NULL,				NULL,								NULL,							NULL					},
};

static Word AnimationList[] =
{
	{"ANIM_DURATION",					UINT32_CONST,		ANIM_VAR_DURATION,					&TakeNoAction,					NULL					},
	{"ANIM_START_TIME",					UINT32_CONST,		ANIM_VAR_START_TIME,				&TakeNoAction,					NULL					},
	{"ANIM_BASE_SCALE",					FLOAT_VAR,			ANIM_VAR_BASE_SCALE,				&TakeNoAction,					NULL					},
	{"ANIM_BASE_OPACITY",				FLOAT_VAR,			ANIM_VAR_BASE_OPACITY,				&TakeNoAction,					NULL					},
	{"ANIM_BASE_ROTATION",				FLOAT_VAR,			ANIM_VAR_BASE_ROTATION,				&TakeNoAction,					NULL					},

	{"BOUNDS",							ACTION,				ANIM_KEYWORD_BOUNDS,				&SetBounds,						NULL					},
	{"VAR_BOUNDS",						ACTION,				ANIM_KEYWORD_VAR_BOUNDS,			&SetVariableBounds,				NULL					},
	{"SCALE",							ACTION,				ANIM_KEYWORD_SCALE,					&SetAnimationScale,				NULL					},
	{"OPACITY",							ACTION,				ANIM_KEYWORD_OPACITY,				&SetAnimationOpacity,			NULL					},
	{"ROTATE",							ACTION,				ANIM_KEYWORD_ROTATE,				&SetAnimationRotation,			NULL					},
	{"SCALING_CENTER",					ACTION,				ANIM_KEYWORD_CENTER_S,				&SetScalingCenter,				NULL					},
	{"ROTATION_CENTER",					ACTION,				ANIM_KEYWORD_CENTER_R,				&SetRotationCenter,				NULL					},
	{"OFFSET",							ACTION,				ANIM_KEYWORD_OFFSET,				&SetAnimationOffset,			NULL					},
	{"DRAW",							ACTION,				ANIM_KEYWORD_DRAW_ANIM,				&DrawAnimation,					NULL					},
	{NULL,								NULL,				NULL,								NULL,							NULL					},
};

static Word * Dictionary[] =
{
	GetGlobalList,
	TriggerList,
	BaseList,
	SpellList,
	GameList,
	TowerList,
	ProjectileList,
	CreepList,
	SpellAreaList,
	GameObjectList,
	AnimationList,
};

int32 LookupWord(const char * word, bool type, Word * lastWord)
{
	if (lastWord != NULL)
	{
		Word * subCommands = lastWord->subcommands;
		if (subCommands != NULL)
		{
			for (uint32 i = 0;; i++)
			{
				if (subCommands[i].handler == NULL)
				{
					break;
				}
				else if (strcmp(subCommands[i].word, word) == 0)
				{
					return (type == LOOKUP_VALUE) ? subCommands[i].value : (uint32)&subCommands[i];
				}
			}
		}
	}
	uint16 dictionarySize = sizeof(Dictionary)/sizeof(Word *);
	for (uint16 i = 0; i < dictionarySize; i++)
	{
		Word * wordList = Dictionary[i];
		for (uint32 j = 0;; j++)
		{
			if (wordList[j].handler == NULL)
			{
				break;
			}
			else if (strcmp(wordList[j].word, word) == 0)
			{
				return (type == LOOKUP_VALUE) ? wordList[j].value : (uint32)&wordList[j];
			}
		}
	}
	return NOT_FOUND;
}