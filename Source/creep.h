#ifndef INC_CREEP_H
#define INC_CREEP_H

#include "type.h"
#include "utility.h"
#include "point.h"
#include "list.h"
#include "spellobject.h"

enum CreepUInt32Variables
{
	CREEP_VAR_TYPE,
	CREEP_VAR_WAVE,
	CREEP_VAR_BASE_HP,
	CREEP_VAR_BASE_ARMOR,
	CREEP_VAR_BASE_SPEED,
	CREEP_VAR_BASE_BOUNTY,
	CREEP_VAR_BASE_DAMAGE,
	CREEP_VAR_MOVE_TIMER,
	CREEP_UINT32_VARS,
};

enum CreepInt32Variables
{
	CREEP_VAR_ARMOR_FLAT,
	CREEP_VAR_SPEED_FLAT,
	CREEP_VAR_BOUNTY_FLAT,
	CREEP_VAR_CAN_MOVE,
	CREEP_VAR_PROJECTILE_IMMUNE,
	CREEP_VAR_SPLASH_IMMUNE,
	CREEP_VAR_SPELL_IMMUNE,
	CREEP_VAR_FLYING,
	CREEP_INT32_VARS
};

enum CreepFloatVariables
{
	CREEP_VAR_HITPOINTS,
	CREEP_VAR_MAX_HITPOINTS,
	CREEP_VAR_ARMOR_MULTIPLIER,
	CREEP_VAR_SPEED_MULTIPLIER,
	CREEP_VAR_BOUNTY_MULTIPLIER,
	CREEP_VAR_DAMAGE_MULTIPLIER,
	CREEP_FLOAT_VARS
};

enum CreepTypes
{
	CREEP_TYPE_NONE,
	CREEP_TYPE_CIRCLE,				//No special abilities
	CREEP_TYPE_DROP,				//Fast (2x speed)
	CREEP_TYPE_SQUARE,				//Armored (4x armor)
	CREEP_TYPE_SPIKE,				//Resistant (Takes 50% damage)
	CREEP_TYPE_TRIANGLE,			//Flying (ignores towers)
	CREEP_TYPE_DIAMOND,				//Projectile-immune
	CREEP_TYPE_OVAL,				//Splash-immune
	CREEP_TYPE_STAR,				//Spell-immune
	CREEP_TYPE_HEART,				//Sturdy (2.5x normal HP)
};

class Creep : public SpellObject
{
	public:
		Creep();
		~Creep();

		uint8 GetCreepType() const { return (uint8)GetUInt32Value(CREEP_VAR_TYPE); }
		Float GetArmor() const { return ((int32)GetUInt32Value(CREEP_VAR_BASE_ARMOR) + GetInt32Value(CREEP_VAR_ARMOR_FLAT)) * GetFloatValue(CREEP_VAR_ARMOR_MULTIPLIER); }
		Float GetSpeed() const { return ZeroFit((int32)GetUInt32Value(CREEP_VAR_BASE_SPEED) + GetInt32Value(CREEP_VAR_SPEED_FLAT)) * GetFloatValue(CREEP_VAR_SPEED_MULTIPLIER); }
		Float GetBounty() const { return ZeroFit((int32)GetUInt32Value(CREEP_VAR_BASE_BOUNTY) + GetInt32Value(CREEP_VAR_BOUNTY_FLAT)) * GetFloatValue(CREEP_VAR_BOUNTY_MULTIPLIER); }
		Float GetDamageMultiplier() const { return ZeroFit((int32)GetUInt32Value(CREEP_VAR_BASE_DAMAGE)/1000.0 * GetFloatValue(CREEP_VAR_DAMAGE_MULTIPLIER)); }
		Float GetHitpoints() const { return GetFloatValue(CREEP_VAR_HITPOINTS); }

		bool InflictDamage(Float damage, Float armorIgnore);
		void CalculateNextMove(const Point2D& location);

		const Point2D& GetMoveTarget() const { return moveTarget; }
		const Point2D& GetLastPos() const { return lastPos; }

		void SetMoveTarget(const Point2D& moveTarget) { this->moveTarget = moveTarget; }

		void Move(uint32 time);

		void Clear();

		void Unload();
	private:
		Point2D lastPos;
		Point2D moveTarget;
};

List<Creep *> * GetActiveCreeps();
void ClearActiveCreeps();

void CreepMaintenance();
//void UnloadCreepData();
void AddCreep();

#endif