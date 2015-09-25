#ifndef INC_OBJECT_H
#define INC_OBJECT_H

#ifndef assert
#include <assert.h>
#include <string.h>
#endif
#include "type.h"
#include "point.h"

enum ObjectValueTypes
{
	OBJ_VAL_INT32 = 1,
	OBJ_VAL_UINT32 = 2,
	OBJ_VAL_FLOAT = 3
};

enum ObjectTypes
{
	OBJ_TYPE_NONE = 0,
	OBJ_TYPE_SPELL_OBJ = 1,
	OBJ_TYPE_TOWER = 2,
	OBJ_TYPE_PROJECTILE = 4,
	OBJ_TYPE_CREEP = 8,
	OBJ_TYPE_GAME_OBJECT = 16,
	OBJ_TYPE_SPELL = 32,
	OBJ_TYPE_ANIMATION = 64,
	OBJ_TYPE_EMITTER = 128
};

class Object
{
	public:
		Object& operator = (const Object& obj)
		{
			Copy(obj);
			return *this;
		}

		Object();
		Object(const Object& obj) { *this = obj; }
		~Object();

		bool GetState() const { return state; }
		bool CheckType(uint16 type) const { return ((this->type & type) == type); }

		uint16 GetType() const { return type; }
		uint32 GetGUID() const { return GUID; }

		const Point3D& GetPosition() const { return position; }

		int32 GetInt32Value(uint32 valueIndex) const 
		{ 
			if ((state == true) && (valueIndex < int32ValuesCount))
			{
				return (int32Values == NULL) ? NULL : int32Values[valueIndex];
			}
			return NULL;
		}
		uint32 GetUInt32Value(uint32 valueIndex) const 
		{ 
			if ((state == true) && (valueIndex < uInt32ValuesCount))
			{
				return (uInt32Values == NULL) ? NULL : uInt32Values[valueIndex];
			}
			return NULL;
		}
		Float GetFloatValue(uint32 valueIndex) const 
		{ 
			if ((state == true) && (valueIndex < floatValuesCount))
			{
				return (floatValues == NULL) ? NULL : floatValues[valueIndex];
			}
			return NULL;
		}
		Float GetValue(uint8 valType, uint32 valueIndex) const
		{
			if (valType == OBJ_VAL_INT32)
			{
				return GetInt32Value(valueIndex);
			}
			else if (valType == OBJ_VAL_UINT32)
			{
				return GetUInt32Value(valueIndex);
			}
			else if (valType == OBJ_VAL_FLOAT)
			{
				return GetFloatValue(valueIndex);
			}
			return NULL;
		}
		char * GetName() const { return name; }

		uint32 GetValueCount(uint8 type);

		void AllocateInt32Values(uint32 size);
		void AllocateUInt32Values(uint32 size);
		void AllocateFloatValues(uint32 size);

		void SetInt32Value(uint32 valueIndex, int32 value);
		void SetUInt32Value(uint32 valueIndex, int32 value);
		void SetFloatValue(uint32 valueIndex, Float value);
		void SetValue(uint8 type, uint32 valueIndex, Float value);

		void ModInt32Value(uint32 valueIndex, int32 value);
		void ModUInt32Value(uint32 valueIndex, int32 value);
		void ModFloatValue(uint32 valueIndex, Float value);
		void ModValue(uint8 type, uint32 valueIndex, Float value);

		void SetName(const char * nameStr);
		void SetState(bool newState) { state = newState; }
		void SetType(uint16 newType) { type = newType; }

		void SetPosition(Float u, Float v) { position = Point3D(u, v, 0); }
		void SetPosition(Float x, Float y, Float z) { position = Point3D(x, y, z); }
		void SetPosition(const Point3D& newPos) { position = newPos; }

		virtual void Copy(const Object& obj);

		void Unload();
		
	protected:
		bool state;
		char * name;
		Float * floatValues;
		int32 * int32Values;
		uint32 * uInt32Values;
		uint32 floatValuesCount;
		uint32 int32ValuesCount;
		uint32 uInt32ValuesCount;
		uint16 type;
		uint32 GUID;
		Point3D position;
		static uint32 GUIDcounter;
};

#endif