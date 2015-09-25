#include "type.h"
#include "shapes.h"
#include "mouse.h"
#include "script.h"
#include "scriptdef.h"
#include "spell.h"
#include "creep.h"
#include "wave.h"
#include "projectile.h"
#include "tower.h"
#include "selection.h"
#include "grid.h"
#include "game.h"

Float GetMoveTarget(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * targetWord = script->GetNextWord();
		Word * xyFlags = script->PeekWord(1);

		Object * target = script->GetTarget(targetWord);
		if ((target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_CREEP) == true))
		{
			Creep * creep = (Creep *)target;
			Point2D * moveTarget = (Point2D *)&creep->GetMoveTarget();
			if ((xyFlags != NULL) && (xyFlags->value == GET_KEYWORD_X_VAL))
			{
				script->GetNextOriginalWord();
				return moveTarget->GetX();
			}
			else if ((xyFlags != NULL) && (xyFlags->value == GET_KEYWORD_Y_VAL))
			{
				script->GetNextOriginalWord();
				return moveTarget->GetY();
			}
			else
			{
				return (uint32)moveTarget;
			}
		}
	}
	return false;
}

Float GetLastPosition(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * targetWord = script->GetNextWord();
		Word * xyFlags = script->PeekWord(1);

		Object * target = script->GetTarget(targetWord);
		if ((target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_CREEP) == true))
		{
			Creep * creep = (Creep *)target;
			Point2D * lastPos = (Point2D *)&creep->GetLastPos();
			if ((xyFlags != NULL) && (xyFlags->value == GET_KEYWORD_X_VAL))
			{
				script->GetNextOriginalWord();
				return lastPos->GetX();
			}
			else if ((xyFlags != NULL) && (xyFlags->value == GET_KEYWORD_Y_VAL))
			{
				script->GetNextOriginalWord();
				return lastPos->GetY();
			}
			else
			{
				return (uint32)lastPos;
			}
		}
	}
	return false;
}

Float GetTargetPosition(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * targetWord = script->GetNextWord();
		Word * xyFlags = script->PeekWord(1);

		Object * target = script->GetTarget(targetWord);
		if ((target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_PROJECTILE) == true))
		{
			Projectile * projectile = (Projectile *)target;
			Creep * target = projectile->GetTarget();

			Point2D * targetPos = (target == NULL) ? &projectile->GetTargetPos() : (Point2D *)&target->GetPosition();
			if ((xyFlags != NULL) && (xyFlags->value == GET_KEYWORD_X_VAL))
			{
				script->GetNextOriginalWord();
				return targetPos->GetX();
			}
			else if ((xyFlags != NULL) && (xyFlags->value == GET_KEYWORD_Y_VAL))
			{
				script->GetNextOriginalWord();
				return targetPos->GetY();
			}
			else
			{
				return (uint32)targetPos;
			}
		}
	}
	return false;
}

Float GetMousePosition(Script * script)
{
	Word * xyFlags = script->PeekWord(1);
	const Point2D * mousePos = (Point2D *)&WorldMouse.GetPosition();

	if ((xyFlags != NULL) && (xyFlags->value == GET_KEYWORD_X_VAL))
	{
		script->GetNextOriginalWord();
		return mousePos->GetX();
	}
	else if ((xyFlags != NULL) && (xyFlags->value == GET_KEYWORD_Y_VAL))
	{
		script->GetNextOriginalWord();
		return mousePos->GetY();
	}
	else
	{
		return (uint32)mousePos;
	}
}

const Shape * IntersectHelper(const Object * object)
{
	Point2D position = object->GetPosition();

	if ((object->CheckType(OBJ_TYPE_TOWER) == true) || (object->CheckType(OBJ_TYPE_CREEP) == true))
	{
		return new Rect(position.GetX(), position.GetY(), position.GetX() + 32, position.GetY() + 32);
	}
	else if (object->CheckType(OBJ_TYPE_PROJECTILE) == true) 
	{
		uint32 baseSize = object->GetUInt32Value(PROJ_VAR_BASE_SIZE);
		return new Rect(position.GetX(), position.GetY(), position.GetX() + baseSize, position.GetY() + baseSize);
	}
	else if (object->CheckType(OBJ_TYPE_GAME_OBJECT) == true)
	{
		Shape * shape = ((GameObject *)object)->GetShape();
		Shape * newShape = shape->CreateInstance();
		newShape->Translate(object->GetPosition());
		return newShape;
	}
	return NULL;
}

Float GetPointIntersection(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * targetWord1 = script->GetNextWord();
		Word * targetWord2 = script->GetNextWord();

		Object * target = script->GetTarget(targetWord1);
		Point2D * point = (Point2D *)(uint32)targetWord2->value;

		if ((target != NULL) && (point != NULL))
		{
			Point2D targetPoint(point->GetX(), point->GetY());
			const Shape * shape = IntersectHelper(target);

			if (shape != NULL)
			{
				bool result = shape->ContainsPoint(targetPoint);
				delete shape;
				
				return result;
			}
		}
	}
	return false;
}

Float GetSelectionSize(Script * script)
{
	Selection * selection = WorldGame.GetSelection();
	return selection->GetSize();
}

Float GetWaveTime(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * waveWord = script->GetNextWord();

		List<Wave *> * ActiveWaves = GetActiveWaves();
		ActiveWaves->ToStart();
		for (uint32 i = 0; i < ActiveWaves->GetSize(); i++)
		{
			Wave * wave = ActiveWaves->GetCurrent();
			ActiveWaves->ToNext();

			if (wave->GetUInt32Value(WAVE_VAR_NUMBER) == (int32)waveWord->value)
			{
				return wave->GetUInt32Value(WAVE_VAR_ADD_TIME);
			}
		}
	}
	return false;
}

Float GetWaveType(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * waveWord = script->GetNextWord();

		List<Wave *> * ActiveWaves = GetActiveWaves();
		ActiveWaves->ToStart();
		for (uint32 i = 0; i < ActiveWaves->GetSize(); i++)
		{
			Wave * wave = ActiveWaves->GetCurrent();
			ActiveWaves->ToNext();

			if (wave->GetUInt32Value(WAVE_VAR_NUMBER) == (int32)waveWord->value)
			{
				return wave->GetWaveType();
			}
		}
	}
	return false;
}

Float GetObjectPosition(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * targetWord = script->GetNextWord();
		Object * target = script->GetTarget(targetWord);

		if ((target != NULL) && (target->GetState() == true))
		{
			Word * xyFlags = script->PeekWord(1);
			const Point2D * targePos = (Point2D *)&target->GetPosition();

			if ((xyFlags != NULL) && (xyFlags->value == GET_KEYWORD_X_VAL))
			{
				script->GetNextOriginalWord();
				return targePos->GetX();
			}
			else if ((xyFlags != NULL) && (xyFlags->value == GET_KEYWORD_Y_VAL))
			{
				script->GetNextOriginalWord();
				return targePos->GetY();
			}
			else
			{
				return (uint32)targePos;
			}
		}
	}
	return false;
}

Float GetDistance(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Point2D positions[2] = { NullPoint, NullPoint };

		for (uint32 i = 0; i < 2; i++)
		{
			Word * word = script->GetNextWord();
			if (word->type == NUMERIC)
			{
				Point2D * pos = (Point2D *)(uint32)word->value;
				if (pos != NULL)
				{
					positions[i].SetValues(pos->GetX(), pos->GetY());
				}
				else
				{
					return false;
				}
			}
			else if (word->type == POINTER)
			{
				Object * target = script->GetTarget(word);
				if (target != NULL)
				{
					positions[i] = target->GetPosition();
				}
			}
		}
		return CalculateDistance(positions[0], positions[1]);
	}
	return false;
}

Float GetShapeIntersection(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * targetWord1 = script->GetNextWord();
		Word * targetWord2 = script->GetNextWord();

		Object * targets[] = {script->GetTarget(targetWord1),
							  script->GetTarget(targetWord2)};

		if ((targets[0] != NULL) && (targets[1] != NULL))
		{
			const Shape * shapes[] = {IntersectHelper(targets[0]),
							          IntersectHelper(targets[1])};

			if ((shapes[0] != NULL) && (shapes[1] != NULL))
			{
				bool result = Intersect(shapes[0], shapes[1]);
				for (uint32 i = 0; i < 2; i++)
				{
					delete shapes[i];
				}
				
				return result;
			}
		}
	}
	return false;
}

Float GetObjectState(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * word = script->GetNextWord();

		Object * target = script->GetTarget(word);
		if (target != NULL)
		{
			return target->GetState();
		}
	}
	return false;
}

Float GetObjectType(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * word = script->GetNextWord();

		Object * target = script->GetTarget(word);
		if (target != NULL)
		{
			return target->GetType();
		}
	}
	return false;
}

Float GetGameTime(Script * script)
{
	return GetGameTime();
}

Float GetSelectedIndex(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * targetWord = script->GetNextWord();
		Tower * target = (Tower *)script->GetTarget(targetWord);

		if ((target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_TOWER) == true))
		{
			Selection * selection = WorldGame.GetSelection();
			return selection->GetIndex(target) + 1;		//0 = not selected, 1-12 are locations
		}
	}
	return false;
}

Float GetObjectText(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * targetWord = script->GetNextWord();
		Object * target = script->GetTarget(targetWord);

		if (target->CheckType(OBJ_TYPE_TOWER) == true)
		{
			Tower * tower = (Tower *)target;
			uint32 towerType = tower->GetTowerType();

			Word * subResult = script->GetLastSubResult();
			const TowerBase * TowerData = GetTowerData();
			subResult->word = StringCopy(TowerData[towerType].GetDescription());

			return true;
		}
		else if (target->CheckType(OBJ_TYPE_SPELL) == true)
		{
			Spell * spell = (Spell *)target;

			Word * subResult = script->GetLastSubResult();
			subResult->word = StringCopy(spell->GetText());

			return true;
		}
	}
	return false;
}

Float GetCastingState(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * targetWord = script->GetNextWord();
		Object * target = script->GetTarget(targetWord);

		if ((target != NULL) && (target->CheckType(OBJ_TYPE_SPELL_OBJ) == true))
		{
			SpellObject * spellObject = (SpellObject *)target;
			return (spellObject->GetCastTarget() != NULL);
		}
	}
	return false;
}

Float GetGridPath(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * posWord = script->GetNextWord();
		Point2D * position = (Point2D *)(uint32)posWord->value;
		Point2D coords(0, 0);
		if (position != NULL)
		{
			coords.SetValues(position->GetX(), position->GetY());
			coords.SetValues((coords.GetX() - 80)/32, (coords.GetY() - 80)/32);
		}
		else
		{
			return false;
		}

		if ((IntervalCheck(coords.GetX(), 1, GRID_SIZE - 2) == true) && (IntervalCheck(coords.GetY(), 1, GRID_SIZE - 2) == true))
		{
			Grid * grid = WorldGame.GetGrid();
			return grid->GetPath(coords);
		}
	}
	return false;
}

Float GetObjectDamage(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * targetWord = script->GetNextWord();
		Object * target = script->GetTarget(targetWord);

		if ((target != NULL) && (target->GetState() == true))
		{
			if (target->CheckType(OBJ_TYPE_TOWER) == true)
			{
				return ((Tower *)target)->GetDamage();
			}
			else if (target->CheckType(OBJ_TYPE_PROJECTILE) == true)
			{
				return ((Projectile *)target)->GetDamage();
			}
			else if (target->CheckType(OBJ_TYPE_GAME_OBJECT) == true)
			{
				return ((GameObject *)target)->GetUInt32Value(GOBJECT_VAR_SOURCE_DMG);
			}
		}
	}
	return false;
}

Float GetTowerRange(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * targetWord = script->GetNextWord();
		Tower * tower = (Tower *)script->GetTarget(targetWord);

		if ((tower != NULL) && (tower->GetState() == true) && (tower->CheckType(OBJ_TYPE_TOWER) == true))
		{
			return tower->GetRange();
		}
	}
	return false;
}

Float GetTowerAttackDelay(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * targetWord = script->GetNextWord();
		Tower * tower = (Tower *)script->GetTarget(targetWord);

		if ((tower != NULL) && (tower->GetState() == true) && (tower->CheckType(OBJ_TYPE_TOWER) == true))
		{
			return tower->GetAttackTime();
		}
	}
	return false;
}

Float GetCreepArmor(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * targetWord = script->GetNextWord();
		Creep * creep = (Creep *)script->GetTarget(targetWord);

		if ((creep != NULL) && (creep->GetState() == true) && (creep->CheckType(OBJ_TYPE_CREEP) == true))
		{
			return creep->GetArmor();
		}
	}
	return false;
}

Float GetNormalValues(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * word = script->GetNextWord();

		if (IntervalCheck(word->type, INT32_VAR, FLOAT_CONST) == true)
		{
			if (script->VerifyArguments(1) == true)
			{
				Word * targetWord = script->GetNextWord();

				if (targetWord->type == POINTER)
				{
					Object * target = script->GetTarget(targetWord);
					if (target != NULL)
					{
						uint8 type = ((word->type - 1) % 3) + 1;
						return target->GetValue(type, word->value);
					}
				}
			}
		}
		else if (word->type == POINTER)
		{
			if (script->VerifyArguments(1) == true)
			{
				Word * targetWord = script->GetNextWord();

				Object * originalSource = script->GetOriginalSource();
				Object * newSource = script->GetTarget(targetWord);

				if (newSource != NULL)
				{
					script->SetSource(newSource);
					Object * target = script->GetTarget(word);
					script->SetSource(originalSource);

					return (uint32)target;
				}
			}
			return false;
		}
	}
	return false;
}

/*#include "type.h"
#include "utility.h"
#include "point.h"
#include "object.h"
#include "shapes.h"
#include "mouse.h"
#include "script.h"
#include "scripthandler.h"
#include "gameobject.h"
#include "spellobject.h"
#include "creep.h"
#include "wave.h"
#include "projectile.h"
#include "tower.h"
#include "game.h"

Point2D coordHelper = NullPoint;

Float ActionCalculateDistance(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 3) == true)
	{
		Point2D positions[2] = { NullPoint, NullPoint };

		for (uint32 i = 0; i < 2; i++)
		{
			Word * word = script->GetWord(index + 2 + i);
			if (word->type == NUMERIC)
			{
				Point2D * pos = (Point2D *)(uint32)word->value;
				if (pos != NULL)
				{
					positions[i].SetValues(pos->GetX(), pos->GetY());
				}
			}
			else if (word->type == POINTER)
			{
				Object * target = script->GetTarget(word);
				if (target != NULL)
				{
					positions[i] = target->GetPosition();
				}
			}
		}
		return CalculateDistance(positions[0], positions[1]);
	}
	return false;
}

Float ActionGetState(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * word = script->GetWord(index + 2);

		if (word->type == POINTER)
		{
			Object * target = script->GetTarget(word);

			if (target != NULL)
			{
				return target->GetState();
			}
		}
	}
	return false;
}

Float ActionGetBaseType(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * word = script->GetWord(index + 2);

		if (word->type == POINTER)
		{
			Object * target = script->GetTarget(word);

			if (target != NULL)
			{
				return target->GetType();
			}
		}
	}
	return false;
}

Float ActionGetPosition(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * word = script->GetWord(index + 2);

		if (word->type == NORMAL)
		{
			switch ((int32)word->value)
			{
				case KEYWORD_MOUSE:
				{
					return (uint32)&WorldMouse.GetPosition();
					break;
				}
			}
		}
		else if (word->type == POINTER)
		{
			Object * target = script->GetTarget(word);
			if (target != NULL)
			{
				return (uint32)&target->GetPosition();
			}
		}
		else if (word->type == NUMERIC)
		{
			Object * target = (Object *)(uint32)word->value;
			if (target != NULL)
			{
				return (uint32)&target->GetPosition();
			}
		}
	}
	return false;
}

Float ActionGetPositionX(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * word = script->GetWord(index + 2);

		if (word->type == NORMAL)
		{
			switch ((int32)word->value)
			{
				case KEYWORD_MOUSE:
				{
					return WorldMouse.GetX();
					break;
				}
			}
		}
		else if (word->type == POINTER)
		{
			Object * target = script->GetTarget(word);
			if (target != NULL)
			{
				Point2D position = target->GetPosition();
				return position.GetX();
			}
		}
	}
	return false;
}

Float ActionGetPositionY(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * word = script->GetWord(index + 2);

		if (word->type == NORMAL)
		{
			switch ((int32)word->value)
			{
				case KEYWORD_MOUSE:
				{
					return WorldMouse.GetY();
					break;
				}
			}
		}
		else if (word->type == POINTER)
		{
			Object * target = script->GetTarget(word);
			if (target != NULL)
			{
				Point2D position = target->GetPosition();
				return position.GetY();
			}
		}
	}
	return false;
}

Float ActionGetCoords(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * word = script->GetWord(index + 2);

		if (word->type == NORMAL)
		{
			switch ((int32)word->value)
			{
				case KEYWORD_MOUSE:
				{
					coordHelper = WorldMouse.GetPosition();
					coordHelper.SetValues((int32)((coordHelper.GetX() - 80)/32), (int32)((coordHelper.GetY() - 80)/32));
					return (uint32)&coordHelper;
					break;
				}
			}
		}
		else if (word->type == POINTER)
		{
			Object * target = script->GetTarget(word);
			if (target != NULL)
			{
				coordHelper = target->GetPosition();
				coordHelper.SetValues((int32)((coordHelper.GetX() - 80)/32), (int32)((coordHelper.GetY() - 80)/32));
				return (uint32)&coordHelper;
			}
		}
	}
	return false;
}

Float ActionGetCoordsX(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * word = script->GetWord(index + 2);

		if (word->type == NORMAL)
		{
			switch ((int32)word->value)
			{
				case KEYWORD_MOUSE:
				{
					return (int32)(WorldMouse.GetX() - 80)/32;
					break;
				}
			}
		}
		else if (word->type == POINTER)
		{
			Object * target = script->GetTarget(word);
			if (target != NULL)
			{
				Point2D position = target->GetPosition();
				return (int32)(position.GetX() - 80)/32;
			}
		}
	}
	return false;
}

Float ActionGetCoordsY(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * word = script->GetWord(index + 2);

		if (word->type == NORMAL)
		{
			switch ((int32)word->value)
			{
				case KEYWORD_MOUSE:
				{
					return (int32)(WorldMouse.GetY() - 80)/32;
					break;
				}
			}
		}
		else if (word->type == POINTER)
		{
			Object * target = script->GetTarget(word);
			if (target != NULL)
			{
				Point2D position = target->GetPosition();
				return (int32)(position.GetY() - 80)/32;
			}
		}
	}
	return false;
}

Float ActionGetMoveTarget(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * targetWord = script->GetWord(index + 2);

		Object * target = script->GetTarget(targetWord);
		if ((target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_CREEP) == true))
		{
			Creep * creep = (Creep *)target;
			return (uint32)&creep->GetMoveTarget();
		}
	}
	return false;
}

Float ActionGetLastPosition(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * targetWord = script->GetWord(index + 2);

		Object * target = script->GetTarget(targetWord);
		if ((target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_CREEP) == true))
		{
			Creep * creep = (Creep *)target;
			return (uint32)&creep->GetLastPos();
		}
	}
	return false;
}

Float ActionGetTargetPosition(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * targetWord = script->GetWord(index + 2);

		Object * target = script->GetTarget(targetWord);
		if ((target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_PROJECTILE) == true))
		{
			Projectile * projectile = (Projectile *)target;
			Creep * target = projectile->GetTarget();

			if (target == NULL)
			{
				return (uint32)&projectile->GetTargetPos();
			}
			else
			{
				return (uint32)&target->GetPosition();
			}
		}
	}
	return false;
}

Float ActionGetGrid(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * coordWord = script->GetWord(index + 2);
		Point2D * coords = (Point2D *)(uint32)coordWord->value;

		if ((coords != NULL) && (IntervalCheck(coords->GetX(), 1, GRID_SIZE - 2) == true) && (IntervalCheck(coords->GetY(), 1, GRID_SIZE - 2) == true))
		{
			Grid * grid = WorldGame.GetGrid();
			return grid->GetPath(Point2D(coords->GetX(), coords->GetY()));
		}
	}
	return false;
}

const Shape * IntersectHelper(const Object * object)
{
	Point2D position = object->GetPosition();

	if ((object->CheckType(OBJ_TYPE_TOWER) == true) || (object->CheckType(OBJ_TYPE_CREEP) == true))
	{
		return new Rect(position.GetX(), position.GetY(), position.GetX() + 32, position.GetY() + 32);
	}
	else if (object->CheckType(OBJ_TYPE_PROJECTILE) == true) 
	{
		uint32 baseSize = object->GetUInt32Value(PROJ_VAR_BASE_SIZE);
		return new Rect(position.GetX(), position.GetY(), position.GetX() + baseSize, position.GetY() + baseSize);
	}
	else if (object->CheckType(OBJ_TYPE_GAME_OBJECT) == true)
	{
		Shape * shape = ((GameObject *)object)->GetShape();
		Shape * newShape = shape->CreateInstance();
		newShape->Translate(object->GetPosition());
		return newShape;
	}
	return NULL;
}

Float ActionGetIntersection(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 3) == true)
	{
		Word * targetWord1 = script->GetWord(index + 2);
		Word * targetWord2 = script->GetWord(index + 3);

		Object * targets[] = {script->GetTarget(targetWord1),
							  script->GetTarget(targetWord2)};

		if ((targets[0] != NULL) && (targets[1] != NULL))
		{
			const Shape * shapes[] = {IntersectHelper(targets[0]),
							          IntersectHelper(targets[1])};

			if ((shapes[0] != NULL) && (shapes[1] != NULL))
			{
				bool result = Intersect(shapes[0], shapes[1]);
				for (uint32 i = 0; i < 2; i++)
				{
					delete shapes[i];
				}
				
				return result;
			}
		}
	}
	return false;
}

Float ActionGetIntersectPoint(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 3) == true)
	{
		Word * targetWord1 = script->GetWord(index + 2);
		Word * targetWord2 = script->GetWord(index + 3);

		Object * target = script->GetTarget(targetWord1);
		Point2D * point = (Point2D *)(uint32)targetWord2->value;

		if ((target != NULL) && (point != NULL))
		{
			Point2D targetPoint(point->GetX(), point->GetY());
			const Shape * shape = IntersectHelper(target);

			if (shape != NULL)
			{
				bool result = shape->ContainsPoint(targetPoint);
				delete shape;
				
				return result;
			}
		}
	}
	return false;
}

Float ActionGetGameTime(Script * script, uint32 index)
{
	return GetGameTime();
}

Float ActionGetDamage(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * targetWord = script->GetWord(index + 2);

		Object * target = script->GetTarget(targetWord);
		if ((target != NULL) && (target->GetState() == true))
		{
			if (target->CheckType(OBJ_TYPE_TOWER) == true)
			{
				Tower * tower = (Tower *)target;
				return tower->GetDamage() + FLOAT_EPSILON;
			}
			else if (target->CheckType(OBJ_TYPE_PROJECTILE) == true)
			{
				Projectile * projectile = (Projectile *)target;
				return projectile->GetDamage();
			}
			else if (target->CheckType(OBJ_TYPE_GAME_OBJECT) == true)
			{
				return target->GetUInt32Value(GOBJECT_VAR_SOURCE_DMG) + FLOAT_EPSILON;
			}
		}
	}
	return false;
}

Float ActionGetSpellDamage(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * targetWord = script->GetWord(index + 2);
		Object * target = script->GetTarget(targetWord);

		if ((target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_TOWER) == true))
		{
			Tower * tower = (Tower *)target;
			return tower->GetSpellDamage();
		}
	}
	return false;
}

Float ActionGetRange(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * targetWord = script->GetWord(index + 2);
		Object * target = script->GetTarget(targetWord);

		if ((target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_TOWER) == true))
		{
			Tower * tower = (Tower *)target;
			return tower->GetRange();
		}
	}
	return false;
}

Float ActionGetArmor(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * targetWord = script->GetWord(index + 2);

		Object * target = script->GetTarget(targetWord);
		if (target != NULL)
		{
			if (target->CheckType(OBJ_TYPE_CREEP) == true)
			{
				Creep * creep = (Creep *)target;
				return creep->GetArmor() + FLOAT_EPSILON;
			}
		}
	}
	return false;
}

Float ActionGetBounty(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * targetWord = script->GetWord(index + 2);

		Object * target = script->GetTarget(targetWord);
		if (target != NULL)
		{
			if (target->CheckType(OBJ_TYPE_CREEP) == true)
			{
				Creep * creep = (Creep *)target;
				return creep->GetBounty() + FLOAT_EPSILON;
			}
		}
	}
	return false;
}

Float ActionGetSelected(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * targetWord = script->GetWord(index + 2);

		if (targetWord->type == POINTER)
		{
			Tower * target = (Tower *)script->GetTarget(targetWord);
			if ((target != NULL) && (target->GetState() == true) && (target->CheckType(OBJ_TYPE_TOWER) == true))
			{
				Selection * selection = WorldGame.GetSelection();
				return (selection->GetIndex(target) != -1);
			}
		}
	}
	return false;
}

Float ActionGetSelectionSize(Script * script, uint32 index)
{
	Selection * selection = WorldGame.GetSelection();
	return selection->GetSize();
}

Float ActionGetWaveType(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * waveWord = script->GetWord(index + 2);

		List<Wave *> * ActiveWaves = GetActiveWaves();

		ActiveWaves->ToStart();
		for (uint32 i = 0; i < ActiveWaves->GetSize(); i++)
		{
			Wave * wave = ActiveWaves->GetCurrent();
			ActiveWaves->ToNext();

			if (wave->GetUInt32Value(WAVE_VAR_NUMBER) == (int32)waveWord->value)
			{
				return wave->GetWaveType();
			}
		}
	}
	return false;
}

Float ActionGetWaveTime(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * waveWord = script->GetWord(index + 2);

		List<Wave *> * ActiveWaves = GetActiveWaves();

		ActiveWaves->ToStart();
		for (uint32 i = 0; i < ActiveWaves->GetSize(); i++)
		{
			Wave * wave = ActiveWaves->GetCurrent();
			ActiveWaves->ToNext();

			if (wave->GetUInt32Value(WAVE_VAR_NUMBER) == (int32)waveWord->value)
			{
				return wave->GetUInt32Value(WAVE_VAR_ADD_TIME);
			}
		}
	}
	return false;
}

Float ActionGetText(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * targetWord = script->GetWord(index + 2);
		Object * target = script->GetTarget(targetWord);

		if ((target != NULL) && (target->GetState() == true))
		{
			if (target->CheckType(OBJ_TYPE_TOWER) == true)
			{
				Tower * tower = (Tower *)target;
				uint32 towerType = tower->GetTowerType();

				Word * subResult = script->GetLastSubResult();
				const TowerBase * TowerData = GetTowerData();
				subResult->word = StringCopy(TowerData[towerType].GetDescription());

				return true;
			}
			else if (target->CheckType(OBJ_TYPE_SPELL) == true)
			{
				Spell * spell = (Spell *)target;

				Word * subResult = script->GetLastSubResult();
				subResult->word = StringCopy(spell->GetText());

				return true;
			}
		}
	}
	return false;
}

Float ActionGetValue(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * value = script->GetWord(index + 1);
		Word * targetWord = script->GetWord(index + 2);

		if (targetWord->type == POINTER)
		{
			Object * target = script->GetTarget(targetWord);
			if (target != NULL)
			{
				uint8 type = ((value->type - 1) % 3) + 1;
				return target->GetValue(type, value->value);
			}
		}
	}
	return false;
}

Float ActionGetTarget(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 2) == true)
	{
		Word * targetWord = script->GetWord(index + 1);
		Word * sourceWord = script->GetWord(index + 2);

		Object * originalSource = script->GetOriginalSource();
		Object * newSource = script->GetTarget(sourceWord);

		if (newSource != NULL)
		{
			script->SetSource(newSource);
			Object * target = script->GetTarget(targetWord);
			script->SetSource(originalSource);

			return (uint32)target;
		}
	}
	return false;
}

Word GetTable[] =
{
	{KEYWORD_DISTANCE,				"DISTANCE",				NORMAL,				&ActionCalculateDistance	},
	{KEYWORD_STATE,					"STATE",				NORMAL,				&ActionGetState				},
	{KEYWORD_BASE_TYPE,				"BASE_TYPE",			NORMAL,				&ActionGetBaseType			},
	{KEYWORD_POSITION,				"POSITION",				NORMAL,				&ActionGetPosition			},
	{KEYWORD_POSITION_X,			"POS_X",				NORMAL,				&ActionGetPositionX			},
	{KEYWORD_POSITION_Y,			"POS_Y",				NORMAL,				&ActionGetPositionY			},
	{KEYWORD_COORDINATES,			"COORDS",				NORMAL,				&ActionGetCoords			},
	{KEYWORD_COORDS_X,				"COORDS_X",				NORMAL,				&ActionGetCoordsX			},
	{KEYWORD_COORDS_Y,				"COORDS_Y",				NORMAL,				&ActionGetCoordsY			},
	{KEYWORD_MOVE_TARGET,			"MOVE_TARGET",			NORMAL,				&ActionGetMoveTarget		},
	{KEYWORD_LAST_POSITION,			"LAST_POS",				NORMAL,				&ActionGetLastPosition		},
	{KEYWORD_TARGET_POSITION,		"TARGET_POS",			NORMAL,				&ActionGetTargetPosition	},
	{KEYWORD_GRID,					"GRID",					NORMAL,				&ActionGetGrid				},
	{KEYWORD_INTERSECTION,			"INTERSECTION",			NORMAL,				&ActionGetIntersection		},
	{KEYWORD_INTERSECT_POINT,		"INTERSECT_POINT",		NORMAL,				&ActionGetIntersectPoint	},
	{KEYWORD_GAME_TIME,				"GAME_TIME",			NORMAL,				&ActionGetGameTime			},
	{KEYWORD_DAMAGE,				"DAMAGE",				NORMAL,				&ActionGetDamage			},
	{KEYWORD_SPELL_DAMAGE,			"SPELL_DAMAGE",			NORMAL,				&ActionGetSpellDamage		},
	{KEYWORD_RANGE,					"RANGE",				NORMAL,				&ActionGetRange				},
	{KEYWORD_ARMOR,					"ARMOR",				NORMAL,				&ActionGetArmor				},
	{KEYWORD_BOUNTY,				"BOUNTY",				NORMAL,				&ActionGetBounty			},
	{KEYWORD_SELECTED,				"SELECTED",				NORMAL,				&ActionGetSelected			},
	{KEYWORD_SELECTION_SIZE,		"SELECTION_SIZE",		NORMAL,				&ActionGetSelectionSize		},
	{KEYWORD_WAVE_TYPE,				"WAVE_TYPE",			NORMAL,				&ActionGetWaveType			},
	{KEYWORD_WAVE_TIME,				"WAVE_TIME",			NORMAL,				&ActionGetWaveTime			},
	{KEYWORD_TEXT,					"TEXT",					NORMAL,				&ActionGetText				},
};

Float ActionGet(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 1) == true)
	{
		Word * target = script->GetWord(index + 1);

		uint16 listSize = sizeof(GetTable)/sizeof(Word);
		for (uint16 i = 0; i < listSize; i++)
		{
			if ((target->value == GetTable[i].value) && (target->type == GetTable[i].type))
			{
				return GetTable[i].handler(script, index);
			}
		}

		if (IntervalCheck(target->type, INT32_VAR, FLOAT_CONST) == true)
		{
			return ActionGetValue(script, index);
		}
		else if (target->type == POINTER)
		{
			return ActionGetTarget(script, index);
		}
	}

	return 0;
}*/