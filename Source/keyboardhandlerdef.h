#ifndef INC_KEYBOARD_HANDLER_DEFS_H
#define INC_KEYBOARD_HANDLER_DEFS_H

enum KeyboardHandlerFunctions
{
	KEYBOARD_HANDLER_ESC_EXIT,
	KEYBOARD_HANDLER_INIT_WAVES,
	KEYBOARD_HANDLER_SEND_NEXT_WAVE,
	KEYBOARD_HANDLER_CLEAR_ACTIVE_STATE,
	KEYBOARD_HANDLER_SET_ACTIVE_STATE_ATTACK,
	KEYBOARD_HANDLER_TOWER_WAIT,
	KEYBOARD_HANDLER_NUMBER_HOTKEYS,
	KEYBOARD_HANDLER_SINGLE_TOWER_HOTKEYS,
	KEYBOARD_HANDLER_TOWER_HOTKEYS,
	KEYBOARD_HANDLER_CLEAR_SELECTION,
	KEYBOARD_HANDLER_DELETE,
	KEYBOARD_HANDLER_AMOUNT
};

enum SpecialHandlerFunctions
{
	KEYBOARD_HANDLER_ASSIGN_SELECTION,
	KEYBOARD_HANDLER_GET_HOTKEY_SELECTION,
	SPECIAL_HANDLER_ALT_F4_EXIT,
	SPECIAL_HANDLER_AMOUNT
};

namespace KeyboardNamespace
{
	bool TakeNoAction();
	bool ExitKey();

	bool InitWaves();
	bool SendNextWave();

	bool ClearActiveState();
	bool SetActiveStateAttack();

	bool ToggleTowerWait();

	bool AssignSelection();
	bool GetHotkeySelection();

	bool NumberHotkeys();
	bool SingleTowerSpellHotkeys();
	bool TowerHotkeys();
	bool Delete();

	bool ClearSelection();
}

#endif