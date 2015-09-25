#ifndef INC_EMITTER_H
#define INC_EMITTER_H

#include "type.h"
#include "vector.h"
#include "object.h"
#include "shape.h"
#include "list.h"
#include "color.h"
#include "animation.h"
#include "particle.h"

enum EmitterUInt32Vars
{
	EMITTER_VAR_ID,
	EMITTER_VAR_SURFACE_ID,
	EMITTER_VAR_LIMIT,
	EMITTER_VAR_DURATION,
	EMITTER_VAR_EMITS_PER_SEC,
	EMITTER_VAR_EPS_VARIANCE,
	EMITTER_VAR_PARTICLES_PER_EMIT,
	EMITTER_VAR_PPE_VARIANCE,
	EMITTER_VAR_PARTICLE_DURATION,
	EMITTER_VAR_PARTICLE_DURATION_VARIANCE,
	EMITTER_VAR_SPAWN_DIST_MIN,
	EMITTER_VAR_SPAWN_DIST_MAX,
	EMITTER_VAR_GRAVITY_FLAG,
	EMITTER_VAR_START_TIME,
	EMITTER_VAR_LAST_EMIT_TIME,
	EMITTER_UINT32_VARS
};

enum EmitterFloatVars
{
	EMITTER_VAR_SPEED,
	EMITTER_VAR_SPEED_VARIANCE,
	EMITTER_VAR_GRAVITY_MAGNITUDE,
	EMITTER_FLOAT_VARS
};

class Particle;
class Emitter : public Object
{
	public:
		Emitter();
		~Emitter();

		uint32 GetEmitsPerSecond() const;
		uint32 GetParticlesPerEmit() const;
		uint32 GetParticleDuration() const;
		Float GetSpeedMultiplier() const;

		Emitter * AddEmitter(Object * source, const Point2D& position) const;

		bool AddParticle();
		void Update();

		void Unload();

		friend class Particle;
		friend bool LoadEmitterData();
	private:
		const Animation * animation;
		Shape * surface;
		List<Particle *> particles;
		ColorARGB startColor, startColorVariance;
		ColorARGB endColor, endColorVariance;
		Vector3D direction, directionVariance;
		Vector3D force;
		Point3D gravity;
};

bool LoadEmitterData();
void UnloadEmitterData();

const Emitter * GetEmitterData();
uint32 GetEmitterDataSize();

void EmitterMaintenance();

List<Emitter *> * GetActiveEmitters();
void ClearActiveEmitters();

#endif