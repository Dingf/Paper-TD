#include "opengl.h"
#include "type.h"
#include "rand.h"
#include "math.h"
#include "point.h"
#include "list.h"
#include "script.h"
#include "spellobject.h"
#include "creep.h"
#include "projectile.h"
#include "tower.h"
#include "grid.h"
#include "game.h"

List<Creep *> ActiveCreeps;
List<Creep *> RemovedCreeps;

Creep::Creep()
{
	SetState(true);
	SetType(OBJ_TYPE_CREEP | OBJ_TYPE_SPELL_OBJ);
	AllocateInt32Values(CREEP_INT32_VARS);
	AllocateUInt32Values(CREEP_UINT32_VARS);
	AllocateFloatValues(CREEP_FLOAT_VARS);

	Clear();
	SetUInt32Value(CREEP_VAR_BASE_DAMAGE, 1000);
	SetInt32Value(CREEP_VAR_CAN_MOVE, true);
}

Creep::~Creep()
{
	SetState(false);
}

bool Creep::InflictDamage(Float damage, Float armorIgnore)
{
	if (GetState() == true)
	{
		damage *= WorldGame.GetFloatValue(GAME_VAR_LC_MULTIPLIER_DAMAGE);

		Float armor = GetArmor() - armorIgnore;
		damage = (armor > damage) ? 0 : damage - armor;

		damage *= GetDamageMultiplier();

		ModFloatValue(CREEP_VAR_HITPOINTS, -damage);

		if (GetFloatValue(CREEP_VAR_HITPOINTS) <= 0)
		{
			WorldGame.AddInk(GetBounty() * WorldGame.GetFloatValue(GAME_VAR_LC_MULTIPLIER_BOUNTY));
			Unload();
			ActivateTriggeredScript(CREEP_TRIGGER_ON_DEATH);
			return true;
		}
	}
	return false;
}

void Creep::CalculateNextMove(const Point2D& location)
{
	if (GetState() == true)
	{
		lastPos = position;
		moveTarget = ((position.GetY() >= 688) || (GetInt32Value(CREEP_VAR_FLYING) == true)) ? Point2D(384, 768) : location;

		if (position.GetY() >= 768)
		{
			WorldGame.AddInk(-(GetBounty() * WorldGame.GetFloatValue(GAME_VAR_LC_MULTIPLIER_BOUNTY)));
			Unload();
			return;
		}

		/*Float dx = moveTarget.GetX() - lastPos.GetX();
		Float dy = moveTarget.GetY() - lastPos.GetY();

		Float distance = (dx == 0) ? AbsVal(dy) : ((dy == 0) ? AbsVal(dx) : Root2((dx * dx) + (dy * dy)));

		Float time = glutGet(GLUT_ELAPSED_TIME);*/

		SetUInt32Value(CREEP_VAR_MOVE_TIMER, GetGameTime());
	}
}

void Creep::Move(uint32 time)
{
	if (GetState() == true)
	{
		Grid * grid = WorldGame.GetGrid();

		Float speed = ZeroFit(GetSpeed());
		Float moveDistance = speed * (time/1000.0f);
		Float targetDistance = CalculateDistance(position, moveTarget);

		if (moveDistance >= targetDistance)
		{
			SetPosition(moveTarget);
			Point2D position = GetPosition();

			uint32 x = (position.GetX() - 80)/32;
			uint32 y = (position.GetY() - 80)/32;

			int8 pathValue = grid->GetPath(Point2D(x, y));
			if (pathValue == 0)
			{
				grid->FindPath(Point2D(x, y), Origin);
			}
				
			switch (pathValue)
			{
				case 2:
				{
					position.SetValues((x * 32) + 80, (y * 32) + 48);
					break;
				}
				case 3:
				{
					position.SetValues((x * 32) + 112, (y * 32) + 80);
					break;
				}
				case 4:
				{
					position.SetValues((x * 32) + 80, (y * 32) + 112);
					break;
				}
				case 5:
				{
					position.SetValues((x * 32) + 48, (y * 32) + 80);
					break;
				}
			}

			CalculateNextMove(position);
			if ((speed != 0) && (GetInt32Value(CREEP_VAR_CAN_MOVE) == true))
			{
				Float remainingTime = (moveDistance - targetDistance)/speed * 1000.0f;
				Move(time - remainingTime);
			}
		}
		else if (moveDistance > 0)
		{	
			Point2D position = GetPosition();

			SetUInt32Value(CREEP_VAR_MOVE_TIMER, GetGameTime());
			moveDistance = CalculateDistance(lastPos, moveTarget);

			if (GetInt32Value(CREEP_VAR_CAN_MOVE) == true)
			{
				Float dx = (moveTarget.GetX() - lastPos.GetX())/moveDistance * speed * time/1000;
				Float dy = (moveTarget.GetY() - lastPos.GetY())/moveDistance * speed * time/1000;

				SetPosition(Point2D(position.GetX() + dx, position.GetY() + dy));
			}
		}
	}
}

void Creep::Clear()
{
	//ClearGameObjects();
	//ClearTriggeredScripts();
	for (uint8 i = CREEP_VAR_ARMOR_FLAT; i <= CREEP_VAR_BOUNTY_FLAT; i++)
	{
		SetInt32Value(i, 0);
	}
	for (uint8 i = CREEP_VAR_ARMOR_MULTIPLIER; i <= CREEP_VAR_DAMAGE_MULTIPLIER; i++)
	{
		SetFloatValue(i, 1.0);
	}
	SetInt32Value(CREEP_VAR_CAN_MOVE, true);
}

void Creep::Unload()
{
	SetState(false);
}

List<Creep *> * GetActiveCreeps()
{
	return &ActiveCreeps;
}

void ClearActiveCreeps()
{
	while (ActiveCreeps.GetSize() > 0)
	{
		if (ActiveCreeps[0] != NULL)
		{
			delete ActiveCreeps[0];
		}
		ActiveCreeps.Remove(0);
	}
	while (RemovedCreeps.GetSize() > 0)
	{
		if (RemovedCreeps[0] != NULL)
		{
			delete RemovedCreeps[0];
		}
		RemovedCreeps.Remove(0);
	}
}

void CreepMaintenance()
{
	while (RemovedCreeps.GetSize() > 0)
	{
		if (RemovedCreeps[0] != NULL)
		{
			delete RemovedCreeps[0];
		}
		RemovedCreeps.Remove(0);
	}

	ActiveCreeps.ToStart();
	for (uint32 i = 0; i < ActiveCreeps.GetSize(); i++)
	{
		Creep * creep = ActiveCreeps.GetCurrent();
		ActiveCreeps.ToNext();

		if ((creep != NULL) && (creep->GetState() == false) || (creep->GetHitpoints() <= 0))
		{
			RemovedCreeps.Append(creep);
			ActiveCreeps.Remove(i);
			i--;
		}
		else if ((creep != NULL) && (creep->GetState() == true))
		{
			if (creep->GetFloatValue(CREEP_VAR_MAX_HITPOINTS) < creep->GetHitpoints())
			{
				creep->SetFloatValue(CREEP_VAR_HITPOINTS, creep->GetFloatValue(CREEP_VAR_MAX_HITPOINTS));
			}

			creep->SpellMaintenance();
			uint32 timeDif = GetGameTime() - creep->GetUInt32Value(CREEP_VAR_MOVE_TIMER);
			creep->Move(timeDif);
		}
	}
}

void AddCreep()
{
	ActiveCreeps.Append(new Creep);

	Creep * creep = ActiveCreeps.GetLast();
	creep->SetState(true);
	creep->SetUInt32Value(CREEP_VAR_TYPE, (RandomUInt32() % 9) + 1);
}