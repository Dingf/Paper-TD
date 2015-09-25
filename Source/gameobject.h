#ifndef INC_GAME_OBJECT_H
#define INC_GAME_OBJECT_H

#include "type.h"
#include "object.h"
#include "shape.h"
#include "list.h"
#include "script.h"

enum GameObjectInt32Values
{
	GOBJECT_VAR_DURATION,
	GOBJECT_INT32_VARS
};

enum GameObjectUInt32Values
{
	GOBJECT_VAR_ID,
	GOBJECT_VAR_START_TIME,
	GOBJECT_VAR_LAST_UPDATE_TIME,
	GOBJECT_VAR_PERIODIC_TIME,
	GOBJECT_VAR_TIME_DIFFERENCE,
	GOBJECT_VAR_ANIMATION_ID,
	GOBJECT_VAR_SOURCE_DMG,
	GOBJECT_UINT32_VARS
};

class GameObject : public Object
{
	public:
		GameObject();
		~GameObject();

		uint32 GetID() const { return GetUInt32Value(GOBJECT_VAR_ID); }

		      Script * GetScript()       { return &script; }
		const Script * GetScript() const { return &script; }

		      Shape * GetShape()       { return shape; }
		const Shape * GetShape() const { return shape; }

			  Object * GetSource()		 { return source; }
		const Object * GetSource() const { return source; }

		void SetSource(Object * source) { this->source = source; }

		GameObject * AddGameObject(Object * source, const Point2D& position) const;

		bool SetShapeValues(Float v1, Float v2, Float v3, Float v4, uint8 type = NULL);
		void Unload();

		friend bool LoadGameObjectData();
	private:
		Script script, onAdd;
		Shape * shape;
		Object * source;
};

bool LoadGameObjectData();
void UnloadGameObjectData();

const GameObject * GetGameObjectData();
uint32 GetGameObjectDataSize();

List<GameObject *> * GetActiveGameObjects();
void ClearActiveGameObjects();

void GameObjectMaintenance();

#endif