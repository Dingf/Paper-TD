#include "tinyxml.h"
#include "type.h"
#include "utility.h"
#include "point.h"
#include "object.h"
#include "shape.h"
#include "shapes.h"
#include "list.h"
#include "script.h"
#include "animation.h"
#include "gameobject.h"
#include "game.h"

GameObject * GameObjectData = NULL;
uint32 gameObjectDataSize = 0;

List<GameObject *> ActiveGameObjects;
List<GameObject *> RemovedGameObjects;

GameObject::GameObject()
{
	SetState(true);
	SetType(OBJ_TYPE_GAME_OBJECT);
	AllocateInt32Values(GOBJECT_INT32_VARS);
	AllocateUInt32Values(GOBJECT_UINT32_VARS);

	shape = NULL;
}
GameObject::~GameObject()
{
	script.Unload();
	if (shape != NULL)
	{
		delete shape;
		shape = NULL;
	}
}

GameObject * GameObject::AddGameObject(Object * source, const Point2D& position) const
{
	if ((GetState() == true) && (source != NULL))
	{
		ActiveGameObjects.Append(new GameObject);

		GameObject * newGObject = ActiveGameObjects.GetLast();
		newGObject->Copy(*this);
		newGObject->source = source;

		newGObject->SetPosition(position);

		const char * gameObjScript = script.GetScript();
		newGObject->script.LoadScript(gameObjScript, newGObject);

		const char * onAddScript = onAdd.GetScript();
		newGObject->onAdd.LoadScript(onAddScript, newGObject);

		newGObject->shape = shape->CreateInstance();

		uint32 animID = GetUInt32Value(GOBJECT_VAR_ANIMATION_ID);

		if ((animID > 0) && (animID < GetAnimationDataSize()))
		{
			const Animation * AnimationData = GetAnimationData();
			const Animation * targetAnim = &AnimationData[animID];

			Animation * newAnim = targetAnim->AddAnimation(newGObject);

			if ((newAnim != NULL) && (shape->GetType() != SHAPE_TYPE_CIRCLE))
			{
				newAnim->SetPosition(position);
			}
			else if (newAnim != NULL)
			{
				const Sprite * sprite = targetAnim->GetSprite();

				uint32 xOffset = (sprite == NULL) ? 0 : sprite->GetWidth()/2.0;
				uint32 yOffset = (sprite == NULL) ? 0 : sprite->GetHeight()/2.0;

				newAnim->SetPosition(Point2D(position.GetX() - xOffset, position.GetY() - yOffset));
			}
		}

		if (source != NULL)
		{
			if (source->CheckType(OBJ_TYPE_TOWER) == true)
			{
				Tower * tower = (Tower *)source;
				newGObject->SetUInt32Value(GOBJECT_VAR_SOURCE_DMG, tower->GetSpellDamage());
			}
			else if (source->CheckType(OBJ_TYPE_PROJECTILE) == true)
			{
				Projectile * projectile = (Projectile *)source;
				newGObject->SetUInt32Value(GOBJECT_VAR_SOURCE_DMG, projectile->GetDamage());
				newGObject->source = projectile->GetSource();
			}
			else if (source->CheckType(OBJ_TYPE_GAME_OBJECT) == true)
			{
				GameObject * gameObj = (GameObject *)source;
				Tower * tower = (Tower *)gameObj->GetSource();

				if ((tower != NULL) && (tower->CheckType(OBJ_TYPE_TOWER) == true))
				{
					newGObject->source = gameObj->GetSource();
					newGObject->SetUInt32Value(GOBJECT_VAR_SOURCE_DMG, tower->GetDamage());
				}
			}
		}

		newGObject->onAdd.ActivateScript(true);
		newGObject->SetUInt32Value(GOBJECT_VAR_START_TIME, GetGameTime());
		newGObject->SetUInt32Value(GOBJECT_VAR_LAST_UPDATE_TIME, GetGameTime());

		return newGObject;
	}
	return NULL;
}

bool GameObject::SetShapeValues(Float v1, Float v2, Float v3, Float v4, uint8 type)
{
	if (type == NULL)
	{
		type = shape->GetType();
	}
	if (shape != NULL)
	{
		delete shape;
	}
	switch (type)
	{
		case SHAPE_TYPE_LINE:
		{
			shape = new Line(Point3D(v1, v2, 0),
							 Point3D(v3, v4, 0));
			break;
		}
		case SHAPE_TYPE_LINE_SEGMENT:
		{
			shape = new LineSegment(Point3D(v1, v2, 0),
									Point3D(v3, v4, 0));
			break;
		}
		case SHAPE_TYPE_RECT:
		{
			shape = new Rect(v1, v2, v3, v4);
			break;
		}
		case SHAPE_TYPE_CIRCLE:
		{
			shape = new Circle(Point3D(v1, v2, 0), v3);
			break;
		}
		case SHAPE_TYPE_CONE:
		{
			shape = new Cone(Point3D(v1, v2, 0), Vector3D(0, 0, 0), v3, v4);
			break;
		}
		default:
		{
			return false;
		}
	}
	shape->Translate(!position);
	return true;
}

void GameObject::Unload()
{
	SetState(false);
}

bool LoadGameObjectData()
{
	if (GameObjectData == NULL)
	{
		FILE * file = fopen("Assets\\XML\\gameobject.xml", "r");

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

		TiXmlDocument GObjectXML;
		GObjectXML.Parse(fileData);

		TiXmlNode * node = GObjectXML.FirstChild("GameObjectData");
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
					gameObjectDataSize = i;
					GameObjectData = new GameObject[gameObjectDataSize];
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

		for (uint32 i = 0; i < gameObjectDataSize; i++)
		{
			int32 ID = 0;
			element->Attribute("ID", &ID);

			GameObjectData[i].SetUInt32Value(GOBJECT_VAR_ID, ID);
			GameObjectData[i].SetName(element->Attribute("Name"));

			if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
			{
				const char * text = secondaryElement->GetText();
				Float word = LookupWord(text, LOOKUP_VALUE);
				if (word == NOT_FOUND)
				{
					word = atof(text);
				}

				if (IntervalCheck(word, SHAPE_TYPE_LINE, SHAPE_TYPE_CONE) == true)
				{
					secondaryElement = secondaryElement->NextSiblingElement();
					const char * text = secondaryElement->GetText();
					int32 values[4] = { 0, 0, 0, 0 };

					sscanf(text, "%d %d %d %d", &values[0], &values[1], &values[2], &values[3]);

					GameObjectData[i].SetShapeValues(values[0], values[1], values[2], values[3], word);
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

				GameObjectData[i].SetInt32Value(GOBJECT_VAR_DURATION, word);
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

				GameObjectData[i].SetUInt32Value(GOBJECT_VAR_ANIMATION_ID, word);
			}
			secondaryElement = secondaryElement->NextSiblingElement();

			if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
			{
				const char * text = secondaryElement->GetText();
				GameObjectData[i].onAdd.LoadScript(text, &GameObjectData[i]);
			}
			secondaryElement = secondaryElement->NextSiblingElement();

			if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
			{
				const char * text = secondaryElement->GetText();
				GameObjectData[i].script.LoadScript(text, &GameObjectData[i]);
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

		GObjectXML.Clear();
		Delete(fileData);

		return true;
	}
	return false;
}

const GameObject * GetGameObjectData()
{
	return GameObjectData;
}

uint32 GetGameObjectDataSize()
{
	return gameObjectDataSize;
}

List<GameObject *> * GetActiveGameObjects()
{
	return &ActiveGameObjects;
}

void GameObjectMaintenance()
{
	while (RemovedGameObjects.GetSize() > 0)
	{
		if (RemovedGameObjects[0] != NULL)
		{
			delete RemovedGameObjects[0];
		}
		RemovedGameObjects.Remove(0);
	}

	for (uint32 i = 0; i < ActiveGameObjects.GetSize(); i++)
	{
		GameObject * gameObj = ActiveGameObjects[i];

		uint32 startTime = gameObj->GetUInt32Value(GOBJECT_VAR_START_TIME);
		uint32 currentTime = GetGameTime();

		if (gameObj->GetState() == true)
		{
			Object * source = gameObj->GetSource();
			if ((source != NULL) && (source->GetState() == false))
			{
				gameObj->SetSource(NULL);
			}

			gameObj->SetUInt32Value(GOBJECT_VAR_TIME_DIFFERENCE, currentTime - gameObj->GetUInt32Value(GOBJECT_VAR_LAST_UPDATE_TIME));
			if (((gameObj->GetInt32Value(GOBJECT_VAR_DURATION) == 0) && (source->GetState() == true)) || ((currentTime - startTime) <= gameObj->GetInt32Value(GOBJECT_VAR_DURATION)))
			{
				Script * script = gameObj->GetScript();
				if (script->ActivateScript(true) == true)
				{
					gameObj->SetUInt32Value(GOBJECT_VAR_LAST_UPDATE_TIME, GetGameTime());
				}
			}
			else
			{
				gameObj->SetState(false);
			}
		}
		else
		{
			RemovedGameObjects.Append(gameObj);
			ActiveGameObjects.Remove(i);
			i--;
		}
	}
}

void ClearActiveGameObjects()
{
	while (ActiveGameObjects.GetSize() > 0)
	{
		if (ActiveGameObjects[0] != NULL)
		{
			delete ActiveGameObjects[0];
		}
		ActiveGameObjects.Remove(0);
	}
	while (RemovedGameObjects.GetSize() > 0)
	{
		if (RemovedGameObjects[0] != NULL)
		{
			delete RemovedGameObjects[0];
		}
		RemovedGameObjects.Remove(0);
	}
}

void UnloadGameObjectData()
{
	delete [] GameObjectData;
	gameObjectDataSize = 0;
}