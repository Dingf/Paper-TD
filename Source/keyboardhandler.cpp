#include "type.h"
#include "keydef.h"
#include "keyboardhandler.h"

using namespace KeyboardNamespace;

KeyboardHandler KeyboardHandlerTable[KEYBOARD_HANDLER_AMOUNT] =
{
	{KEYBOARD_HANDLER_ESC_EXIT,					NULL,				KEY_ESCAPE,			KEY_ESCAPE,			KEY_DOWN,	&ExitKey					},
	{KEYBOARD_HANDLER_INIT_WAVES,				NULL,				' ',				' ',				KEY_DOWN,	&InitWaves					},
	{KEYBOARD_HANDLER_SEND_NEXT_WAVE,			NULL,				' ',				' ',				KEY_DOWN,	&SendNextWave				},
	{KEYBOARD_HANDLER_SET_ACTIVE_STATE_ATTACK,	NULL,				'a',				'a',				KEY_DOWN,	&SetActiveStateAttack		},
	{KEYBOARD_HANDLER_CLEAR_ACTIVE_STATE,		NULL,				0,					255,				KEY_DOWN,	&ClearActiveState			},
	{KEYBOARD_HANDLER_TOWER_WAIT,				NULL,				'w',				'w',				KEY_DOWN,	&ToggleTowerWait			},
	{KEYBOARD_HANDLER_NUMBER_HOTKEYS,			NULL,				'1',				'9',				KEY_DOWN,	&NumberHotkeys				},
	{KEYBOARD_HANDLER_SINGLE_TOWER_HOTKEYS,		NULL,				'a',				'z',				KEY_DOWN,	&SingleTowerSpellHotkeys	},
	{KEYBOARD_HANDLER_TOWER_HOTKEYS,			NULL,				'a',				'z',				KEY_DOWN,	&TowerHotkeys				},
	{KEYBOARD_HANDLER_DELETE,					NULL,				'x',				'x',				KEY_DOWN,	&Delete						},
	//{KEYBOARD_HANDLER_CLEAR_SELECTION,			NULL,				0,					255,				KEY_DOWN,	&ClearSelection				},
};

KeyboardHandler SpecialHandlerTable[SPECIAL_HANDLER_AMOUNT] =
{
	{KEYBOARD_HANDLER_ASSIGN_SELECTION,			MODIFIER_CTRL,		1,					8,					KEY_DOWN,	&AssignSelection			},
	{KEYBOARD_HANDLER_GET_HOTKEY_SELECTION,		NULL,				1,					8,					KEY_DOWN,	&GetHotkeySelection			},
	{SPECIAL_HANDLER_ALT_F4_EXIT,				MODIFIER_ALT,		KEY_F4,				KEY_F4,				KEY_DOWN,	&ExitKey					},
};