#include "type.h"
#include "math.h"
#include "utility.h"
#include "shapes.h"
#include "script.h"
#include "scriptdef.h"
#include "gameobject.h"
#include "creep.h"
#include "projectile.h"
#include "tower.h"
#include "grid.h"
#include "game.h"
//
//Float ActionAddGameObject(Script * script, uint32 index)
//{
//	if (VerifyArguments(script, index, 3) == true)
//	{
//		Word * gameObjWord = script->GetWord(index + 1);
//		Word * posWord = script->GetWord(index + 2);
//		Word * sourceWord = script->GetWord(index + 3);
//
//		Point2D * position = (Point2D *)(uint32)posWord->value;
//
//		if ((position != NULL) && (IntervalCheck(gameObjWord->value, 0, GetGameObjectDataSize()) == true))
//		{
//			Object * source = script->GetTarget(sourceWord);
//			Point2D targetPos(position->GetX(), position->GetY());
//
//			const GameObject * GameObjectData = GetGameObjectData();
//			const GameObject * gameObject = &GameObjectData[(uint32)gameObjWord->value];
//
//			if ((source != NULL) && (gameObject->AddGameObject(source, targetPos) != NULL))
//			{
//				return true;
//			}
//		}
//	}
//	return false;
//}
//
//Float ActionRemoveGameObject(Script * script, uint32 index)
//{
//	if (VerifyArguments(script, index, 2) == true)
//	{
//		Word * gameObjWord = script->GetWord(index + 1);
//		Word * sourceWord = script->GetWord(index + 2);
//
//		Object * source = script->GetTarget(sourceWord);
//
//		if ((source != NULL) && (source->GetState() == true))
//		{
//			List<GameObject *> * ActiveGameObjects = GetActiveGameObjects();
//
//			ActiveGameObjects->ToStart();
//			for (uint32 i = 0; i < ActiveGameObjects->GetSize(); i++)
//			{
//				GameObject * gameObj = ActiveGameObjects->GetCurrent();
//				ActiveGameObjects->ToNext();
//
//				if ((gameObj != NULL) && (gameObj->GetState() == true) && (gameObj->GetID() == gameObjWord->value) && (gameObj->GetSource() == source))
//				{
//					gameObj->SetState(false);
//				}
//			}
//			return true;
//		}
//	}
//	return false;
//}


Float RotateGameObject(Script * script)
{
	if (script->VerifyArguments(3) == true)
	{
		Word * rotatePosWord = script->GetNextWord();
		Point2D rotatePosition(0, 0);
		Point2D * rotatePos = (Point2D *)(uint32)rotatePosWord->value;
		if (rotatePos != NULL)
		{
			rotatePosition.SetValues(rotatePos->GetX(), rotatePos->GetY());
		}

		Word * angleWord = script->GetNextWord();

		Word * directionWord = script->GetNextWord();
		Vector3D direction(0, 0, 0);
		Point2D * dirVector = (Point2D *)(uint32)directionWord->value;
		if (dirVector != NULL)
		{
			direction.SetValues(dirVector->GetX(), dirVector->GetY(), 0);
		}

		GameObject * source = (GameObject *)script->GetSource();
		Shape * shape = source->GetShape();
		
		if ((rotatePos != NULL) && (source != NULL) && (source->CheckType(OBJ_TYPE_GAME_OBJECT) == true) && (shape != NULL))
		{
			Float distance = CalculateDistance(rotatePosition, source->GetPosition());
			if (angleWord->value != NULL)
			{
				Vector3D sourceDirection(rotatePosition, source->GetPosition());
				Float angle = sourceDirection.GetZeroAngleD() + angleWord->value;
				
				source->SetPosition(rotatePosition.GetX() + (distance * CosD(angle)),
									rotatePosition.GetY() + (distance * SinD(angle)));

				direction.SetValues(rotatePosition, source->GetPosition());
				direction.SetUnitVector();
			}
			else if (dirVector != NULL)
			{
				direction -= rotatePosition;
				direction.SetUnitVector();

				source->SetPosition(rotatePosition.GetX() + (direction.GetX() * distance),
									rotatePosition.GetY() + (direction.GetY() * distance));
			}
			else
			{
				return false;
			}

			if ((shape->GetType() == SHAPE_TYPE_LINE) || (shape->GetType() == SHAPE_TYPE_LINE_SEGMENT))
			{
				Line * line = (Line *)shape;
				Line * newLine = (Line *)line->CreateInstance();
				newLine->Translate(source->GetPosition());	

				Vector3D v1(rotatePosition, newLine->GetPoint1());
				Vector3D v2(rotatePosition, newLine->GetPoint2());

				Float baseAngle = direction.GetZeroAngleD();

				Float v1Length = v1.GetLength();
				Float v2Length = v2.GetLength();
				Float angle1 = v1.GetZeroAngleD() - baseAngle;
				Float angle2 = v2.GetZeroAngleD() - baseAngle;

				Point2D p1((v1Length * CosD(angle1)),
						   (v1Length * SinD(angle1)));
				Point2D p2((v2Length * CosD(angle2)),
						   (v2Length * SinD(angle2)));

				line->SetValues(p1 + rotatePosition, p2 + rotatePosition);

				line->Translate(!source->GetPosition());

				delete newLine;
			}
			else if (shape->GetType() == SHAPE_TYPE_CONE)
			{
				Cone * cone = (Cone *)shape;
				cone->SetValues(cone->GetVertex(), Vector3D(dirVector->GetX(), dirVector->GetY(), 0), cone->GetHeight(), cone->GetAngle());
			}
			return true;
		}
	}
	return false;
}

//Float ActionSetGameObjectValues(Script * script, uint32 index)
//{
//	if (VerifyArguments(script, index, 4) == true)
//	{
//		Word * value1 = script->GetWord(index + 1);
//		Word * value2 = script->GetWord(index + 2);
//		Word * value3 = script->GetWord(index + 3);
//		Word * value4 = script->GetWord(index + 4);
//
//		Object * source = script->GetSource();
//		if ((source != NULL) && (source->GetState() == true) && (source->CheckType(OBJ_TYPE_GAME_OBJECT) == true))
//		{
//			GameObject * gameObj = (GameObject *)source;
//			return gameObj->SetShapeValues(value1->value, value2->value, value3->value, value4->value);
//		}
//	}
//	return false;
//}

Float DamageArea(Script * script)
{
	if (script->VerifyArguments(4) == true)
	{
		Word * damageType = script->GetNextWord();
		Word * timeWord = script->PeekWord(2);
		Word * cycles = script->PeekWord(3);

		Object * originalSource = script->GetOriginalSource();
		GameObject * target = (GameObject *)script->GetSource();
		if ((target != NULL) && (target->CheckType(OBJ_TYPE_GAME_OBJECT) == true))
		{
			uint32 currentTime = GetGameTime();
			uint32 startTime = target->GetUInt32Value(GOBJECT_VAR_START_TIME);

			if (((startTime + (timeWord->value * cycles->value)) > currentTime) || (cycles->value <= 0))
			{
				List<Creep *> * ActiveCreeps = GetActiveCreeps();

				Shape * shape = target->GetShape();
				Shape * newShape = shape->CreateInstance();
				newShape->Translate(target->GetPosition());

				ActiveCreeps->ToStart();
				for (uint32 i = 0; i < ActiveCreeps->GetSize(); i++)
				{
					Creep * creep = ActiveCreeps->GetCurrent();
					ActiveCreeps->ToNext();

					if ((creep != NULL) && (creep->GetState() == true))
					{
						Point2D creepPos = creep->GetPosition();
						Rect creepRect(creepPos.GetX(), creepPos.GetY(), creepPos.GetX() + 32, creepPos.GetY() + 32);

						if (Intersect(newShape, &creepRect) == true)
						{
							script->SetSource(creep);

							Word * damageWord = script->PeekWord(1);
							Float damage = (timeWord->value == 0) ? damageWord->value : (damageWord->value * target->GetUInt32Value(GOBJECT_VAR_TIME_DIFFERENCE))/timeWord->value;

							if (damageType->value == CREEP_VAR_HITPOINTS)
							{
								if ((creep->GetInt32Value(CREEP_VAR_SPELL_IMMUNE) == false) && (creep->GetInt32Value(CREEP_VAR_SPLASH_IMMUNE) == false))
								{
									creep->InflictDamage(damage, creep->GetArmor());
								}
							}
							else
							{
								creep->ModValue(damageType->type, damageType->value, -damage);
							}
							if ((creep->GetState() == false) || (creep->GetFloatValue(CREEP_VAR_MAX_HITPOINTS) <= 0))
							{
								creep->SetState(false);
								AddTowerKill(target);
							}
						}
					}
				}

				delete newShape;
				script->SetSource(originalSource);

				//Properly offset the words list
				for (uint32 i = 0; i < 3; i++)
				{
					script->GetNextOriginalWord();
				}
				return true;
			}	
		}
		script->SetSource(originalSource);
	}
	return false;
}

Float SpellAreaTowers(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * spellWord = script->GetNextWord();

		GameObject * source = (GameObject *)script->GetSource();
		if ((source != NULL) && (source->CheckType(OBJ_TYPE_GAME_OBJECT) == true))
		{
			const Spell * spell = GetSpell(spellWord->value);
			if (spell != NULL)
			{
				Shape * shape = source->GetShape();
				Shape * newShape = shape->CreateInstance();
				newShape->Translate(source->GetPosition());

				Grid * grid = WorldGame.GetGrid();
				Tower ** TowerData = grid->GetData();

				for (uint32 i = 1; i < GRID_SIZE - 1; i++)
				{
					for (uint32 j = 1; j < GRID_SIZE - 1; j++)
					{
						Tower * tower = TowerData[(i*GRID_SIZE)+j];
						if ((tower != NULL) && (tower->GetState() == true) && (tower->ActiveSpell(spell, source) == false))
						{
							Point2D towerPos = tower->GetPosition();
							Rect towerRect(towerPos.GetX(), towerPos.GetY(), towerPos.GetX() + 32, towerPos.GetY() + 32);

							if (Intersect(newShape, &towerRect) == true)
							{
								if (tower->ActiveSpell(spell, source) == false)
								{
									tower->ApplySpell(spell, source);
								}
							}
							else
							{
								tower->RemoveSpell(spell, source);
							}
						}
					}
				}
				delete newShape;
				return true;
			}
		}
	}
	return false;
}

Float SpellAreaProjectiles(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * spellWord = script->GetNextWord();

		GameObject * source = (GameObject *)script->GetSource();
		if ((source != NULL) && (source->CheckType(OBJ_TYPE_GAME_OBJECT) == true))
		{
			const Spell * spell = GetSpell(spellWord->value);
			if (spell != NULL)
			{
				Shape * shape = source->GetShape();
				Shape * newShape = shape->CreateInstance();
				newShape->Translate(source->GetPosition());

				List<Projectile *> * ActiveProjectiles = GetActiveProjectiles();

				ActiveProjectiles->ToStart();
				for (uint32 i = 0; i < ActiveProjectiles->GetSize(); i++)
				{
					Projectile * projectile = ActiveProjectiles->GetCurrent();
					ActiveProjectiles->ToNext();

					uint32 baseSize = projectile->GetUInt32Value(PROJ_VAR_BASE_SIZE);
					Point2D position = projectile->GetPosition();
					Rect bounds(position.GetX(), position.GetY(), position.GetX() + baseSize, position.GetY() + baseSize);
					
					if (Intersect(newShape, &bounds) == true)
					{
						if (projectile->ActiveSpell(spell, source) == false)
						{
							projectile->ApplySpell(spell, source);
						}
					}
					else
					{
						projectile->RemoveSpell(spell, source);
					}
				}
				delete newShape;
				return true;
			}
		}
	}
	return false;
}

Float SpellAreaCreeps(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * spellWord = script->GetNextWord();

		GameObject * source = (GameObject *)script->GetSource();
		if ((source != NULL) && (source->CheckType(OBJ_TYPE_GAME_OBJECT) == true))
		{
			const Spell * spell = GetSpell(spellWord->value);
			if (spell != NULL)
			{
				Shape * shape = source->GetShape();
				Shape * newShape = shape->CreateInstance();
				newShape->Translate(source->GetPosition());

				List<Creep *> * ActiveCreeps = GetActiveCreeps();

				ActiveCreeps->ToStart();
				for (uint32 i = 0; i < ActiveCreeps->GetSize(); i++)
				{
					Creep * creep = ActiveCreeps->GetCurrent();
					ActiveCreeps->ToNext();

					Point2D position = creep->GetPosition();
					Rect bounds(position.GetX(), position.GetY(), position.GetX() + 32, position.GetY() + 32);
					
					if (Intersect(newShape, &bounds) == true)
					{
						if (creep->ActiveSpell(spell, source) == false)
						{
							creep->ApplySpell(spell, source);
						}
					}
					else
					{
						creep->RemoveSpell(spell, source);
					}
				}
				delete newShape;
				return true;
			}
		}
	}
	return false;
}
//
//Float ActionSpellArea(Script * script)
//{
//	if (script->VerifyArguments(2) == true)
//	{
//		Word * spellWord = script->GetNextWord();
//		Word * targetWord = script->GetNextWord();
//
//		GameObject * source = (GameObject *)script->GetSource();
//		if ((source != NULL) && (source->CheckType(OBJ_TYPE_GAME_OBJECT) == true) && (targetWord->type == LOOP_TYPE) && (IntervalCheck(targetWord->value, KEYWORD_LOOP_TYPE_TOWER, KEYWORD_LOOP_TYPE_CREEP) == true))
//		{
//			const Spell * spell = GetSpell(spellWord->value);
//			if (spell != NULL)
//			{
//				Shape * shape = source->GetShape();
//				Shape * newShape = shape->CreateInstance();
//				newShape->Translate(source->GetPosition());
//
//				if (targetWord->value == KEYWORD_LOOP_TYPE_TOWER)
//				{
//					Grid * grid = WorldGame.GetGrid();
//					Tower ** TowerData = grid->GetData();
//
//					for (uint32 i = 1; i < GRID_SIZE - 1; i++)
//					{
//						for (uint32 j = 1; j < GRID_SIZE - 1; j++)
//						{
//							Tower * tower = TowerData[(i*GRID_SIZE)+j];
//							if ((tower != NULL) && (tower->GetState() == true) && (tower->ActiveSpell(spell, source) == false))
//							{
//								Point2D towerPos = tower->GetPosition();
//								Rect towerRect(towerPos.GetX(), towerPos.GetY(), towerPos.GetX() + 32, towerPos.GetY() + 32);
//
//								if (Intersect(newShape, &towerRect) == true)
//								{
//									if (tower->ActiveSpell(spell, source) == false)
//									{
//										tower->ApplySpell(spell, source);
//									}
//								}
//								else
//								{
//									tower->RemoveSpell(spell, source);
//								}
//							}
//						}
//					}
//				}
//				else
//				{
//					List<SpellObject *> * objData = (targetWord->value == KEYWORD_LOOP_TYPE_CREEP) ? 
//											        (List<SpellObject *> *)GetActiveCreeps() :
//										            (List<SpellObject *> *)GetActiveProjectiles();
//
//					objData->ToStart();
//					for (uint32 i = 0; i < objData->GetSize(); i++)
//					{
//						SpellObject * object = objData->GetCurrent();
//						objData->ToNext();
//
//						Point2D position = object->GetPosition();
//						Rect objRect;
//
//						if (targetWord->value == KEYWORD_LOOP_TYPE_PROJECTILE)
//						{
//							uint32 baseSize = object->GetUInt32Value(PROJ_VAR_BASE_SIZE);
//							objRect.SetValues(position.GetX(), position.GetY(), position.GetX() + baseSize, position.GetY() + baseSize);
//						}
//						else
//						{
//							objRect.SetValues(position.GetX(), position.GetY(), position.GetX() + 32, position.GetY() + 32);
//						}
//						if (Intersect(newShape, &objRect) == true)
//						{
//							if (object->ActiveSpell(spell, source) == false)
//							{
//								object->ApplySpell(spell, source);
//							}
//						}
//						else
//						{
//							object->RemoveSpell(spell, source);
//						}
//						/*if ( && (Intersect(newShape, &objRect) == true))
//						{
//							
//						}
//						else if ((object->ActiveSpell(spell, source) == true) && (Intersect(newShape, &objRect) == false))
//						{
//							
//						}*/
//					}
//				}
//
//				delete newShape;
//				return true;
//			}
//		}
//	}
//	return false;
//};
