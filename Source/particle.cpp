#include "type.h"
#include "math.h"
#include "rand.h"
#include "utility.h"
#include "point.h"
#include "vector.h"
#include "object.h"
#include "list.h"
#include "color.h"
#include "sprite.h"
#include "particle.h"
#include "emitter.h"
#include "game.h"

List<Particle *> ParticlePool;

Particle::Particle()
{
	SetState(false);
	AllocateUInt32Values(PARTICLE_UINT32_VARS);
}

Particle::~Particle()
{
	SetState(false);
}

//void Particle::AddParticleEffect(const ParticleEffect& effect)
//{
//}

bool Particle::SetValues(const Emitter * emitter)
{
	if ((emitter != NULL) && (emitter->GetState() == true))
	{
		SetState(true);
		animation = emitter->animation->AddAnimation(this);

		if (animation == NULL)
		{
			Unload();
			return false;
		}

		Float spawnMin = emitter->GetUInt32Value(EMITTER_VAR_SPAWN_DIST_MIN);
		Float spawnMax = emitter->GetUInt32Value(EMITTER_VAR_SPAWN_DIST_MAX);

		Float dx = 0;
		Float dy = 0;

		if (spawnMin != spawnMax)
		{
			Float distance = RandomRange(spawnMin, spawnMax);
			Float angle = RandomRange(0, 360);

			dy = SinD(angle) * distance;
			dx = CosD(angle) * distance;
		}

		position = emitter->surface->GetRandomPoint();
		position += emitter->position;
		position.SetValues(position.GetX() + dx, position.GetY() + dy);

		direction.SetValues(emitter->direction.GetX() + (RandomRange(-1.0, 1.0) * emitter->directionVariance.GetX()),
							emitter->direction.GetY() + (RandomRange(-1.0, 1.0) * emitter->directionVariance.GetY()),
							emitter->direction.GetZ() + (RandomRange(-1.0, 1.0) * emitter->directionVariance.GetZ()));

		direction *= emitter->GetSpeedMultiplier();

		Float a = emitter->startColor.GetA() + (RandomRange(-1.0, 1.0) * emitter->startColorVariance.GetA());
		Float r = emitter->startColor.GetR() + (RandomRange(-1.0, 1.0) * emitter->startColorVariance.GetR());
		Float g = emitter->startColor.GetG() + (RandomRange(-1.0, 1.0) * emitter->startColorVariance.GetG());
		Float b = emitter->startColor.GetB() + (RandomRange(-1.0, 1.0) * emitter->startColorVariance.GetB());
		startColor.SetValues(IntervalFit(a, 0, 255), IntervalFit(r, 0, 255), IntervalFit(g, 0, 255), IntervalFit(b, 0, 255));

		a = emitter->endColor.GetA() + (RandomRange(-1.0, 1.0) * emitter->endColorVariance.GetA());
		r = emitter->endColor.GetR() + (RandomRange(-1.0, 1.0) * emitter->endColorVariance.GetR());
		g = emitter->endColor.GetG() + (RandomRange(-1.0, 1.0) * emitter->endColorVariance.GetG());
		b = emitter->endColor.GetB() + (RandomRange(-1.0, 1.0) * emitter->endColorVariance.GetB());
		endColor.SetValues(IntervalFit(a, 0, 255), IntervalFit(r, 0, 255), IntervalFit(g, 0, 255), IntervalFit(b, 0, 255));

		SetUInt32Value(PARTICLE_VAR_DURATION, emitter->GetParticleDuration());
		SetUInt32Value(PARTICLE_VAR_START_TIME, GetGameTime());
		SetUInt32Value(PARTICLE_VAR_LAST_UPDATE_TIME, GetGameTime());

		this->emitter = emitter;
		return true;
	}
	return false;
}

void Particle::Update()
{
	if ((GetState() == true) && (animation != NULL) && (animation->GetState() == true))
	{
		uint32 updateTime = GetGameTime();
		Float updateDifference = (Float)(updateTime - GetUInt32Value(PARTICLE_VAR_LAST_UPDATE_TIME))/1000.0;
		Float durationPercentage = (Float)(GetGameTime() - GetUInt32Value(PARTICLE_VAR_START_TIME))/(Float)GetUInt32Value(PARTICLE_VAR_DURATION);

		position += (direction * updateDifference);
		direction += (emitter->force * updateDifference);

		if (emitter->GetUInt32Value(EMITTER_VAR_GRAVITY_FLAG) == true)
		{
			Float r = CalculateDistance(position, emitter->gravity);
			Float magnitude = emitter->GetFloatValue(EMITTER_VAR_GRAVITY_MAGNITUDE);

			Vector3D v(position, emitter->gravity);
			direction += (v * magnitude/(r * r));
		}

		SetUInt32Value(PARTICLE_VAR_LAST_UPDATE_TIME, updateTime);

		ColorARGB color((startColor.GetA() * (1.0 - durationPercentage)) + (durationPercentage * endColor.GetA()),
						(startColor.GetR() * (1.0 - durationPercentage)) + (durationPercentage * endColor.GetR()),
						(startColor.GetG() * (1.0 - durationPercentage)) + (durationPercentage * endColor.GetG()),
						(startColor.GetB() * (1.0 - durationPercentage)) + (durationPercentage * endColor.GetB()));

		animation->SetColor(color);
	}
	else
	{
		Unload();
	}
}

void Particle::Unload()
{
	if (GetState() == true)
	{
		SetState(false);
		animation = NULL;
		ParticlePool.Append(this);
	}
}

void LoadParticlePool()
{
	if (ParticlePool.GetSize() == 0)
	{
		for (uint32 i = 0; i < PARTICLE_POOL_SIZE; i++)
		{
			ParticlePool.Append(new Particle);
		}
	}
}

Particle * GetParticleFromPool()
{
	if (ParticlePool.GetSize() > 0)
	{
		Particle * particle = ParticlePool[0];
		ParticlePool.Remove(0);

		return particle;
	}
	return NULL;
}

void UnloadParticlePool()
{
	while (ParticlePool.GetSize() > 0)
	{
		delete ParticlePool[0];
		ParticlePool.Remove(0);
	}
}