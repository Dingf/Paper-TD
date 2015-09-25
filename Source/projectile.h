#ifndef INC_PROJECTILE_H
#define INC_PROJECTILE_H

#include "type.h"
#include "point.h"
#include "list.h"
#include "script.h"
#include "animation.h"
#include "spellobject.h"
#include "tower.h"
#include "creep.h"

enum ProjectileUInt32Variables
{
	PROJ_VAR_BASE_TYPE,
	PROJ_VAR_BASE_SIZE,
	PROJ_VAR_BASE_DAMAGE,
	PROJ_VAR_BASE_SPEED,
	PROJ_VAR_BASE_SPLASH,
	PROJ_VAR_DURATION,
	PROJ_VAR_ANIMATION_ID,
	PROJ_VAR_EXPLOSION_ID,
	PROJ_VAR_START_TIME,
	PROJ_VAR_LAST_MOVE_TIME,
	PROJ_UINT32_VARS
};

enum ProjectileInt32Variables
{
	PROJ_VAR_DAMAGE_FLAT,
	PROJ_VAR_SPEED_FLAT,
	PROJ_VAR_SPLASH_FLAT,
	PROJ_VAR_ARMOR_IGNORE_FLAT,
	PROJ_VAR_FLAGS,
	PROJ_VAR_CAN_MOVE,
	PROJ_INT32_VARS
};

enum ProjectileFloatVariables
{
	PROJ_VAR_DAMAGE_MULTIPLIER,
	PROJ_VAR_SPEED_MULTIPLIER,
	PROJ_VAR_SPLASH_MULTIPLIER,
	PROJ_VAR_ARMOR_IGNORE_PERCENT,
	PROJ_VAR_PHASE_PERCENT,
	PROJ_FLOAT_VARS
};

enum ProjectileFlags
{
	PFLAG_COLLIDE_WITH_CREEPS = 1 << 0,
	PFLAG_INSTANTANEOUS = 1 << 1,
	PFLAG_LINEAR = 1 << 2,
};

enum ProjectileAnimations
{
	PROJ_ANIM_NORMAL,
	PROJ_ANIM_EXPLOSION
};

class Tower;
class Projectile: public SpellObject
{
	public:
		Projectile();
		Projectile(const Projectile& projectile);
		~Projectile();

		Float GetDamage() const { return (GetUInt32Value(PROJ_VAR_BASE_DAMAGE) + GetInt32Value(PROJ_VAR_DAMAGE_FLAT)) * GetFloatValue(PROJ_VAR_DAMAGE_MULTIPLIER); }
		Float GetSpeed() const { return (GetUInt32Value(PROJ_VAR_BASE_SPEED) + GetInt32Value(PROJ_VAR_SPEED_FLAT)) * GetFloatValue(PROJ_VAR_SPEED_MULTIPLIER); }
		Float GetSplash() const { return (GetUInt32Value(PROJ_VAR_BASE_SPLASH) + GetInt32Value(PROJ_VAR_SPLASH_FLAT)) * GetFloatValue(PROJ_VAR_SPLASH_MULTIPLIER); }
		Float GetArmorIgnore(const Creep * creep) const;

			  Point2D& GetTargetPos()       { return targetPos; }
		const Point2D& GetTargetPos() const { return targetPos; }

			  Creep * GetTarget()       { return target; }
		const Creep * GetTarget() const { return target; }
		      Tower * GetSource()       { return source; }
		const Tower * GetSource() const { return source; }

		      Animation * GetAnimation()       { return animation; }
		const Animation * GetAnimation() const { return animation; }
		      Animation * GetExplosion()       { return explosion; }
		const Animation * GetExplosion() const { return explosion; }

		bool AddAnimation(bool animationType);

		void Move(uint32 time);

		void DamageCreep(Creep * creep, bool phase);
		void Explode();

		void SetSource(Tower * source) { this->source = source; }
		void SetTarget(Creep * target) { this->target = target; }
		void SetTarget(Point2D position) { target = NULL; targetPos = position; }

		void Clear();

		void Copy(const Projectile& obj);
		void Unload();
	private:
		Point2D targetPos;		//If creep is destroyed or is unloaded, projectile still goes to where that creep was (for splash and such)
		Creep * target;
		Tower * source;
		Animation * animation;
		Animation * explosion;
};

bool LoadProjectileData();
void UnloadProjectileData();

const Projectile * GetProjectileData();
uint32 GetProjectileDataSize();

List<Projectile *> * GetActiveProjectiles();
void ClearActiveProjectiles();

void ProjectileMaintenance();
bool AddProjectile(Tower * tower);

#endif