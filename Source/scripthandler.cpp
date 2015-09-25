//#include "type.h"
//#include "script.h"
//#include "scripthandler.h"
//#include "gameobject.h"
//#include "spell.h"
//#include "creep.h"
//#include "projectile.h"
//#include "tower.h"
//#include "game.h"
//
//static Word ForAllList[] = 
//{
//	{"TOWERS",							NORMAL,				1,							&TakeNoAction,					ForAllList				},
//	{"CREEPS",							NORMAL,				2,							&TakeNoAction,					ForAllList				},
//	{"PROJECTILES",						NORMAL,				3,							&TakeNoAction,					ForAllList				},
//	{"GAME_OBEJCTS",					NORMAL,				4,							&TakeNoAction,					ForAllList				},
//	{"ANIMATIONS",						NORMAL,				5,							&TakeNoAction,					ForAllList				},
//	{"EMITTERS",						NORMAL,				6,							&TakeNoAction,					ForAllList				},
//};
//
//static Word ForList[] = 
//{
//	{"ALL",								NORMAL,				1,							&TakeNoAction,					ForAllList				},
//};
//
//static Word MakeList[] =
//{
//	{"POINT2D",							NORMAL,				1,							&TakeNoAction,					NULL					},
//	{"RECT",							NORMAL,				2,							&TakeNoAction,					NULL					},
//	{"VARIABLE",						NORMAL,				3,							&TakeNoAction,					NULL					},
//	{"ANIMATION",						NORMAL,				4,							&TakeNoAction,					NULL					},
//	{"GAME_OBJECT",						NORMAL,				5,							&TakeNoAction,					NULL					},
//	{"EMITTER",							NORMAL,				6,							&TakeNoAction,					NULL					},
//};
//
//static Word SetList[] = 
//{
//	{"POSITION",						NORMAL,				0,							&TakeNoAction,					NULL					},
//	{"PROJECTILE",						NORMAL,				0,							&TakeNoAction,					NULL					},
//};
//
//static Word GetPositionList[] =
//{
//	{"MOVE_TARGET",						NORMAL,				GET_MOVE_TARGET,			&TakeNoAction,					NULL					},
//	{"LAST_POS",						NORMAL,				GET_LAST_POSITION,			&TakeNoAction,					NULL					},
//	{"TARGET_POS",						NORMAL,				GET_TARGET_POSITION,		&TakeNoAction,					NULL					},
//	{"MOUSE",							NORMAL,				GET_MOUSE,					&TakeNoAction,					NULL					},
//};
//
//static Word GetSelectionList[] = 
//{
//	{"SIZE",							NORMAL,				GET_SELECTION_SIZE,			&TakeNoAction,					NULL					},
//};
//
//static Word GetWaveList[] = 
//{
//	{"WAVE_TIME",						NORMAL,				GET_WAVE_TIME,				&TakeNoAction,					NULL					},
//	{"WAVE_TYPE",						NORMAL,				GET_WAVE_TYPE,				&TakeNoAction,					NULL					},
//};
//
//static Word GetList[] = 
//{
//	{"DISTANCE",						NORMAL,				GET_KEYWORD_DISTANCE,		&TakeNoAction,					NULL					},
//	{"STATE",							NORMAL,				GET_KEYWORD_STATE,			&TakeNoAction,					NULL					},
//	{"OBJ_TYPE",						NORMAL,				GET_KEYWORD_OBJ_TYPE,		&TakeNoAction,					NULL					},
//	{"POSITION",						NORMAL,				GET_KEYWORD_POSITION,		&TakeNoAction,					GetPositionList			},
//	{"X_VAL",							NORMAL,				GET_KEYWORD_X_VAL,			&TakeNoAction,					NULL					},
//	{"Y_VAL",							NORMAL,				GET_KEYWORD_Y_VAL,			&TakeNoAction,					NULL					},
//	{"GAME_TIME",						NORMAL,				GET_KEYWORD_GAME_TIME,		&TakeNoAction,					NULL					},
//	{"SELECTION",						NORMAL,				GET_KEYWORD_SELECTION,		&TakeNoAction,					GetSelectionList		},
//	{"WAVE",							NORMAL,				GET_KEYWORD_WAVE,			&TakeNoAction,					GetWaveList				},
//	{"TEXT",							NORMAL,				GET_KEYWORD_TEXT,			&TakeNoAction,					NULL					},
//};
//
//static Word BaseList[] = 
//{
//	{"NONE",							NORMAL,				0,							&TakeNoAction,					NULL					},
//	{"FALSE",							NORMAL,				0,							&TakeNoAction,					NULL					},
//	{"TRUE",							NORMAL,				1,							&TakeNoAction,					NULL					},
//	{"NO",								NORMAL,				0,							&TakeNoAction,					NULL					},
//	{"YES",								NORMAL,				1,							&TakeNoAction,					NULL					},
//	{"INVALID",							NORMAL,				0,							&TakeNoAction,					NULL					},
//	{"VALID",							NORMAL,				1,							&TakeNoAction,					NULL					},
//	{"INFINITE",						NORMAL,				-1,							&TakeNoAction,					NULL					},
//
//	{"FLAT",							NORMAL,				KEYWORD_FLAT,				&TakeNoAction,					NULL					},
//	{"PERCENT",							NORMAL,				KEYWORD_PERCENT,			&TakeNoAction,					NULL					},
//
//	{";",								ACTION,				KEYWORD_END,				&EndCommand,					NULL					},
//	{"IF",								ACTION,				KEYWORD_IF,					&EvaluateIf,					NULL					},
//	{"ELSE",							ACTION,				KEYWORD_ELSE,				&TakeNoAction,					NULL					},
//	{"FOR",								ACTION,				KEYWORD_FOR,				&ForLoop,						ForList					},
//	{"EVERY",							ACTION,				KEYWORD_EVERY,				&PeriodicScript,				NULL					},
//	{"STOP",							ACTION,				KEYWORD_STOP,				&StopScript,					NULL					},
//	{"MAKE",							ACTION,				KEYWORD_MAKE,				&TakeNoAction,					MakeList				},
//	{"SET",								ACTION,				KEYWORD_SET,				&ActionSet,						SetList					},
//	{"MOD",								ACTION,				KEYWORD_MOD,				&ActionMod,						NULL					},
//	{"FLAG",							ACTION,				KEYWORD_FLAG,				&ActionFlag,					NULL					},
//	{"GET",								ACTION,				KEYWORD_GET,				&ActionGet,						GetList					},
//	{"SELECT",							ACTION,				KEYWORD_SELECT,				&ActionSelect,					NULL					},
//	{"DELETE",							ACTION,				KEYWORD_DELETE,				&ActionDelete,					NULL					},
//	{"REFRESH",							ACTION,				KEYWORD_REFRESH,			&ActionRefresh,					NULL					},
//	{"TRIGGER",							ACTION,				KEYWORD_TRIGGER,			&TakeNoAction,					TriggerList				},
//	{"CUSTOM",							ACTION,				KEYWORD_CUSTOM,				&ActionCustomScript,			NULL					},
//
//	{"SELF",							POINTER,			KEYWORD_TARGET_SELF,		&TakeNoAction,					NULL					},
//	{"SELECTION",						POINTER,			KEYWORD_TARGET_SELECTION,	&TakeNoAction,					NULL					},
//	{"TARGET",							POINTER,			KEYWORD_TARGET_TARGET,		&TakeNoAction,					NULL					},
//	{"TOWER_PROJECTILE",				POINTER,			KEYWORD_TARGET_PROJECTILE,	&TakeNoAction,					NULL					},
//	{"SOURCE",							POINTER,			KEYWORD_TARGET_SOURCE,		&TakeNoAction,					NULL					},
//	{"THIS",							POINTER,			KEYWORD_TARGET_THIS,		&TakeNoAction,					NULL					},
//	{"WORLDGAME",						POINTER,			KEYWORD_TARGET_WORLDGAME,	&TakeNoAction,					NULL					},
//
//	{"==",								OPERATOR,			KEYWORD_EQUALS,				&OperatorEquals,				NULL					},
//	{"!=",								OPERATOR,			KEYWORD_NOT_EQUALS,			&OperatorNotEquals,				NULL					},
//	{"<",								OPERATOR,			KEYWORD_LESS_THAN,			&OperatorLessThan,				NULL					},
//	{"<=",								OPERATOR,			KEYWORD_LESS_OR_EQUALS,		&OperatorLessOrEqual,			NULL					},
//	{">",								OPERATOR,			KEYWORD_GREATER_THAN,		&OperatorGreaterThan,			NULL					},
//	{">=",								OPERATOR,			KEYWORD_GREATER_OR_EQUAL,	&OperatorGreaterOrEqual,		NULL					},
//	{"+",								OPERATOR,			KEYWORD_ADD,				&OperatorAdd,					NULL					},
//	{"-",								OPERATOR,			KEYWORD_SUBTRACT,			&OperatorSubtract,				NULL					},
//	{"*",								OPERATOR,			KEYWORD_MULTIPLY,			&OperatorMultiply,				NULL					},
//	{"/",								OPERATOR,			KEYWORD_DIVIDE,				&OperatorDivide,				NULL					},
//	{"/%",								OPERATOR,			KEYWORD_INT_DIVIDE,			&OperatorIntDivide,				NULL					},
//	{"%",								OPERATOR,			KEYWORD_MODULO,				&OperatorModulo,				NULL					},
//	{"=",								OPERATOR,			KEYWORD_ASSIGN,				&OperatorAssign,				NULL					},
//	{"EQUALS",							OPERATOR,			KEYWORD_EQUALS,				&OperatorEquals,				NULL					},
//	{"NOT_EQUALS",						OPERATOR,			KEYWORD_NOT_EQUALS,			&OperatorNotEquals,				NULL					},
//	{"LESS_THAN",						OPERATOR,			KEYWORD_LESS_THAN,			&OperatorLessThan,				NULL					},
//	{"LESS_OR_EQUAL",					OPERATOR,			KEYWORD_LESS_OR_EQUALS,		&OperatorLessOrEqual,			NULL					},
//	{"GREATER_THAN",					OPERATOR,			KEYWORD_GREATER_THAN,		&OperatorGreaterThan,			NULL					},
//	{"GREATER_OR_EQUAL",				OPERATOR,			KEYWORD_GREATER_OR_EQUAL,	&OperatorGreaterOrEqual,		NULL					},
//	{"ADD",								OPERATOR,			KEYWORD_ADD,				&OperatorAdd,					NULL					},
//	{"PLUS",							OPERATOR,			KEYWORD_ADD,				&OperatorAdd,					NULL					},
//	{"SUBTRACT",						OPERATOR,			KEYWORD_SUBTRACT,			&OperatorSubtract,				NULL					},
//	{"MINUS",							OPERATOR,			KEYWORD_SUBTRACT,			&OperatorSubtract,				NULL					},
//	{"MULTIPLY",						OPERATOR,			KEYWORD_MULTIPLY,			&OperatorMultiply,				NULL					},
//	{"TIMES",							OPERATOR,			KEYWORD_MULTIPLY,			&OperatorMultiply,				NULL					},
//	{"DIVIDE",							OPERATOR,			KEYWORD_DIVIDE,				&OperatorDivide,				NULL					},
//	{"INT_DIVIDE",						OPERATOR,			KEYWORD_INT_DIVIDE,			&OperatorIntDivide,				NULL					},
//	{"MODULO",							OPERATOR,			KEYWORD_MODULO,				&OperatorModulo,				NULL					},
//	{"TRUNCATE",						OPERATOR,			KEYWORD_TRUNCATE,			&OperatorTruncate,				NULL					},
//	{"ROUND",							OPERATOR,			KEYWORD_ROUND,				&OperatorRound,					NULL					},
//	{"CEILING",							OPERATOR,			KEYWORD_CEILING,			&OperatorCeiling,				NULL					},
//	{"FLOOR",							OPERATOR,			KEYWORD_FLOOR,				&OperatorFloor,					NULL					},
//	{"&&",								OPERATOR,			KEYWORD_AND,				&OperatorLogicalAnd,			NULL					},
//	{"AND",								OPERATOR,			KEYWORD_AND,				&OperatorLogicalAnd,			NULL					},
//	{"||",								OPERATOR,			KEYWORD_OR,					&OperatorLogicalOr,				NULL					},
//	{"OR",								OPERATOR,			KEYWORD_OR,					&OperatorLogicalOr,				NULL					},
//	{"!!",								OPERATOR,			KEYWORD_NOT,				&OperatorLogicalNot,			NULL					},
//	{"NOT",								OPERATOR,			KEYWORD_NOT,				&OperatorLogicalNot,			NULL					},
//	{"^^",								OPERATOR,			KEYWORD_XOR,				&OperatorLogicalXOr,			NULL					},
//	{"XOR",								OPERATOR,			KEYWORD_XOR,				&OperatorLogicalXOr,			NULL					},
//
//	{"SIN_R",							OPERATOR,			KEYWORD_SINE_R,				&OperatorSineR,					NULL					},
//	{"SIN_D",							OPERATOR,			KEYWORD_SINE_D,				&OperatorSineD,					NULL					},
//	{"COS_R",							OPERATOR,			KEYWORD_COSINE_R,			&OperatorCosineR,				NULL					},
//	{"COS_D",							OPERATOR,			KEYWORD_COSINE_D,			&OperatorCosineD,				NULL					},
//	{"TAN_R",							OPERATOR,			KEYWORD_TANGENT_R,			&OperatorTangentR,				NULL					},
//	{"TAN_D",							OPERATOR,			KEYWORD_TANGENT_D,			&OperatorTangentD,				NULL					},
//	{"ASIN_R",							OPERATOR,			KEYWORD_ARCSINE_R,			&OperatorArcSineR,				NULL					},
//	{"ASIN_D",							OPERATOR,			KEYWORD_ARCSINE_D,			&OperatorArcSineD,				NULL					},
//	{"ACOS_R",							OPERATOR,			KEYWORD_ARCCOSINE_R,		&OperatorArcCosineR,			NULL					},
//	{"ACOS_D",							OPERATOR,			KEYWORD_ARCCOSINE_D,		&OperatorArcCosineD,			NULL					},
//	{"ATAN_R",							OPERATOR,			KEYWORD_ARCTANGENT_R,		&OperatorArcTangentR,			NULL					},
//	{"ATAN_D",							OPERATOR,			KEYWORD_ARCTANGENT_D,		&OperatorArcTangentD,			NULL					},
//	{"LOG_2",							OPERATOR,			KEYWORD_LOG_2,				&OperatorLog2,					NULL					},
//	{"LOG_N",							OPERATOR,			KEYWORD_LOG_N,				&OperatorLogN,					NULL					},
//	{"LOG_E",							OPERATOR,			KEYWORD_LOG_E,				&OperatorLogE,					NULL					},
//	{"LN",								OPERATOR,			KEYWORD_LOG_E,				&OperatorLogE,					NULL					},
//	{"LOG",								OPERATOR,			KEYWORD_LOG_10,				&OperatorLog10,					NULL					},
//	{"LOG_10",							OPERATOR,			KEYWORD_LOG_10,				&OperatorLog10,					NULL					},
//	{"EXP_2",							OPERATOR,			KEYWORD_EXP_2,				&OperatorExp2,					NULL					},
//	{"EXP_N",							OPERATOR,			KEYWORD_EXP_N,				&OperatorExpN,					NULL					},
//	{"EXP_E",							OPERATOR,			KEYWORD_EXP_E,				&OperatorExpE,					NULL					},
//	{"EXP",								OPERATOR,			KEYWORD_EXP_E,				&OperatorExpE,					NULL					},
//	{"EXP_10",							OPERATOR,			KEYWORD_EXP_10,				&OperatorExp10,					NULL					},
//	{"ROOT_2",							OPERATOR,			KEYWORD_ROOT_2,				&OperatorRoot2,					NULL					},
//	{"SQRT",							OPERATOR,			KEYWORD_ROOT_2,				&OperatorRoot2,					NULL					},
//	{"SQUARE_ROOT",						OPERATOR,			KEYWORD_ROOT_2,				&OperatorRoot2,					NULL					},
//	{"ROOT_3",							OPERATOR,			KEYWORD_ROOT_3,				&OperatorRoot3,					NULL					},
//	{"CUBE_ROOT",						OPERATOR,			KEYWORD_ROOT_3,				&OperatorRoot3,					NULL					},
//	{"MIN_FIT",							OPERATOR,			KEYWORD_MIN_FIT,			&OperatorMinFit,				NULL					},
//	{"MAX_FIT",							OPERATOR,			KEYWORD_MAX_FIT,			&OperatorMaxFit,				NULL					},
//	{"INTERVAL_FIT",					OPERATOR,			KEYWORD_INTERVAL_FIT,		&OperatorIntervalFit,			NULL					},
//	{"ABS_VAL",							OPERATOR,			KEYWORD_ABS_VAL,			&OperatorAbsVal,				NULL					},
//	{"ZERO_ANGLE_D",					OPERATOR,			KEYWORD_ZERO_ANGLE_D,		&ActionGetZeroAngleD,			NULL					},
//	{"ZERO_ANGLE_R",					OPERATOR,			KEYWORD_ZERO_ANGLE_R,		&ActionGetZeroAngleR,			NULL					},
//};
//
//
//	//{KEYWORD_ACTION_MAKE_POINT2D,	"POINT2D",				ACTION,				&ActionMakePoint2D			},
//	{KEYWORD_ACTION_RANDOM,			"RAND",					ACTION,				&ActionGenerateRand			},
//	{KEYWORD_ACTION_SET_VALUE,		"SET",					ACTION,				&ActionSet					},
//	{KEYWORD_ACTION_MOD_VALUE,		"MOD",					ACTION,				&ActionMod					},
//	{KEYWORD_ACTION_FLAG_VALUE,		"FLAG",					ACTION,				&ActionFlag					},
//	{KEYWORD_ACTION_GET_VALUE,		"GET",					ACTION,				&ActionGet					},
//	//{KEYWORD_ACTION_NEW_VARIABLE,	"VARIABLE",				ACTION,				&ActionNewVariable			},
//	{KEYWORD_ACTION_SELECT,			"SELECT",				ACTION,				&ActionSelect				},
//	{KEYWORD_ACTION_DELETE,			"DELETE",				ACTION,				&ActionDelete				},
//	{KEYWORD_ACTION_REFRESH,		"REFRESH",				ACTION,				&ActionRefresh				},
//	//{KEYWORD_ACTION_FORALL,			"FORALL",				ACTION,				&ActionForAllLoop			},
//	{KEYWORD_ACTION_EVERY,			"EVERY",				ACTION,				&ActionPeriodicScript		},
//	{KEYWORD_ACTION_TRIGGER,		"TRIGGER",				ACTION,				&ActionTrigger				},
//	{KEYWORD_ACTION_CUSTOM_SCRIPT,	"CUSTOM_SCRIPT",		ACTION,				&ActionCustomScript			},
//
//	{"FALSE",								"NONE",					NORMAL,				&TakeNoAction				},
//	{"TRUE",								"FALSE",				NORMAL,				&TakeNoAction				},
//	{"NO",								"TRUE",					NORMAL,				&TakeNoAction				},
//	{"YES",								"NO",					NORMAL,				&TakeNoAction				},
//	{"INVALID",								"YES",					NORMAL,				&TakeNoAction				},
//	{"VALID",								"INVALID",				NORMAL,				&TakeNoAction				},
//	{"INFINITE",								"VALID",				NORMAL,				&TakeNoAction				},
//}
//
//Word WordList[] =
//{
//	
//
//	/*{KEYWORD_END,					";",					NORMAL,				&EndCommand					},
//	{KEYWORD_IF,					"IF",					NORMAL,				&EvaluateIf					},
//	{KEYWORD_ELSE,					"ELSE",					NORMAL,				&TakeNoAction				},
//	{KEYWORD_FOR,					"FOR",					NORMAL,				&ForLoop					},
//	{KEYWORD_STOP,					"STOP",					NORMAL,				&StopScript					},
//	{KEYWORD_FLAT,					"FLAT",					NORMAL,				&TakeNoAction				},
//	{KEYWORD_PERCENT,				"PERCENT",				NORMAL,				&TakeNoAction				},*/
//
//	{KEYWORD_ACTION_MAKE_POINT2D,	"POINT2D",				ACTION,				&ActionMakePoint2D			},
//	{KEYWORD_ACTION_RANDOM,			"RAND",					ACTION,				&ActionGenerateRand			},
//	{KEYWORD_ACTION_SET_VALUE,		"SET",					ACTION,				&ActionSet					},
//	{KEYWORD_ACTION_MOD_VALUE,		"MOD",					ACTION,				&ActionMod					},
//	{KEYWORD_ACTION_FLAG_VALUE,		"FLAG",					ACTION,				&ActionFlag					},
//	{KEYWORD_ACTION_GET_VALUE,		"GET",					ACTION,				&ActionGet					},
//	{KEYWORD_ACTION_NEW_VARIABLE,	"VARIABLE",				ACTION,				&ActionNewVariable			},
//	{KEYWORD_ACTION_SELECT,			"SELECT",				ACTION,				&ActionSelect				},
//	{KEYWORD_ACTION_DELETE,			"DELETE",				ACTION,				&ActionDelete				},
//	{KEYWORD_ACTION_REFRESH,		"REFRESH",				ACTION,				&ActionRefresh				},
//	{KEYWORD_ACTION_FORALL,			"FORALL",				ACTION,				&ActionForAllLoop			},
//	{KEYWORD_ACTION_EVERY,			"EVERY",				ACTION,				&ActionPeriodicScript		},
//	{KEYWORD_ACTION_TRIGGER,		"TRIGGER",				ACTION,				&ActionTrigger				},
//	{KEYWORD_ACTION_CUSTOM_SCRIPT,	"CUSTOM_SCRIPT",		ACTION,				&ActionCustomScript			},
//
//	{KEYWORD_DISTANCE,				"DISTANCE",				NORMAL,				&TakeNoAction				},
//	{KEYWORD_STATE,					"STATE",				NORMAL,				&TakeNoAction				},
//	{KEYWORD_BASE_TYPE,				"BASE_TYPE",			NORMAL,				&TakeNoAction				},
//	{KEYWORD_POSITION,				"POSITION",				NORMAL,				&TakeNoAction				},
//	{KEYWORD_POSITION_X,			"POS_X",				NORMAL,				&TakeNoAction				},
//	{KEYWORD_POSITION_Y,			"POS_Y",				NORMAL,				&TakeNoAction				},
//	{KEYWORD_COORDINATES,			"COORDS",				NORMAL,				&TakeNoAction				},
//	{KEYWORD_COORDS_X,				"COORDS_X",				NORMAL,				&TakeNoAction				},
//	{KEYWORD_COORDS_Y,				"COORDS_Y",				NORMAL,				&TakeNoAction				},
//	{KEYWORD_MOVE_TARGET,			"MOVE_TARGET",			NORMAL,				&TakeNoAction				},
//	{KEYWORD_LAST_POSITION,			"LAST_POS",				NORMAL,				&TakeNoAction				},
//	{KEYWORD_TARGET_POSITION,		"TARGET_POS",			NORMAL,				&TakeNoAction				},
//	{KEYWORD_MOUSE,					"MOUSE",				NORMAL,				&TakeNoAction				},
//	{KEYWORD_GRID,					"GRID",					NORMAL,				&TakeNoAction				},
//	{KEYWORD_INTERSECTION,			"INTERSECTION",			NORMAL,				&TakeNoAction				},
//	{KEYWORD_INTERSECT_POINT,		"INTERSECT_POINT",		NORMAL,				&TakeNoAction				},
//	{KEYWORD_GAME_TIME,				"GAME_TIME",			NORMAL,				&TakeNoAction				},
//	{KEYWORD_DAMAGE,				"DAMAGE",				NORMAL,				&TakeNoAction				},
//	{KEYWORD_SPELL_DAMAGE,			"SPELL_DAMAGE",			NORMAL,				&TakeNoAction				},
//	{KEYWORD_RANGE,					"RANGE",				NORMAL,				&TakeNoAction				},
//	{KEYWORD_ARMOR,					"ARMOR",				NORMAL,				&TakeNoAction				},
//	{KEYWORD_BOUNTY,				"BOUNTY",				NORMAL,				&TakeNoAction				},
//	{KEYWORD_SELECTED,				"SELECTED",				NORMAL,				&TakeNoAction				},
//	{KEYWORD_SELECTION_SIZE,		"SELECTION_SIZE",		NORMAL,				&TakeNoAction				},
//	{KEYWORD_WAVE_TYPE,				"WAVE_TYPE",			NORMAL,				&TakeNoAction				},
//	{KEYWORD_WAVE_TIME,				"WAVE_TIME",			NORMAL,				&TakeNoAction				},
//	{KEYWORD_TEXT,					"TEXT",					NORMAL,				&TakeNoAction				},
//
//	{SPELL_MODE_ACTIVE,				"ACTIVE",				SPELL_TYPE,			&TakeNoAction				},
//	{SPELL_MODE_PASSIVE,			"PASSIVE",				SPELL_TYPE,			&TakeNoAction				},
//	{SPELL_MODE_TRIGGERED,			"TRIGGERED",			SPELL_TYPE,			&TakeNoAction				},
//	{SPELL_MODE_LEARN,				"LEARN",				SPELL_TYPE,			&TakeNoAction				},
//
//	/*{KEYWORD_LOOP_TYPE_TOWER,		"TOWER",				LOOP_TYPE,			&TakeNoAction				},
//	{KEYWORD_LOOP_TYPE_PROJECTILE,	"PROJECTILE",			LOOP_TYPE,			&TakeNoAction				},
//	{KEYWORD_LOOP_TYPE_CREEP,		"CREEP",				LOOP_TYPE,			&TakeNoAction				},*/
//
//	{KEYWORD_EQUALS,				"==",					OPERATOR,			&OperatorEquals				},
//	{KEYWORD_NOT_EQUALS,			"!=",					OPERATOR,			&OperatorNotEquals			},
//	{KEYWORD_LESS_THAN,				"<",					OPERATOR,			&OperatorLessThan			},
//	{KEYWORD_LESS_OR_EQUAL,			"<=",					OPERATOR,			&OperatorLessOrEqual		},
//	{KEYWORD_GREATER_THAN,			">",					OPERATOR,			&OperatorGreaterThan		},
//	{KEYWORD_GREATER_OR_EQUAL,		">=",					OPERATOR,			&OperatorGreaterOrEqual		},
//	{KEYWORD_ADD,					"+",					OPERATOR,			&OperatorAdd				},
//	{KEYWORD_SUBTRACT,				"-",					OPERATOR,			&OperatorSubtract			},
//	{KEYWORD_MULTIPLY,				"*",					OPERATOR,			&OperatorMultiply			},
//	{KEYWORD_DIVIDE,				"/",					OPERATOR,			&OperatorDivide				},
//	{KEYWORD_INT_DIVIDE,			"//",					OPERATOR,			&OperatorIntDivide			},
//	{KEYWORD_MODULO,				"%",					OPERATOR,			&OperatorModulo				},
//	{KEYWORD_ASSIGN,				"=",					OPERATOR,			&OperatorAssign				},
//	{KEYWORD_EQUALS,				"EQUALS",				OPERATOR,			&OperatorEquals				},
//	{KEYWORD_NOT_EQUALS,			"NOT_EQUALS",			OPERATOR,			&OperatorNotEquals			},
//	{KEYWORD_LESS_THAN,				"LESS_THAN",			OPERATOR,			&OperatorLessThan			},
//	{KEYWORD_LESS_OR_EQUAL,			"LESS_OR_EQUAL",		OPERATOR,			&OperatorLessOrEqual		},
//	{KEYWORD_GREATER_THAN,			"GREATER_THAN",			OPERATOR,			&OperatorGreaterThan		},
//	{KEYWORD_GREATER_OR_EQUAL,		"GREATER_OR_EQUAL",		OPERATOR,			&OperatorGreaterOrEqual		},
//	{KEYWORD_ADD,					"ADD",					OPERATOR,			&OperatorAdd				},
//	{KEYWORD_ADD,					"PLUS",					OPERATOR,			&OperatorAdd				},
//	{KEYWORD_SUBTRACT,				"SUBTRACT",				OPERATOR,			&OperatorSubtract			},
//	{KEYWORD_SUBTRACT,				"MINUS",				OPERATOR,			&OperatorSubtract			},
//	{KEYWORD_MULTIPLY,				"MULTIPLY",				OPERATOR,			&OperatorMultiply			},
//	{KEYWORD_MULTIPLY,				"TIMES",				OPERATOR,			&OperatorMultiply			},
//	{KEYWORD_DIVIDE,				"DIVIDE",				OPERATOR,			&OperatorDivide				},
//	{KEYWORD_INT_DIVIDE,			"INT_DIVIDE",			OPERATOR,			&OperatorIntDivide			},
//	{KEYWORD_MODULO,				"MODULO",				OPERATOR,			&OperatorModulo				},
//	{KEYWORD_TRUNCATE,				"TRUNCATE",				OPERATOR,			&OperatorTruncate			},
//	{KEYWORD_ROUND,					"ROUND",				OPERATOR,			&OperatorRound				},
//	{KEYWORD_CEILING,				"CEILING",				OPERATOR,			&OperatorCeiling			},
//	{KEYWORD_FLOOR,					"FLOOR",				OPERATOR,			&OperatorFloor				},
//	{KEYWORD_AND,					"&&",					OPERATOR,			&OperatorLogicalAnd			},
//	{KEYWORD_AND,					"AND",					OPERATOR,			&OperatorLogicalAnd			},
//	{KEYWORD_OR,					"||",					OPERATOR,			&OperatorLogicalOr			},
//	{KEYWORD_OR,					"OR",					OPERATOR,			&OperatorLogicalOr			},
//	{KEYWORD_NOT,					"!!",					OPERATOR,			&OperatorLogicalNot			},
//	{KEYWORD_NOT,					"NOT",					OPERATOR,			&OperatorLogicalNot			},
//	{KEYWORD_XOR,					"^^",					OPERATOR,			&OperatorLogicalXOr			},
//	{KEYWORD_XOR,					"XOR",					OPERATOR,			&OperatorLogicalXOr			},
//
//	{KEYWORD_TARGET_SELF,			"SELF",					POINTER,			&TakeNoAction				},
//	{KEYWORD_TARGET_SELECTION,		"SELECTION",			POINTER,			&TakeNoAction				},
//	{KEYWORD_TARGET_ATTACK,			"ATTACK_TARGET",		POINTER,			&TakeNoAction				},
//	{KEYWORD_TARGET_PROJECTILE,		"TOWER_PROJECTILE",		POINTER,			&TakeNoAction				},
//	{KEYWORD_TARGET_SOURCE,			"SOURCE",				POINTER,			&TakeNoAction				},
//	{KEYWORD_TARGET_SPELL_SOURCE,	"SPELL_SOURCE",			POINTER,			&TakeNoAction				},
//	{KEYWORD_TARGET_SPELL_TARGET,	"SPELL_TARGET",			POINTER,			&TakeNoAction				},
//	{KEYWORD_TARGET_WORLDGAME,		"WORLDGAME",			POINTER,			&TakeNoAction				},
//
//	{KEYWORD_ACTION_PRINT_TEXT,		"PRINT_TEXT",			ACTION,				&ActionPrintText			},
//
//	{PROJ_TRIGGER_ON_HIT,			"ONHIT",				TRIGGER,			&TakeNoAction				},
//	{PROJ_TRIGGER_ON_EXPLODE,		"ONEXPLODE",			TRIGGER,			&TakeNoAction				},
//	{TOWER_TRIGGER_ON_FIRE,			"ONFIRE",				TRIGGER,			&TakeNoAction				},
//	{TOWER_TRIGGER_ON_UPGRADE,		"ONUPGRADE",			TRIGGER,			&TakeNoAction				},
//	{TOWER_TRIGGER_ON_SELL,			"ONSELL",				TRIGGER,			&TakeNoAction				},
//	{TOWER_TRIGGER_ON_BUILD,		"ONBUILD",				TRIGGER,			&TakeNoAction				},
//	{TOWER_TRIGGER_ON_KILL,			"ONKILL",				TRIGGER,			&TakeNoAction				},
//	{CREEP_TRIGGER_ON_DEATH,		"ONDEATH",				TRIGGER,			&TakeNoAction				},
//	{GAME_TRIGGER_ON_CLICK,			"ONCLICK",				TRIGGER,			&TakeNoAction				},
//
//	{GAME_VAR_SCREEN,				"GAME_SCREEN",			UINT32_CONST,		&TakeNoAction				},
//	{GAME_VAR_BUILD,				"GAME_BUILD",			UINT32_CONST,		&TakeNoAction				},
//	{GAME_VAR_WAVE_NUMBER,			"GAME_WAVE_NUMBER",		UINT32_CONST,		&TakeNoAction				},
//	{GAME_VAR_WAVE_SIZE,			"GAME_WAVE_SIZE",		UINT32_CONST,		&TakeNoAction				},
//	{GAME_VAR_WAVE_INTERVAL,		"GAME_WAVE_INTERVAL",	UINT32_CONST,		&TakeNoAction				},
//	{GAME_VAR_WAVE_TIME_DIFFERENCE,	"GAME_WAVE_TIME_DIF",	UINT32_CONST,		&TakeNoAction				},
//	{GAME_VAR_WAVE_PRELOAD_COUNT,	"GAME_WAVE_PRELOAD",	UINT32_CONST,		&TakeNoAction				},
//	{GAME_VAR_WAVE_TIMER,			"GAME_WAVE_TIMER",		UINT32_CONST,		&TakeNoAction				},
//	{GAME_VAR_DIFFICULTY,			"GAME_DIFFICULTY",		UINT32_CONST,		&TakeNoAction				},
//	{GAME_VAR_ACTIVE_STATE,			"GAME_ACTIVE_STATE",	UINT32_CONST,		&TakeNoAction				},
//	{GAME_VAR_INK,					"GAME_INK",				FLOAT_VAR,			&TakeNoAction				},
//	{GAME_VAR_MAX_INK,				"GAME_MAX_INK",			FLOAT_VAR,			&TakeNoAction				},
//	{GAME_VAR_SCORE,				"GAME_SCORE",			INT32_VAR,			&TakeNoAction				},
//	{GAME_VAR_LIGHT_COUNTER,		"LIGHT_COUNTER",		INT32_VAR,			&TakeNoAction				},
//	
//	{TOWER_VAR_TYPE,				"TOWER_TYPE",			UINT32_CONST,		&TakeNoAction				},
//	{TOWER_VAR_ICON_ID,				"TOWER_ICON",			UINT32_CONST,		&TakeNoAction				},
//	{TOWER_VAR_LEVEL,				"TOWER_LEVEL",			UINT32_CONST,		&TakeNoAction				},
//	{TOWER_VAR_BASE_DAMAGE,			"TOWER_BASE_DAMAGE",	UINT32_CONST,		&TakeNoAction				},
//	{TOWER_VAR_BASE_COST,			"TOWER_BASE_COST",		UINT32_CONST,		&TakeNoAction				},
//	{TOWER_VAR_BASE_RANGE,			"TOWER_BASE_RANGE",		UINT32_CONST,		&TakeNoAction				},
//	{TOWER_VAR_BASE_ATTACK_TIME,	"TOWER_BAT",			UINT32_CONST,		&TakeNoAction				},
//	{TOWER_VAR_CAN_BUILD,			"TOWER_CAN_BUILD",		UINT32_CONST,		&TakeNoAction				},
//	{TOWER_VAR_CREEP_KILLS,			"TOWER_CREEP_KILLS",	UINT32_CONST,		&TakeNoAction				},
//	{TOWER_VAR_BUILD_TIME,			"TOWER_BUILD_TIME",		UINT32_CONST,		&TakeNoAction				},
//	{TOWER_VAR_LAST_ATTACK_TIME,	"TOWER_ATTACK_TIME",	UINT32_CONST,		&TakeNoAction				},
//	{TOWER_VAR_ATTACK_TIME_DIF,		"TOWER_ATTACK_DIF",		UINT32_CONST,		&TakeNoAction				},
//	{TOWER_VAR_WAITING,				"TOWER_WAITING",		UINT32_CONST,		&TakeNoAction				},
//	{TOWER_VAR_DAMAGE_FLAT,			"TOWER_DAMAGE_FLAT",	INT32_VAR,			&TakeNoAction				},
//	{TOWER_VAR_RANGE_FLAT,			"TOWER_RANGE_FLAT",		INT32_VAR,			&TakeNoAction				},
//	{TOWER_VAR_BAT_FLAT,			"TOWER_BAT_FLAT",		INT32_VAR,			&TakeNoAction				},
//	{TOWER_VAR_CAN_FIRE,			"TOWER_CAN_FIRE",		INT32_VAR,			&TakeNoAction				},
//	{TOWER_VAR_CAN_SELL,			"TOWER_CAN_SELL",		INT32_VAR,			&TakeNoAction				},
//	{TOWER_VAR_SPECIAL_RANGE,		"TOWER_SPECIAL_RANGE",	INT32_VAR,			&TakeNoAction				},
//	{TOWER_VAR_GHOST_STATE,			"TOWER_GHOST_STATE",	INT32_VAR,			&TakeNoAction				},
//	{TOWER_VAR_SPELL_COEFFICIENT,	"TOWER_SPELL_COEFF",	FLOAT_VAR,			&TakeNoAction				},
//	{TOWER_VAR_DAMAGE_MULTIPLIER,	"TOWER_DAMAGE_BONUS",	FLOAT_VAR,			&TakeNoAction				},
//	{TOWER_VAR_RANGE_MULTIPLIER,	"TOWER_RANGE_BONUS",	FLOAT_VAR,			&TakeNoAction				},
//	{TOWER_VAR_BAT_MULTIPLIER,		"TOWER_BAT_BONUS",		FLOAT_VAR,			&TakeNoAction				},
//
//	{PROJ_VAR_BASE_TYPE,			"PROJ_TYPE",			UINT32_CONST,		&TakeNoAction				},
//	{PROJ_VAR_BASE_SIZE,			"PROJ_SIZE",			UINT32_CONST,		&TakeNoAction				},
//	{PROJ_VAR_BASE_DAMAGE,			"PROJ_BASE_DAMAGE",		UINT32_CONST,		&TakeNoAction				},
//	{PROJ_VAR_BASE_SPEED,			"PROJ_BASE_SPEED",		UINT32_CONST,		&TakeNoAction				},
//	{PROJ_VAR_BASE_SPLASH,			"PROJ_BASE_SPLASH",		UINT32_CONST,		&TakeNoAction				},
//	{PROJ_VAR_DURATION,				"PROJ_DURATION",		UINT32_CONST,		&TakeNoAction				},
//	{PROJ_VAR_ANIMATION_ID,			"PROJ_ANIMATION_ID",	UINT32_CONST,		&TakeNoAction				},
//	{PROJ_VAR_EXPLOSION_ID,			"PROJ_EXPLOSION_ID",	UINT32_CONST,		&TakeNoAction				},
//	{PROJ_VAR_START_TIME,			"PROJ_START_TIME",		UINT32_CONST,		&TakeNoAction				},
//	{PROJ_VAR_LAST_MOVE_TIME,		"PROJ_LAST_MOVE_TIME",	UINT32_CONST,		&TakeNoAction				},
//	{PROJ_VAR_DAMAGE_FLAT,			"PROJ_DAMAGE_FLAT",		INT32_VAR,			&TakeNoAction				},
//	{PROJ_VAR_SPEED_FLAT,			"PROJ_SPEED_FLAT",		INT32_VAR,			&TakeNoAction				},
//	{PROJ_VAR_SPLASH_FLAT,			"PROJ_SPLASH_FLAT",		INT32_VAR,			&TakeNoAction				},
//	{PROJ_VAR_ARMOR_IGNORE_FLAT,	"PROJ_ARMOR_IGNORE_F",	INT32_VAR,			&TakeNoAction				},
//	{PROJ_VAR_FLAGS,				"PROJ_FLAGS",			INT32_VAR,			&TakeNoAction				},
//	{PROJ_VAR_CAN_MOVE,				"PROJ_CAN_MOVE",		INT32_VAR,			&TakeNoAction				},
//	{PROJ_VAR_DAMAGE_MULTIPLIER,	"PROJ_DAMAGE_BONUS",	FLOAT_VAR,			&TakeNoAction				},
//	{PROJ_VAR_SPEED_MULTIPLIER,		"PROJ_SPEED_BONUS",		FLOAT_VAR,			&TakeNoAction				},
//	{PROJ_VAR_SPLASH_MULTIPLIER,	"PROJ_SPLASH_BONUS",	FLOAT_VAR,			&TakeNoAction				},
//	{PROJ_VAR_ARMOR_IGNORE_PERCENT,	"PROJ_ARMOR_IGNORE_P",	FLOAT_VAR,			&TakeNoAction				},
//	{PROJ_VAR_PHASE_PERCENT,		"PROJ_PHASE_PERCENT",	FLOAT_VAR,			&TakeNoAction				},
//
//	{PFLAG_COLLIDE_WITH_CREEPS,		"PFLAG_COLLIDE",		NORMAL,				&TakeNoAction				},
//	{PFLAG_INSTANTANEOUS,			"PFLAG_INSTANTANEOUS",	NORMAL,				&TakeNoAction				},
//	{PFLAG_LINEAR,					"PFLAG_LINEAR",			NORMAL,				&TakeNoAction				},
//
//	{CREEP_VAR_TYPE,				"CREEP_TYPE",			UINT32_CONST,		&TakeNoAction				},
//	{CREEP_VAR_WAVE,				"CREEP_WAVE",			UINT32_CONST,		&TakeNoAction				},
//	{CREEP_VAR_BASE_HP,				"CREEP_BASE_HP",		UINT32_CONST,		&TakeNoAction				},
//	{CREEP_VAR_BASE_ARMOR,			"CREEP_BASE_ARMOR",		UINT32_CONST,		&TakeNoAction				},
//	{CREEP_VAR_BASE_SPEED,			"CREEP_BASE_SPEED",		UINT32_CONST,		&TakeNoAction				},
//	{CREEP_VAR_BASE_BOUNTY,			"CREEP_BASE_BOUNTY",	UINT32_CONST,		&TakeNoAction				},
//	{CREEP_VAR_BASE_DAMAGE,			"CREEP_BASE_DAMAGE",	UINT32_CONST,		&TakeNoAction				},
//	{CREEP_VAR_MOVE_TIMER,			"CREEP_MOVE_TIMER",		UINT32_CONST,		&TakeNoAction				},
//	{CREEP_VAR_FLYING,				"CREEP_FLYING",			UINT32_CONST,		&TakeNoAction				},
//	{CREEP_VAR_ARMOR_FLAT,			"CREEP_ARMOR_FLAT",		INT32_VAR,			&TakeNoAction				},
//	{CREEP_VAR_SPEED_FLAT,			"CREEP_SPEED_FLAT",		INT32_VAR,			&TakeNoAction				},
//	{CREEP_VAR_BOUNTY_FLAT,			"CREEP_BOUNTY_FLAT",	INT32_VAR,			&TakeNoAction				},
//	{CREEP_VAR_CAN_MOVE,			"CREEP_CAN_MOVE",		INT32_VAR,			&TakeNoAction				},
//	{CREEP_VAR_PROJECTILE_IMMUNE,	"CREEP_PROJ_IMMUNE",	INT32_VAR,			&TakeNoAction				},
//	{CREEP_VAR_SPLASH_IMMUNE,		"CREEP_SPLASH_IMMUNE",	INT32_VAR,			&TakeNoAction				},
//	{CREEP_VAR_SPELL_IMMUNE,		"CREEP_SPELL_IMMUNE",	INT32_VAR,			&TakeNoAction				},
//	{CREEP_VAR_HITPOINTS,			"CREEP_HITPOINTS",		FLOAT_VAR,			&TakeNoAction				},
//	{CREEP_VAR_MAX_HITPOINTS,		"CREEP_MAX_HITPOINTS",	FLOAT_VAR,			&TakeNoAction				},
//	{CREEP_VAR_ARMOR_MULTIPLIER,	"CREEP_ARMOR_BONUS",	FLOAT_VAR,			&TakeNoAction				},
//	{CREEP_VAR_SPEED_MULTIPLIER,	"CREEP_SPEED_BONUS",	FLOAT_VAR,			&TakeNoAction				},
//	{CREEP_VAR_BOUNTY_MULTIPLIER,	"CREEP_BOUNTY_BONUS",	FLOAT_VAR,			&TakeNoAction				},
//	{CREEP_VAR_DAMAGE_MULTIPLIER,	"CREEP_DAMAGE_BONUS",	FLOAT_VAR,			&TakeNoAction				},
//
//	{GOBJECT_VAR_ID,				"GOBJECT_ID",			UINT32_CONST,		&TakeNoAction				},
//	{GOBJECT_VAR_START_TIME,		"GOBJECT_START_TIME",	UINT32_CONST,		&TakeNoAction				},
//	{GOBJECT_VAR_LAST_UPDATE_TIME,	"GOBJECT_UPDATE_TIME",	UINT32_CONST,		&TakeNoAction				},
//	{GOBJECT_VAR_TIME_DIFFERENCE,	"GOBJECT_TIME_DIF",		UINT32_CONST,		&TakeNoAction				},
//	{GOBJECT_VAR_ANIMATION_ID,		"GOBJECT_ANIMATION_ID",	UINT32_CONST,		&TakeNoAction				},
//	{GOBJECT_VAR_DURATION,			"GOBJECT_DURATION",		INT32_VAR,			&TakeNoAction				},
//
//	{ANIM_VAR_DURATION,				"ANIM_DURATION",		UINT32_CONST,		&TakeNoAction				},
//	{ANIM_VAR_START_TIME,			"ANIM_START_TIME",		UINT32_CONST,		&TakeNoAction				},
//	{ANIM_VAR_BASE_SCALE,			"ANIM_BASE_SCALE",		FLOAT_VAR,			&TakeNoAction				},
//	{ANIM_VAR_BASE_OPACITY,			"ANIM_BASE_OPACITY",	FLOAT_VAR,			&TakeNoAction				},
//	{ANIM_VAR_BASE_ROTATION,		"ANIM_BASE_ROTATION",	FLOAT_VAR,			&TakeNoAction				},
//
//	{KEYWORD_ACTION_ANIMATE,		"ANIMATE",				ACTION,				&ActionAnimate				},
//	{KEYWORD_ACTION_BOUNDS,			"BOUNDS",				ACTION,				&ActionSetBounds			},
//	{KEYWORD_ACTION_VAR_BOUNDS,		"VAR_BOUNDS",			ACTION,				&ActionSetVarBounds			},
//	{KEYWORD_ACTION_SCALE,			"SCALE",				ACTION,				&ActionSetScale				},
//	{KEYWORD_ACTION_OPACITY,		"OPACITY",				ACTION,				&ActionSetOpacity			},
//	{KEYWORD_ACTION_ROTATE,			"ROTATE",				ACTION,				&ActionSetRotation			},
//	{KEYWORD_ACTION_CENTER_S,		"SCALING_CENTER",		ACTION,				&ActionSetScalingCenter		},
//	{KEYWORD_ACTION_CENTER_R,		"ROTATION_CENTER",		ACTION,				&ActionSetRotationCenter	},
//	{KEYWORD_ACTION_OFFSET,			"OFFSET",				ACTION,				&ActionSetOffset			},
//	{KEYWORD_ACTION_DRAW_ANIM,		"DRAW",					ACTION,				&ActionDrawAnimation		},
//
//	{KEYWORD_ACTION_ADD_GOBJECT,	"ADD_GOBJECT",			ACTION,				&ActionAddGameObject		},
//	{KEYWORD_ACTION_REMOVE_GOBJECT,	"REMOVE_GOBJECT",		ACTION,				&ActionRemoveGameObject		},
//	{KEYWORD_ACTION_ROTATE_GOBJECT,	"ROTATE_GOBJECT",		ACTION,				&ActionRotateGameObject		},
//	{KEYWORD_ACTION_GOBJECT_VALUES,	"GOBJECT_VALUES",		ACTION,				&ActionSetGameObjectValues	},
//	{KEYWORD_ACTION_DAMAGE_AREA,	"DAMAGE_AREA",			ACTION,				&ActionDamageArea			},
//	{KEYWORD_ACTION_SPELL_AREA,		"SPELL_AREA",			ACTION,				&ActionSpellArea			},
//
//	{KEYWORD_ACTION_ADD_EMITTER,	"ADD_EMITTER",			ACTION,				&ActionAddParticleEmitter	},
//
//	{KEYWORD_ACTION_LEARN_SPELL,	"LEARN_SPELL",			ACTION,				&ActionLearnSpell			},
//	{KEYWORD_ACTION_FORGET_SPELL,	"FORGET_SPELL",			ACTION,				&ActionForgetSpell			},
//	{KEYWORD_ACTION_APPLY_SPELL,	"APPLY_SPELL",			ACTION,				&ActionApplySpell			},
//	{KEYWORD_ACTION_REMOVE_SPELL,	"REMOVE_SPELL",			ACTION,				&ActionRemoveSpell			},
//	{KEYWORD_ACTION_ACTIVE_SPELL,	"ACTIVE_SPELL",			ACTION,				&ActionActiveSpell			},
//	{KEYWORD_ACTION_KNOWS_SPELL,	"KNOWS_SPELL",			ACTION,				&ActionKnowsSpell			},
//	{KEYWORD_ACTION_SHOW_SPELL,		"SHOW_SPELL",			ACTION,				&ActionShowSpell			},
//	{KEYWORD_ACTION_CAST_SPELL,		"CAST_SPELL",			ACTION,				&ActionCastSpell			},
//	{KEYWORD_ACTION_CHECK_CAST,		"CHECK_CAST",			ACTION,				&ActionCheckCast			},
//	{KEYWORD_ACTION_SPELL_COOLDOWN,	"SPELL_COOLDOWN",		ACTION,				&ActionCheckSpellCooldown	},
//	{KEYWORD_ACTION_SPELL_CHARGES,	"SPELL_CHARGES",		ACTION,				&ActionCheckSpellCharges	},
//	{KEYWORD_ACTION_APPLY_AURA,		"APPLY_AURA",			ACTION,				&ActionApplyAura			},
//
//	{KEYWORD_ACTION_FIRE_TOWER,		"FIRE_TOWER",			ACTION,				&ActionTowerFire			},
//	{KEYWORD_ACTION_BUILD_TOWER,	"BUILD_TOWER",			ACTION,				&ActionBuildTower			},
//	{KEYWORD_ACTION_RESET_ATTACK,	"RESET_ATTACK",			ACTION,				&ActionResetAttack			},
//	{KEYWORD_ACTION_TOWER_WAIT,		"TOWER_WAIT",			ACTION,				&ActionSetTowerWait			},
//
//	{KEYWORD_ACTION_DAMAGE_CREEP,	"DAMAGE_CREEP",			ACTION,				&ActionDamageCreep			},
//
//	{SHAPE_TYPE_LINE,				"LINE",					NORMAL,				&TakeNoAction				},
//	{SHAPE_TYPE_LINE_SEGMENT,		"LINE_SEGMENT",			NORMAL,				&TakeNoAction				},
//	{SHAPE_TYPE_RECT,				"RECT",					NORMAL,				&TakeNoAction				},
//	{SHAPE_TYPE_CIRCLE,				"CIRCLE",				NORMAL,				&TakeNoAction				},
//	{SHAPE_TYPE_CONE,				"CONE",					NORMAL,				&TakeNoAction				},
//
//	{KEYWORD_SINE_R,				"SIN_R",				OPERATOR,			&OperatorSineR				},
//	{KEYWORD_SINE_D,				"SIN_D",				OPERATOR,			&OperatorSineD				},
//	{KEYWORD_COSINE_R,				"COS_R",				OPERATOR,			&OperatorCosineR			},
//	{KEYWORD_COTANGENT_D,			"COS_D",				OPERATOR,			&OperatorCosineD			},
//	{KEYWORD_TANGENT_R,				"TAN_R",				OPERATOR,			&OperatorTangentR			},
//	{KEYWORD_TANGENT_D,				"TAN_D",				OPERATOR,			&OperatorTangentD			},
//	{KEYWORD_ARCSINE_R,				"ASIN_R",				OPERATOR,			&OperatorArcSineR			},
//	{KEYWORD_ARCSINE_D,				"ASIN_D",				OPERATOR,			&OperatorArcSineD			},
//	{KEYWORD_ARCCOSINE_R,			"ACOS_R",				OPERATOR,			&OperatorArcCosineR			},
//	{KEYWORD_ARCCOSINE_D,			"ACOS_D",				OPERATOR,			&OperatorArcCosineD			},
//	{KEYWORD_ARCTANGENT_R,			"ATAN_R",				OPERATOR,			&OperatorArcTangentR		},
//	{KEYWORD_ARCTANGENT_D,			"ATAN_D",				OPERATOR,			&OperatorArcTangentD		},
//	{KEYWORD_LOG_2,					"LOG_2",				OPERATOR,			&OperatorLog2				},
//	{KEYWORD_LOG_N,					"LOG_N",				OPERATOR,			&OperatorLogN				},
//	{KEYWORD_LOG_E,					"LOG_E",				OPERATOR,			&OperatorLogE				},
//	{KEYWORD_LOG_E,					"LN",					OPERATOR,			&OperatorLogE				},
//	{KEYWORD_LOG_10,				"LOG",					OPERATOR,			&OperatorLog10				},
//	{KEYWORD_LOG_10,				"LOG_10",				OPERATOR,			&OperatorLog10				},
//	{KEYWORD_EXP_2,					"EXP_2",				OPERATOR,			&OperatorExp2				},
//	{KEYWORD_EXP_N,					"EXP_N",				OPERATOR,			&OperatorExpN				},
//	{KEYWORD_EXP_E,					"EXP_E",				OPERATOR,			&OperatorExpE				},
//	{KEYWORD_EXP_E,					"EXP",					OPERATOR,			&OperatorExpE				},
//	{KEYWORD_EXP_10,				"EXP_10",				OPERATOR,			&OperatorExp10				},
//	{KEYWORD_ROOT_2,				"ROOT_2",				OPERATOR,			&OperatorRoot2				},
//	{KEYWORD_ROOT_2,				"SQRT",					OPERATOR,			&OperatorRoot2				},
//	{KEYWORD_ROOT_2,				"SQUARE_ROOT",			OPERATOR,			&OperatorRoot2				},
//	{KEYWORD_ROOT_3,				"ROOT_3",				OPERATOR,			&OperatorRoot3				},
//	{KEYWORD_ROOT_3,				"CUBE_ROOT",			OPERATOR,			&OperatorRoot3				},
//	{KEYWORD_MIN_FIT,				"MIN_FIT",				OPERATOR,			&OperatorMinFit				},
//	{KEYWORD_MAX_FIT,				"MAX_FIT",				OPERATOR,			&OperatorMaxFit				},
//	{KEYWORD_INTERVAL_FIT,			"INTERVAL_FIT",			OPERATOR,			&OperatorIntervalFit		},
//	{KEYWORD_ABS_VAL,				"ABS_VAL",				OPERATOR,			&OperatorAbsVal				},
//	{KEYWORD_ZERO_ANGLE_D,			"ZERO_ANGLE_D",			NORMAL,				&ActionGetZeroAngleD		},
//	{KEYWORD_ZERO_ANGLE_R,			"ZERO_ANGLE_R",			NORMAL,				&ActionGetZeroAngleR		},
//};
//
//int32 LookupWord(const char * word, bool type)
//{
//	uint16 listSize = sizeof(WordList)/sizeof(Word);
//	for (uint16 i = 0; i < listSize; i++)
//	{		
//		if (strcmp(WordList[i].word, word) == 0)
//		{
//			return (type == LOOKUP_INDEX) ? i : (uint32)&WordList[i];
//		}
//	}
//	return NOT_FOUND;
//}
//
//bool VerifyArguments(Script * script, uint32 index, uint32 amount)
//{
//	for (uint32 i = 1; i <= amount; i++)
//	{
//		Word * word = script->GetOriginalWord(index + i);
//		if ((word == NULL) || ((word->value == KEYWORD_END) && (word->type == ACTION)))
//		{
//			return false;
//		}
//	}
//	return true;
//}
//
//void GetScriptText(Script * script, uint32 index, std::string& string)
//{
//	for (uint32 i = index;; i++)
//	{
//		Word * word = script->GetWord(i);
//		if ((word == NULL) || (word->value == RETURN_CONTINUE))
//		{
//			break;
//		}
//		else
//		{
//			if (word->word == NULL)
//			{
//				char * floatStr = FloatToString(word->value);
//				string.append(floatStr);
//				delete [] floatStr;
//			}
//			else if (strcmp(word->word, "_") == 0)
//			{
//				string.append(" ");
//			}
//			else
//			{
//				string.append(word->word);
//			}
//		}
//	}
//}