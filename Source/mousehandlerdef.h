#ifndef INC_MOUSE_HANDLER_DEFS_H
#define INC_MOUSE_HANDLER_DEFS_H

enum MouseHandlerFunctions
{
	MOUSE_HANDLER_TAKE_NO_ACTION,
	MOUSE_HANDLER_GAME_ONCLICK_TRIGGER,
	MOUSE_HANDLER_CLEAR_ACTIVE_STATE,
	MOUSE_HANDLER_ACTIVE_STATE_ATTACK,
	MOUSE_HANDLER_SET_ACTIVE_STATE_ATTACK,
	MOUSE_HANDLER_TOWER_WAIT,
	MOUSE_HANDLER_BUILD_TOWER,
	MOUSE_HANDLER_TOGGLE_SELECT_TOWER,
	MOUSE_HANDLER_NEW_SELECT_TOWER,
	MOUSE_HANDLER_TOGGLE_INFOBOX_SELECT,
	MOUSE_HANDLER_NEW_INFOBOX_SELECT,
	MOUSE_HANDLER_CAST_INFOBOX_SPELL,
	MOUSE_HANDLER_BUILD_SELECT,
	MOUSE_HANDLER_RIGHT_CLICK_ATTACK,
	MOUSE_HANDLER_BUILD_NO_ACTION,
	MOUSE_HANDLER_CLEAR_BUILD,
	MOUSE_HANDLER_AMOUNT
};

namespace MouseNamespace
{
	bool TakeNoAction(uint8 button, const Point2D& position);

	bool OnClickTrigger(uint8 button, const Point2D& position);
	bool SelectionAttack(uint8 button, const Point2D& position);
	bool ClearActiveState(uint8 button, const Point2D& position);
	bool ActiveStateAttack(uint8 button, const Point2D& position);
	bool SetActiveStateAttack(uint8 button, const Point2D& position);

	bool BuildTower(uint8 button, const Point2D& position);
	bool ToggleSelectTower(uint8 button, const Point2D& position);
	bool NewSelectTower(uint8 button, const Point2D& position);
	bool ToggleInfoboxSelection(uint8 button, const Point2D& position);
	bool NewInfoboxSelection(uint8 button, const Point2D& position);
	bool BuildNoAction(uint8 button, const Point2D& position);
	bool ToggleTowerWait(uint8 button, const Point2D& position);

	bool CastInfoboxSpell(uint8 button, const Point2D& position);
	bool BuildSelect(uint8 button, const Point2D& position);
	bool ClearBuildSelection(uint8 button, const Point2D& position);
}

#endif