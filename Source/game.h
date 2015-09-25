#ifndef INC_GAME_H
#define INC_GAME_H

#include "type.h"
#include "math.h"
#include "spellobject.h"
#include "selection.h"
#include "grid.h"

enum Screens
{
	SCREEN_MAIN_MENU,
	SCREEN_GAMEPLAY
};

enum ActiveStates
{
	ACTIVE_STATE_NORMAL,
	ACTIVE_STATE_ATTACK
};

enum GameInt32Variables
{
	GAME_VAR_SCORE,
	GAME_VAR_LIGHT_COUNTER,
	GAME_VAR_PREV_LIGHT_COUNTER,
	GAME_INT32_VARS,
};

enum GameUInt32Variables
{
	GAME_VAR_SCREEN,
	GAME_VAR_BUILD,
	GAME_VAR_WAVE_NUMBER,
	GAME_VAR_WAVE_SIZE,
	GAME_VAR_WAVE_INTERVAL,
	GAME_VAR_WAVE_TIME_DIFFERENCE,
	GAME_VAR_WAVE_PRELOAD_COUNT,
	GAME_VAR_CAN_SELL,
	GAME_VAR_CAN_UPGRADE,
	GAME_VAR_GLOBAL_SELL_TIMER,
	GAME_VAR_SELL_TIMER_INCREMENT,
	GAME_VAR_WAVE_TIMER,
	GAME_VAR_DIFFICULTY,
	GAME_VAR_INSTANT_CAST_FLAG,
	GAME_VAR_ACTIVE_STATE,
	GAME_UINT32_VARS
};

enum GameFloatVariables
{
	GAME_VAR_INK,
	GAME_VAR_MAX_INK,
	GAME_VAR_MAX_INK_INCREMENT,
	GAME_VAR_BASE_CREEP_HP,
	GAME_VAR_CREEP_HP_SCALE,
	GAME_VAR_CREEP_ARMOR_SCALE,
	GAME_VAR_CREEP_BOUNTY_SCALE,
	GAME_VAR_LC_MULTIPLIER_DAMAGE,
	GAME_VAR_LC_MULTIPLIER_BOUNTY,
	GAME_FLOAT_VARS,
};

class Game : public SpellObject
{
	public:
		uint8 GetScreen() const { return GetUInt32Value(GAME_VAR_SCREEN); }
		uint8 GetBuild() const { return GetUInt32Value(GAME_VAR_BUILD); }
		Float GetInk() const { return GetFloatValue(GAME_VAR_INK); }
		Float GetMaxInk() const { return GetFloatValue(GAME_VAR_MAX_INK) + (ZeroFit(50 - GetInt32Value(GAME_VAR_LIGHT_COUNTER)) * 100); }

		const char * GetAssetsPath() const { return assetsPath; }

		Grid * GetGrid() { return &grid; }

		Tower ** GetGridData() { return grid.GetData(); }
		//Tower * GetGridTower(const Point2D& coords) const { return grid.GetData(coords); }

		uint8 * GetPathData() { return grid.GetPath(); }
		//uint8 GetGridPath(const Point2D& coords) const { return grid.GetPath(coords); }

		Selection * GetSelection() { return &selection; }
		Selection * GetHotkey(uint8 ID) { return (ID < 10) ? &hotkeys[ID] : NULL; }

		void AddInk(Float amount) { ModFloatValue(GAME_VAR_INK, IntervalFit(amount, -GetInk(), GetMaxInk() - GetInk())); }

		void SetBuild(uint8 towerType) { SetUInt32Value(GAME_VAR_BUILD, towerType); selection.Clear(); }
		void SetHotkey(uint8 hotkey) { hotkeys[hotkey].Copy(selection); }
		void SetSelection(const Selection& selection) { this->selection.Copy(selection); }
		//void SetGameTarget(SpellObject * target) { this->target = target; }

		void ClearBuild() { SetBuild(0); }
		void ClearSelection() { selection.Clear(); }

		void ClearHotkeys();
		void ClearGridPath() { grid.ClearPath(0); }

		void Clear() {};

		friend bool LoadGame(const char * filename);
		friend void UnloadGame();
	private:
		char * assetsPath;
		Selection selection;
		Selection hotkeys[10];
		Grid grid;

		//SpellObject * target;
};

extern Game WorldGame;

uint32 GetGameTime();

void InitGame();
void UpdateGame();

#endif