#ifndef INC_PARTICLE_H
#define INC_PARTICLE_H

#include "type.h"
#include "point.h"
#include "vector.h"
#include "object.h"
#include "list.h"
#include "color.h"
#include "animation.h"
#include "emitter.h"

#define PARTICLE_POOL_SIZE 10000

//enum EffectTypes
//{
//	EFFECT_COLOR_CHANGE,
//	EFFECT_SCALE,
//	EFFECT_ROTATE
//};
//
//struct ParticleEffect
//{
//	uint8 changeType;
//	Float amount;
//	uint32 duration;
//	uint32 startTime;
//};

enum ParticleVars
{
	PARTICLE_VAR_GUID,
	PARTICLE_VAR_DURATION,
	PARTICLE_VAR_START_TIME,
	PARTICLE_VAR_LAST_UPDATE_TIME,
	PARTICLE_UINT32_VARS
};

class Emitter;
class Particle : public Object
{
	public:
		Particle();
		~Particle();

		//void AddParticleEffect(const ParticleEffect& effect);

		bool SetValues(const Emitter * emitter);

		void Update();
		void Unload();

	private:
		const Emitter * emitter;
		
		Vector3D direction;
		ColorARGB startColor, endColor;
		Animation * animation;
		//List<ParticleEffect *> effects;	
};

void LoadParticlePool();
void UnloadParticlePool();

Particle * GetParticleFromPool();

#endif