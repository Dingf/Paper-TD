#include <stdio.h>
#include "tinyxml.h"
#include "type.h"
#include "math.h"
#include "rand.h"
#include "utility.h"
#include "object.h"
#include "list.h"
#include "script.h"
#include "animation.h"
#include "render.h"
#include "particle.h"
#include "emitter.h"
#include "gameobject.h"
#include "game.h"

Emitter * EmitterData;
uint32 emitterDataSize;

List<Emitter *> ActiveEmitters;

Emitter::Emitter()
{
	SetState(true);
	SetType(OBJ_TYPE_EMITTER);
	AllocateUInt32Values(EMITTER_UINT32_VARS);
	AllocateFloatValues(EMITTER_FLOAT_VARS);

	surface = NULL;
	animation = GetAnimation(0);
}

Emitter::~Emitter()
{
	SetState(false);
}

uint32 Emitter::GetEmitsPerSecond() const
{
	int32 offset = (int32)GetUInt32Value(EMITTER_VAR_EPS_VARIANCE) * RandomRange(-1.0, 1.0);
	return ZeroFit((int32)GetUInt32Value(EMITTER_VAR_EMITS_PER_SEC) + offset);
}

uint32 Emitter::GetParticlesPerEmit() const
{
	int32 offset = (int32)GetUInt32Value(EMITTER_VAR_PPE_VARIANCE) * RandomRange(-1.0, 1.0);
	return ZeroFit((int32)GetUInt32Value(EMITTER_VAR_PARTICLES_PER_EMIT) + offset);
}

uint32 Emitter::GetParticleDuration() const
{
	int32 offset = (int32)GetUInt32Value(EMITTER_VAR_PARTICLE_DURATION_VARIANCE) * RandomRange(-1.0, 1.0);
	return ZeroFit((int32)GetUInt32Value(EMITTER_VAR_PARTICLE_DURATION) + offset);
}

Float Emitter::GetSpeedMultiplier() const
{
	Float offset = GetFloatValue(EMITTER_VAR_SPEED_VARIANCE) * RandomRange(-1.0, 1.0);
	return ZeroFit(GetFloatValue(EMITTER_VAR_SPEED) + offset);
}

Emitter * Emitter::AddEmitter(Object * source, const Point2D& position) const
{
	if (GetState() == true)
	{
		ActiveEmitters.Append(new Emitter);

		Emitter * newEmitter = ActiveEmitters.GetLast();
		newEmitter->Copy(*this);
		newEmitter->animation = animation;
		newEmitter->position = position;

		const GameObject * GameObjectData = GetGameObjectData();
		const GameObject * BaseGameObject = &GameObjectData[GetUInt32Value(EMITTER_VAR_SURFACE_ID)];

		if (BaseGameObject->AddGameObject(source, position) != NULL)
		{
			List<GameObject *> * ActiveGameObjects = GetActiveGameObjects();

			GameObject * gameObj = ActiveGameObjects->GetData(ActiveGameObjects->GetSize() - 1);

			const Shape * shape = gameObj->GetShape();
			newEmitter->surface = shape->CreateInstance();

			gameObj->SetState(false);
		}
		else
		{
			return NULL;
		}

		newEmitter->startColor = startColor;
		newEmitter->startColorVariance = startColorVariance;
		newEmitter->endColor = endColor;
		newEmitter->endColorVariance = endColorVariance;

		newEmitter->direction = direction;
		newEmitter->directionVariance = directionVariance;
		newEmitter->force = force;
		newEmitter->gravity = gravity;

		newEmitter->SetUInt32Value(EMITTER_VAR_START_TIME, GetGameTime());
		newEmitter->SetUInt32Value(EMITTER_VAR_LAST_EMIT_TIME, GetGameTime());

		return newEmitter;
	}
	return NULL;
}

bool Emitter::AddParticle()
{
	if ((GetState() == true) && (particles.GetSize() < GetUInt32Value(EMITTER_VAR_LIMIT)))
	{
		Particle * particle = GetParticleFromPool();
		if (particle != NULL)
		{
			particles.Append(particle);
			particle = particles.GetLast();

			return particle->SetValues(this);
		}
	}
	return false;
}

void Emitter::Update()
{
	if (GetState() == true)
	{
		uint32 updateTime = GetGameTime();
		uint32 timeDif = updateTime - GetUInt32Value(EMITTER_VAR_LAST_EMIT_TIME);

		if ((updateTime - GetUInt32Value(EMITTER_VAR_START_TIME)) <= GetUInt32Value(EMITTER_VAR_DURATION))
		{
			uint32 eps = GetEmitsPerSecond();
			uint32 emitAmount = (eps * timeDif)/1000 * GetParticlesPerEmit();
			for (uint32 i = 0; i < emitAmount; i++)
			{
				AddParticle();
			}

			if (emitAmount > 0)
			{
				SetUInt32Value(EMITTER_VAR_LAST_EMIT_TIME, updateTime - (timeDif % (1000/eps)));
			}
		}

		if (particles.GetSize() > 0)
		{
			particles.ToStart();
			for (uint32 i = 0; i < particles.GetSize(); i++)
			{
				Particle * particle = particles.GetCurrent();
				particles.ToNext();

				if (particle->GetState() == false)
				{
					particles.Remove(i);
					i--;
				}
				else if ((GetGameTime() - particle->GetUInt32Value(PARTICLE_VAR_START_TIME)) > particle->GetUInt32Value(PARTICLE_VAR_DURATION))
				{
					particle->Unload();
				}
				else
				{
					particle->Update();
				}
			}
		}
		else if ((updateTime - GetUInt32Value(EMITTER_VAR_START_TIME)) > GetUInt32Value(EMITTER_VAR_DURATION))
		{
			Unload();
		}
	}
}

void Emitter::Unload()
{
	if (surface != NULL)
	{
		delete surface;
	}
	while (particles.GetSize() > 0)
	{
		particles[0]->Unload();
		particles.Remove(0);
	}

	ActiveEmitters.ToStart();
	for (uint32 i = 0; i < ActiveEmitters.GetSize(); i++)
	{
		Emitter * emitter = ActiveEmitters.GetCurrent();
		ActiveEmitters.ToNext();

		if ((emitter == this) && (emitter != NULL))
		{
			delete emitter;
			ActiveEmitters.Remove(i);
		}
	}
}

Vector3D CalculateDirection(Float yaw, Float pitch)
{
	return Vector3D(-SinD(yaw) * CosD(pitch),
					 SinD(pitch),
					 CosD(pitch) * CosD(yaw));
}

bool LoadEmitterData()
{
	LoadParticlePool();
	if (EmitterData == NULL)
	{
		FILE * file = fopen("Assets\\XML\\particle.xml", "r");

		if (file == NULL)
		{
			return false;
		}

		fseek(file, 0, SEEK_END);
		uint32 fileSize = ftell(file);
		fseek(file, 0, SEEK_SET);

		char * fileData = new char[fileSize];

		fread(fileData, fileSize, 1, file);

		fclose(file);

		TiXmlDocument EmitterXML;
		EmitterXML.Parse(fileData);

		TiXmlNode * node = EmitterXML.FirstChild("ParticleData");
		TiXmlElement * element = node->ToElement();

		node = element->FirstChild();
		element = node->ToElement();

		TiXmlElement * lastElement = element;
		for (uint32 i = 0;; i++)
		{
			if (lastElement == NULL)
			{
				if (i > 0)
				{
					emitterDataSize = i;
					EmitterData = new Emitter[emitterDataSize];
					break;
				}
				else
				{
					return false;
				}
			}
			lastElement = lastElement->NextSiblingElement();
		}

		node = element->FirstChild();
		TiXmlElement * secondaryElement = node->ToElement();

		for (uint32 i = 0; i < emitterDataSize; i++)
		{
			int32 ID = 0;
			element->Attribute("ID", &ID);

			EmitterData[i].SetUInt32Value(EMITTER_VAR_ID, ID);
			EmitterData[i].SetName(element->Attribute("Name"));

			for (uint8 j = EMITTER_VAR_SURFACE_ID; j <= EMITTER_VAR_SPAWN_DIST_MAX; j++)
			{
				if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
				{
					const char * text = secondaryElement->GetText();
					Float word = LookupWord(text, LOOKUP_VALUE);
					if (word == NOT_FOUND)
					{
						word = atof(text);
					}

					EmitterData[i].SetUInt32Value(j, word);
				}
				secondaryElement = secondaryElement->NextSiblingElement();		
			}

			for (uint8 j = EMITTER_VAR_SPEED; j <= EMITTER_VAR_SPEED_VARIANCE; j++)
			{
				if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
				{
					const char * text = secondaryElement->GetText();
					Float word = LookupWord(text, LOOKUP_VALUE);
					if (word == NOT_FOUND)
					{
						word = atof(text);
					}

					EmitterData[i].SetFloatValue(j, word);
				}
				secondaryElement = secondaryElement->NextSiblingElement();		
			}

			if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
			{
				const char * text = secondaryElement->GetText();
				int32 animID = ZeroFit(atoi(text));

				EmitterData[i].animation = GetAnimation(animID);
			}
			secondaryElement = secondaryElement->NextSiblingElement();

			ColorARGB * colors[4] = { &EmitterData[i].startColor, &EmitterData[i].startColorVariance, &EmitterData[i].endColor, &EmitterData[i].endColorVariance };
			for (uint8 j = 0; j < 4; j++)
			{
				if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
				{
					const char * text = secondaryElement->GetText();
					uint32 values[4] = { 0, 0, 0, 0 };

					sscanf(text, "%d %d %d %d", &values[0], &values[1], &values[2], &values[3]);

					colors[j]->SetValues(MaxFit(values[0], 255), MaxFit(values[1], 255), MaxFit(values[2], 255), MaxFit(values[3], 255));
				}
				secondaryElement = secondaryElement->NextSiblingElement();
			}

			Float values[4] = { 0.0, 0.0, 0.0, 0.0 };
			for (uint8 j = 0; j < 4; j++)
			{
				if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
				{
					const char * text = secondaryElement->GetText();
					values[j] = atof(text);
				}
				secondaryElement = secondaryElement->NextSiblingElement();
			}
			EmitterData[i].direction = CalculateDirection(values[0], values[2]);
			EmitterData[i].directionVariance = CalculateDirection(values[1], values[3]);

			for (uint8 j = 0; j < 3; j++)
			{
				if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
				{
					const char * text = secondaryElement->GetText();
					values[j] = atof(text);
				}
				secondaryElement = secondaryElement->NextSiblingElement();
			}
			EmitterData[i].force = CalculateDirection(values[0], values[1]) * values[2];

			if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
			{
				const char * text = secondaryElement->GetText();
				uint32 coords[2] = { 0, 0 };
				if (sscanf(text, "%d %d", &coords[0], &coords[1]) == 2)
				{
					EmitterData[i].SetUInt32Value(EMITTER_VAR_GRAVITY_FLAG, true);
					EmitterData[i].gravity.SetValues(coords[0], coords[1]);
				}
			}
			secondaryElement = secondaryElement->NextSiblingElement();

			if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
			{
				const char * text = secondaryElement->GetText();
				Float word = LookupWord(text, LOOKUP_VALUE);
				if (word == NOT_FOUND)
				{
					word = atof(text);
				}

				EmitterData[i].SetFloatValue(EMITTER_VAR_GRAVITY_MAGNITUDE, word);
			}
			secondaryElement = secondaryElement->NextSiblingElement();
			
			element = element->NextSiblingElement();
			if (element != NULL)
			{
				node = element->FirstChild();
				secondaryElement = node->ToElement();
			}
			else
			{
				break;
			}
		}

		EmitterXML.Clear();
		Delete(fileData);

		return true;
	}
	return false;
}

const Emitter * GetEmitterData()
{
	return EmitterData;
}

uint32 GetEmitterDataSize()
{
	return emitterDataSize;
}

List<Emitter *> * GetActiveEmitters()
{
	return &ActiveEmitters;
}

void EmitterMaintenance()
{
	for (uint32 i = 0; i < ActiveEmitters.GetSize(); i++)
	{
		ActiveEmitters[i]->Update();
	}
}

void ClearActiveEmitters()
{
	while (ActiveEmitters.GetSize() > 0)
	{
		ActiveEmitters[0]->Unload();
	}
}

void UnloadEmitterData()
{
	delete [] EmitterData;
	emitterDataSize = 0;
	UnloadParticlePool();
}