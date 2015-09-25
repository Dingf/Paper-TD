#ifndef INC_TOWER_H
#define INC_TOWER_H

#include "type.h"
#include "point.h"
#include "list.h"
#include "spell.h"
#include "spellobject.h"
#include "creep.h"
#include "projectile.h"

#define MAX_TOWER_LEVEL 5

enum TowerInt32Variables
{
	TOWER_VAR_DAMAGE_FLAT,
	TOWER_VAR_RANGE_FLAT,
	TOWER_VAR_BAT_FLAT,
	TOWER_VAR_CAN_FIRE,
	TOWER_VAR_CAN_SELL,
	TOWER_VAR_SPECIAL_RANGE,	//1 == infinite, 2 == no range, 0 == no special range
	TOWER_VAR_GHOST_STATE,
	TOWER_INT32_VARS
};

enum TowerUInt32Variables
{
	TOWER_VAR_TYPE,
	TOWER_VAR_ICON_ID,
	TOWER_VAR_LEVEL,
	TOWER_VAR_BASE_DAMAGE,
	TOWER_VAR_BASE_COST,
	TOWER_VAR_BASE_RANGE,
	TOWER_VAR_BASE_ATTACK_TIME,
	TOWER_VAR_CAN_BUILD,
	TOWER_VAR_CREEP_KILLS,
	TOWER_VAR_BUILD_TIME,
	TOWER_VAR_LAST_ATTACK_TIME,
	TOWER_VAR_ATTACK_TIME_DIF,
	TOWER_VAR_WAITING,
	TOWER_VAR_PROJECTILE_ID,
	TOWER_UINT32_VARS
};

enum TowerFloatVariables
{
	TOWER_VAR_SPELL_COEFFICIENT,
	TOWER_VAR_DAMAGE_MULTIPLIER,
	TOWER_VAR_RANGE_MULTIPLIER,
	TOWER_VAR_BAT_MULTIPLIER,
	TOWER_FLOAT_VARS
};
//
//enum TowerSpellLists
//{
//	TOWER_SPELL_LIST_UPGRADE,
//	TOWER_SPELL_LIST_KNOWN
//};

enum TowerBaseUpgrades
{
	UPGRADE_DAMAGE,
	UPGRADE_RANGE,
	UPGRADE_BAT,
	UPGRADE_AMOUNT
};

class Projectile;
class Tower: public SpellObject
{
	public:
		Tower();
		~Tower();

		uint32 GetTowerType() const { return GetUInt32Value(TOWER_VAR_TYPE); }
		uint32 GetTowerLevel() const { return GetUInt32Value(TOWER_VAR_LEVEL); }
		Float GetDamage() const { return ((int32)GetUInt32Value(TOWER_VAR_BASE_DAMAGE) + GetInt32Value(TOWER_VAR_DAMAGE_FLAT)) * GetFloatValue(TOWER_VAR_DAMAGE_MULTIPLIER); }
		Float GetSpellDamage() const { return (GetDamage() * GetFloatValue(TOWER_VAR_SPELL_COEFFICIENT)); }
		Float GetRange() const { return ((int32)GetUInt32Value(TOWER_VAR_BASE_RANGE) + GetInt32Value(TOWER_VAR_RANGE_FLAT)) * GetFloatValue(TOWER_VAR_RANGE_MULTIPLIER); }
		Float GetAttackTime() const { return ((int32)GetUInt32Value(TOWER_VAR_BASE_ATTACK_TIME) + GetInt32Value(TOWER_VAR_BAT_FLAT)) * GetFloatValue(TOWER_VAR_BAT_MULTIPLIER); }

		      Creep * GetTarget()       { return target; }
		const Creep * GetTarget() const { return target; }

		      Projectile * GetProjectile()       { return projectile; }
		const Projectile * GetProjectile() const { return projectile; }

			  List<Spell *> * GetUpgradeSpells()       { return &upgradeSpells; }
		const List<Spell *> * GetUpgradeSpells() const { return &upgradeSpells; }

		Spell * GetVisibleSpell(bool row, uint8 col);

		void ClearAttackTarget() { target = NULL; }

		void PerformAction();

		void UpdateState();
		void AcquireAttackTarget();
		bool SetAttackTarget(Creep * creep);

		void AddKill();

		void FireProjectile();

		//bool CheckSpellPrerequisites(uint32 spellID) const;
		void ClearUpgradeSpells();
		void RefreshUpgradeSpells();
		//bool ActivateSpell(bool spellList, uint16 spellID);
	/*	bool ApplySpell(uint16 spellID);
		bool LearnSpell(uint16 spellID);
		bool UnlearnSpell(uint16 spellID);*/
		void OnCast(Spell * spell);
		void Clear();

		void Copy(const Tower& obj);

		void Unload();

	protected:
		Projectile * projectile;
		Creep * target;
		//Spell * castTarget;

		List<Spell *> upgradeSpells;	
};

class TowerBase : public Tower
{
	public:
		TowerBase() : Tower() { description = NULL; }
		~TowerBase();

		uint8 GetHotkey() const { return hotkey; }
		int32 GetUpgradeStats(uint8 type, uint8 index) const { return (((type < UPGRADE_AMOUNT) && (index < MAX_TOWER_LEVEL - 1)) ? upgradeStats[type][index] : 0); }

		const char * GetDescription() const { return description; }

		friend bool LoadTowerData();

	private:
		char * description;
		int32 upgradeStats[UPGRADE_AMOUNT][MAX_TOWER_LEVEL-1];
		uint8 hotkey;
};

bool LoadTowerData();
void UnloadTowerData();

const TowerBase * GetTowerData();
uint32 GetTowerDataSize();

void TowerMaintenance();
void RefreshActiveTowers();

bool AddTowerKill(Object * object);

bool BuildTower(uint8 type, const Point2D& coords, bool active);

#endif