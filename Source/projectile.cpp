#include "type.h"
#include "math.h"
#include "utility.h"
#include "point.h"
#include "shapes.h"
#include "list.h"
#include "spellobject.h"
#include "animation.h"
#include "tower.h"
#include "creep.h"
#include "projectile.h"
#include "game.h"
#include "script.h"
#include "tinyxml.h"
#include "rand.h"
#include "render.h"

Projectile * ProjectileData = NULL;
uint32 projDataSize = 0;

List<Projectile *> ActiveProjectiles;
List<Projectile *> RemovedProjectiles;

Projectile::Projectile()
{
	SetState(true);
	SetType(OBJ_TYPE_PROJECTILE | OBJ_TYPE_SPELL_OBJ);
	source = NULL;
	position = NullPoint;
	target = NULL;
	
	AllocateInt32Values(PROJ_INT32_VARS);
	AllocateUInt32Values(PROJ_UINT32_VARS);
	AllocateFloatValues(PROJ_FLOAT_VARS);

	SetInt32Value(PROJ_VAR_CAN_MOVE, true);

	SetFloatValue(PROJ_VAR_DAMAGE_MULTIPLIER, 1);
	SetFloatValue(PROJ_VAR_SPEED_MULTIPLIER, 1);
	SetFloatValue(PROJ_VAR_SPLASH_MULTIPLIER, 1);
}

Projectile::Projectile(const Projectile& projectile)
{
	SetState(true);
	SetType(OBJ_TYPE_PROJECTILE | OBJ_TYPE_SPELL_OBJ);

	AllocateInt32Values(PROJ_INT32_VARS);
	AllocateUInt32Values(PROJ_UINT32_VARS);
	AllocateFloatValues(PROJ_FLOAT_VARS);

	Copy(projectile);
}

Projectile::~Projectile()
{
	SetState(false);

	target = NULL;
	targetPos = NullPoint;
	position = NullPoint;

	/*if (animation != NULL)
	{
		animation->Unload();
	}*/
}

Float Projectile::GetArmorIgnore(const Creep * creep) const
{
	return GetInt32Value(PROJ_VAR_ARMOR_IGNORE_FLAT) + (creep->GetArmor() * MaxFit(GetFloatValue(PROJ_VAR_ARMOR_IGNORE_PERCENT), 100)/100.0);
}

bool Projectile::AddAnimation(bool animationType)
{
	uint32 animationID = GetUInt32Value(PROJ_VAR_ANIMATION_ID + animationType);

	if ((animationID != NULL) && (animationID < GetAnimationDataSize()))
	{
		const Animation * AnimationData = GetAnimationData();
		Animation * newAnim = AnimationData[animationID].AddAnimation(this);
		if (newAnim != NULL)
		{
			if (animationType == PROJ_ANIM_NORMAL)
			{
				animation = newAnim;
			}
			else if (animationType == PROJ_ANIM_EXPLOSION)
			{
				explosion = newAnim;
			}

			return true;
		}
	}
	return false;
}

void Projectile::Move(uint32 time)
{
	ModUInt32Value(PROJ_VAR_LAST_MOVE_TIME, time);
	if ((GetState() == true) && ((bool)GetInt32Value(PROJ_VAR_CAN_MOVE) == true))
	{
		Rect screenBounds(-100, -100, screenWidth + 100, screenHeight + 100);

		if ((((target == NULL) || (target->GetState() == false)) && (targetPos == NullPoint)) || (screenBounds.ContainsPoint(GetPosition()) == false))
		{
			Explode();
		}
		else
		{
			uint32 duration = GetUInt32Value(PROJ_VAR_DURATION);
			if ((duration != 0) && ((GetGameTime() - GetUInt32Value(PROJ_VAR_START_TIME)) > duration))
			{
				Explode();
			}
			else
			{
				int32 offset = (32 - (int32)GetUInt32Value(PROJ_VAR_BASE_SIZE))/2;

				Point2D moveTarget = ((target == NULL) || (target->GetState() == false)) ? targetPos : target->GetPosition();
				moveTarget.SetValues(moveTarget.GetX() + offset, moveTarget.GetY() + offset);

				Float distance = CalculateDistance(Origin, moveTarget - position);

				if (distance > 0)
				{
					Float speed = GetSpeed();
					Point2D moveVector = (moveTarget - position)/distance * time/1000.0 * ZeroFit(speed);
					Point2D destination = position + moveVector;

					if (CheckFlag(GetInt32Value(PROJ_VAR_FLAGS), PFLAG_INSTANTANEOUS) == true)
					{
						destination = moveTarget;
					}

					if (CheckFlag(GetInt32Value(PROJ_VAR_FLAGS), PFLAG_COLLIDE_WITH_CREEPS) == true)
					{
						LineSegment travelPath(position, destination);
						List<Creep *> * ActiveCreeps = GetActiveCreeps();

						ActiveCreeps->ToStart();
						for (uint32 i = 0; i < ActiveCreeps->GetSize(); i++)
						{
							Creep * creep = ActiveCreeps->GetCurrent();
							ActiveCreeps->ToNext();

							Point2D creepPos = creep->GetPosition();
							Rect creepRect(creepPos.GetX(), creepPos.GetY(), creepPos.GetX() + 32, creepPos.GetY() + 32);

							if (Intersect(&travelPath, &creepRect) == true)
							{
								SetPosition(Point2D(creepPos.GetX() + (16 - GetUInt32Value(PROJ_VAR_BASE_SIZE)/2), creepPos.GetY() + (16 - GetUInt32Value(PROJ_VAR_BASE_SIZE)/2)));
								AddAnimation(PROJ_ANIM_EXPLOSION);
								DamageCreep(creep, true);
							}
						}
					}

					if (CalculateDistance(Origin, moveVector) >= distance)
					{
						SetPosition(moveTarget);
						Explode();
					}
					else
					{
						SetPosition(destination);
					}
				}
				else
				{
					SetPosition(moveTarget);
					Explode();
				}
			}
		}
	}
}

void Projectile::DamageCreep(Creep * creep, bool phase)
{
	if ((creep != NULL) && (creep->GetState() == true))
	{
		Point2D origin = GetPosition();
		origin.SetValues(origin.GetX() + (GetUInt32Value(PROJ_VAR_BASE_SIZE)/2.0), origin.GetY() + (GetUInt32Value(PROJ_VAR_BASE_SIZE)/2.0));

		Point2D creepPos = creep->GetPosition();
		Point2D creepCenter(creepPos.GetX() + 16, creepPos.GetY() + 16);

		Float distance = CalculateDistance(origin, creepCenter);
		Float damage = (GetSplash() == 0) ? GetDamage() : ((1.0 - (distance/GetSplash())) * GetDamage());

		Creep * originalAttackTarget = GetTarget();

		SetTarget(creep);
		ActivateTriggeredScript(PROJ_TRIGGER_ON_HIT);
		SetTarget(originalAttackTarget);

		if ((creep->GetInt32Value(CREEP_VAR_PROJECTILE_IMMUNE) == false) && ((creep->GetInt32Value(CREEP_VAR_SPLASH_IMMUNE) == false) || (creep == target)))
		{
			creep->InflictDamage(ZeroFit(damage), GetArmorIgnore(creep));
		}

		if (creep->GetState() == false)
		{
			AddTowerKill(this);
		}

		Float phaseChance = GetFloatValue(PROJ_VAR_PHASE_PERCENT)/100.0f;
		Float result = RandomFloat();
		if ((result >= phaseChance) && (phase == true))
		{
			Unload();
		}
	}
}

void Projectile::Explode()
{
	if (GetState() == true)
	{
		List<Creep *> * ActiveCreeps = GetActiveCreeps();

		Point2D origin = GetPosition();
		origin.SetValues(origin.GetX() + (GetUInt32Value(PROJ_VAR_BASE_SIZE)/2.0), origin.GetY() + (GetUInt32Value(PROJ_VAR_BASE_SIZE)/2.0));
		Circle splashArea(origin, GetSplash());

		ActivateTriggeredScript(PROJ_TRIGGER_ON_EXPLODE);
		AddAnimation(PROJ_ANIM_EXPLOSION);

		//Point2D explosionPos = ((target == NULL) || (target->GetState() == false)) ? GetPosition() : target->GetPosition();
		ActiveCreeps->ToStart();
		for (uint8 i = 0; i < ActiveCreeps->GetSize(); i++)
		{
			Creep * creep = ActiveCreeps->GetCurrent();
			ActiveCreeps->ToNext();

			if ((creep->GetState() == true) && (GetState() == true))
			{
				Point2D creepPos = creep->GetPosition();
				Rect creepRect(creepPos.GetX(), creepPos.GetY(), creepPos.GetX() + 32, creepPos.GetY() + 32);
				if (Intersect(&creepRect, &splashArea) == true)
				{
					DamageCreep(creep, false);
				}
			}
		}
		Unload();
	}
}

void Projectile::Clear()
{
	Projectile * base = &ProjectileData[GetUInt32Value(PROJ_VAR_BASE_TYPE)];

	int32ValuesCount = base->int32ValuesCount;
	floatValuesCount = base->floatValuesCount;
	memcpy(int32Values, base->int32Values, (base->int32ValuesCount * sizeof(int32)));
	memcpy(floatValues, base->floatValues, (base->floatValuesCount * sizeof(Float)));

	triggeredScripts.ToStart();
	for (uint32 i = 0; i < triggeredScripts.GetSize(); i++)
	{
		TriggeredScript * triggeredScript = triggeredScripts.GetCurrent();
		triggeredScripts.ToNext();

		triggeredScript->SetState(false);
	}
}

void Projectile::Copy(const Projectile& obj)
{
	SpellObject::Copy(obj);
	target = obj.target;
	source = obj.source;
}

void Projectile::Unload()
{
	SetState(false);
}

bool LoadProjectileData()
{
	if (ProjectileData == NULL)
	{
		FILE * file = fopen("Assets\\XML\\projectile.xml", "r");

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

		TiXmlDocument ProjXML;
		ProjXML.Parse(fileData);

		TiXmlNode * node = ProjXML.FirstChild("ProjectileData");
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
					projDataSize = i;
					ProjectileData = new Projectile[projDataSize];
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

		for (uint32 i = 0; i < projDataSize; i++)
		{
			int32 ID = 0;
			element->Attribute("ID", &ID);

			ProjectileData[i].SetName(element->Attribute("Name"));
			ProjectileData[i].SetUInt32Value(PROJ_VAR_BASE_TYPE, ID);

			for (uint8 j = PROJ_VAR_BASE_SPEED; j <= PROJ_VAR_EXPLOSION_ID; j++)
			{
				if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
				{
					const char * text = secondaryElement->GetText();
					Float word = LookupWord(text, LOOKUP_VALUE);
					if (word == NOT_FOUND)
					{
						word = atof(text);
					}

					ProjectileData[i].SetUInt32Value(j, word);	
				}
				secondaryElement = secondaryElement->NextSiblingElement();
			}

			if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
			{
				const char * text = secondaryElement->GetText();
				Float word = LookupWord(text, LOOKUP_VALUE);
				if (word == NOT_FOUND)
				{
					word = atof(text);
				}

				ProjectileData[i].SetInt32Value(PROJ_VAR_FLAGS, word);	
			}
			secondaryElement = secondaryElement->NextSiblingElement();

			uint32 animationID = ProjectileData[i].GetUInt32Value(PROJ_VAR_ANIMATION_ID);
			if (animationID != NULL)
			{
				const Animation * AnimationData = GetAnimationData();
				Rect bounds = AnimationData[animationID].GetBounds();

				Float size = (bounds.GetWidth() > bounds.GetHeight()) ? bounds.GetWidth() : bounds.GetHeight();

				ProjectileData[i].SetUInt32Value(PROJ_VAR_BASE_SIZE, size);
			}

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

		ProjXML.Clear();
		Delete(fileData);

		return true;
	}
	return false;
}

const Projectile * GetProjectileData()
{
	return ProjectileData;
}

uint32 GetProjectileDataSize()
{
	return projDataSize;
}

List<Projectile *> * GetActiveProjectiles()
{
	return &ActiveProjectiles;
}

void ClearActiveProjectiles()
{
	while (ActiveProjectiles.GetSize() > 0)
	{
		if (ActiveProjectiles[0] != NULL)
		{
			delete ActiveProjectiles[0];
		}
		ActiveProjectiles.Remove(0);
	}
	while (RemovedProjectiles.GetSize() > 0)
	{
		if (RemovedProjectiles[0] != NULL)
		{
			delete RemovedProjectiles[0];
		}
		RemovedProjectiles.Remove(0);
	}
}

//void PurgeInactiveProjectiles()
//{
//	
//}
//
//void MoveProjectiles()
//{
//	for (uint32 i = 0; i < ActiveProjectiles.size(); i++)
//	{
//		if ((ActiveProjectiles[i] != NULL) && (ActiveProjectiles[i]->GetState() == true))
//		{
//			uint32 time = GetGameTime();
//			uint32 x = ActiveProjectiles[i]->GetUInt32Value(PROJ_VAR_LAST_MOVE_TIME);
//
//			ActiveProjectiles[i]->Move(time - x);
//		}
//	}
//}

void ProjectileMaintenance()
{
	while (RemovedProjectiles.GetSize() > 0)
	{
		if (RemovedProjectiles[0] != NULL)
		{
			delete RemovedProjectiles[0];
		}
		RemovedProjectiles.Remove(0);
	}

	ActiveProjectiles.ToStart();
	for (uint32 i = 0; i < ActiveProjectiles.GetSize(); i++)
	{
		Projectile * projectile = ActiveProjectiles.GetCurrent();
		ActiveProjectiles.ToNext();

		if ((projectile != NULL) && (projectile->GetState() == false))
		{
			RemovedProjectiles.Append(projectile);
			ActiveProjectiles.Remove(i);
			i--;
		}
		else if ((projectile != NULL) && (projectile->GetState() == true))
		{
			Tower * source = projectile->GetSource();
			if ((source != NULL) && (source->GetState() == false))
			{
				projectile->SetSource(NULL);
			}

			Creep * target = projectile->GetTarget();
			if ((target != NULL) && (target->GetState() == false))
			{
				projectile->SetTarget(target->GetPosition());
			}

			projectile->SpellMaintenance();
			projectile->Move(GetGameTime() - projectile->GetUInt32Value(PROJ_VAR_LAST_MOVE_TIME));
		}
	}
}

bool AddProjectile(Tower * tower)
{
	if ((tower != NULL) && (tower->GetState() == true))
	{
		Creep * target = tower->GetTarget();

		if (target != NULL)
		{
			Projectile * projectile = tower->GetProjectile();
			ActiveProjectiles.Append(new Projectile);

			Projectile * newProjectile = ActiveProjectiles.GetLast();
			newProjectile->Copy(*projectile);
			newProjectile->SetUInt32Value(PROJ_VAR_BASE_DAMAGE, tower->GetDamage());

			newProjectile->SetSource(tower);
			newProjectile->SetTarget(target);
			if (CheckFlag(newProjectile->GetInt32Value(PROJ_VAR_FLAGS), PFLAG_LINEAR) == true)
			{
				Point2D position = newProjectile->GetPosition();
				Point2D targetPos = target->GetPosition();
				//newProjectile->SetTarget(target->GetPosition());

				Float offset = 16 - (newProjectile->GetUInt32Value(PROJ_VAR_BASE_SIZE)/2);

				Float dy = (targetPos.GetY() - position.GetY() + offset);
				Float dx = (targetPos.GetX() - position.GetX() + offset);

				Rect screenBounds(-100, -100,  screenWidth + 100, screenHeight + 100);
				while (screenBounds.ContainsPoint(targetPos) == true)
				{
					targetPos.SetValues(targetPos.GetX() + dx, targetPos.GetY() + dy);
				}

				newProjectile->SetTarget(targetPos);
			}

			newProjectile->AddAnimation(PROJ_ANIM_NORMAL);

			newProjectile->SetUInt32Value(PROJ_VAR_START_TIME, GetGameTime());
			newProjectile->SetUInt32Value(PROJ_VAR_LAST_MOVE_TIME, GetGameTime());
			return true;
		}
	}
	return false;
}

void UnloadProjectileData()
{
	delete [] ProjectileData;
	projDataSize = 0;
}
