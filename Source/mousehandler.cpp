#include "type.h"
#include "rect.h"
#include "keydef.h"
#include "mousedef.h"
#include "mousehandler.h"

using namespace MouseNamespace;

MouseHandler MouseHandlerTable1024[MOUSE_HANDLER_AMOUNT] = 
{
	{MOUSE_HANDLER_GAME_ONCLICK_TRIGGER,		NULL,			FLAG_LEFT | FLAG_RIGHT,		DOWN,	Rect(0, 0, 1024, 768),			&OnClickTrigger				},
	{MOUSE_HANDLER_ACTIVE_STATE_ATTACK,			NULL,			FLAG_LEFT,					DOWN,	Rect(113, 0, 685, 768),			&ActiveStateAttack			},
	{MOUSE_HANDLER_CLEAR_ACTIVE_STATE,			NULL,			FLAG_LEFT | FLAG_RIGHT,		DOWN,	Rect(0, 0, 1024, 768),			&ClearActiveState			},
	{MOUSE_HANDLER_BUILD_TOWER,					NULL,			FLAG_LEFT,					DOWN,	Rect(113, 113, 685, 685),		&BuildTower					},
	{MOUSE_HANDLER_TOGGLE_SELECT_TOWER,			MODIFIER_SHIFT,	FLAG_LEFT,					DOWN,	Rect(113, 113, 685, 685),		&ToggleSelectTower			},
	{MOUSE_HANDLER_NEW_SELECT_TOWER,			NULL,			FLAG_LEFT,					DOWN,	Rect(113, 113, 685, 685),		&NewSelectTower				},
	{MOUSE_HANDLER_TOGGLE_INFOBOX_SELECT,		MODIFIER_SHIFT,	FLAG_LEFT,					DOWN,	Rect(792, 561, 1002, 723),		&ToggleInfoboxSelection		},
	{MOUSE_HANDLER_NEW_INFOBOX_SELECT,			NULL,			FLAG_LEFT,					DOWN,	Rect(792, 561, 1002, 723),		&NewInfoboxSelection		},
	{MOUSE_HANDLER_CAST_INFOBOX_SPELL,			NULL,			FLAG_LEFT,					DOWN,	Rect(792, 618, 1002, 723),		&CastInfoboxSpell			},
	{MOUSE_HANDLER_SET_ACTIVE_STATE_ATTACK,		NULL,			FLAG_LEFT,					DOWN,	Rect(846, 561, 894, 609),		&SetActiveStateAttack		},
	{MOUSE_HANDLER_TOWER_WAIT,					NULL,			FLAG_LEFT,					DOWN,	Rect(954, 561, 1002, 609),		&ToggleTowerWait			},
	{MOUSE_HANDLER_BUILD_SELECT,				NULL,			FLAG_LEFT,					DOWN,	Rect(789, 303, 1005, 495),		&BuildSelect				},
	{MOUSE_HANDLER_TAKE_NO_ACTION,				NULL,			FLAG_LEFT,					DOWN,	Rect(786, 552, 1005, 725),		&TakeNoAction				},
	{MOUSE_HANDLER_RIGHT_CLICK_ATTACK,			NULL,			FLAG_RIGHT,					DOWN,	Rect(113, 0, 685, 768),			&SelectionAttack			},
	{MOUSE_HANDLER_BUILD_NO_ACTION,				NULL,			FLAG_LEFT,					DOWN,	Rect(0, 0, 1024, 768),			&BuildNoAction				},
	{MOUSE_HANDLER_CLEAR_BUILD,					NULL,			FLAG_RIGHT,					DOWN,	Rect(0, 0, 1024, 768),			&ClearBuildSelection		},
};

MouseHandler * MouseHandlerTable = MouseHandlerTable1024;