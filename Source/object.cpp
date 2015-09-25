#include "type.h"
#include "utility.h"
#include "point.h"
#include "object.h"

uint32 Object::GUIDcounter = 0;

Object::Object()
{
	name = NULL;
	int32Values = NULL;
	uInt32Values = NULL;
	floatValues = NULL;
	floatValuesCount = 0;
	int32ValuesCount = 0;
	uInt32ValuesCount = 0;
	SetPosition(Point3D(0, 0, 0));
	GUID = GUIDcounter;
	GUIDcounter++;
}

Object::~Object()
{
	Unload();	
}

uint32 Object::GetValueCount(uint8 type)
{
	assert(IntervalCheck(type, OBJ_VAL_INT32, OBJ_VAL_FLOAT));
	switch (type)
	{
		case OBJ_VAL_INT32:
		{
			return this->int32ValuesCount;
			break;
		}
		case OBJ_VAL_UINT32:
		{
			return this->uInt32ValuesCount;
			break;
		}
		case OBJ_VAL_FLOAT:
		{
			return this->floatValuesCount;
			break;
		}
		default:
		{
			//Should be stopped by the assert but just in case...
			return 0;
			break;
		}
	}
}
void Object::AllocateInt32Values(uint32 size)
{
	if (int32Values == NULL)
	{
		int32ValuesCount = size;
		int32Values = new int32[size];
		for (uint32 i = 0; i < size; i++)
		{
			int32Values[i] = 0;
		}
	}
}

void Object::AllocateUInt32Values(uint32 size)
{
	if (uInt32Values == NULL)
	{
		uInt32ValuesCount = size;
		uInt32Values = new uint32[size];
		for (uint32 i = 0; i < size; i++)
		{
			uInt32Values[i] = 0;
		}
	}
}

void Object::AllocateFloatValues(uint32 size)
{
	if (floatValues == NULL)
	{
		floatValuesCount = size;
		floatValues = new Float[size];
		for (uint32 i = 0; i < size; i++)
		{
			floatValues[i] = 0;
		}
	}
}

void Object::SetInt32Value(uint32 valueIndex, int32 value)
{
	if (valueIndex < int32ValuesCount)
	{
		int32Values[valueIndex] = value;
	}
}

void Object::SetUInt32Value(uint32 valueIndex, int32 value)
{
	if (valueIndex < uInt32ValuesCount)
	{
		uInt32Values[valueIndex] = ZeroFit(value);
	}
}

void Object::SetFloatValue(uint32 valueIndex, Float value)
{
	if (valueIndex < floatValuesCount)
	{
		floatValues[valueIndex] = value;
	}
}

void Object::SetValue(uint8 type, uint32 valueIndex, Float value)
{
	if (type == OBJ_VAL_INT32)
	{
		SetInt32Value(valueIndex, value);
	}
	else if (type == OBJ_VAL_UINT32)
	{
		SetUInt32Value(valueIndex, value);
	}
	else if (type == OBJ_VAL_FLOAT)
	{
		SetFloatValue(valueIndex, value);
	}
}

void Object::ModInt32Value(uint32 valueIndex, int32 value)
{
	if (valueIndex < int32ValuesCount)
	{
		int32Values[valueIndex] += value;
	}
}

void Object::ModUInt32Value(uint32 valueIndex, int32 value)
{
	if (valueIndex < uInt32ValuesCount)
	{
		int32 val = GetUInt32Value(valueIndex) + value;
		uInt32Values[valueIndex] = ZeroFit(val);
	}
}

void Object::ModFloatValue(uint32 valueIndex, Float value)
{
	if (valueIndex < floatValuesCount)
	{
		floatValues[valueIndex] += value;
	}
}

void Object::ModValue(uint8 type, uint32 valueIndex, Float value)
{
	if (type == OBJ_VAL_INT32)
	{
		ModInt32Value(valueIndex, value);
	}
	else if (type == OBJ_VAL_UINT32)
	{
		ModUInt32Value(valueIndex, value);
	}
	else if (type == OBJ_VAL_FLOAT)
	{
		ModFloatValue(valueIndex, value);
	}
}

void Object::SetName(const char * nameStr)
{
	if (nameStr != NULL)
	{/*
		uint8 nameStrLength = 0;
		for (; nameStr[nameStrLength] != NULL; nameStrLength++);*/

		Delete(name);
		name = StringCopy(nameStr);
			
		/*	new char[nameStrLength+1];

		for (uint8 i = 0; nameStr[i] != NULL; i++)
		{
			name[i] = nameStr[i];
		}

		name[nameStrLength] = '\0';*/
	}
}

void Object::Copy(const Object& obj)
{
	if ((&obj != this) && (type == obj.GetType()))
	{
		SetState(obj.GetState());
		SetName(obj.GetName());
		uInt32ValuesCount = obj.uInt32ValuesCount;
		int32ValuesCount = obj.int32ValuesCount;
		floatValuesCount = obj.floatValuesCount;
		memcpy(uInt32Values, obj.uInt32Values, (obj.uInt32ValuesCount * sizeof(uint32)));
		memcpy(int32Values, obj.int32Values, (obj.int32ValuesCount * sizeof(int32)));
		memcpy(floatValues, obj.floatValues, (obj.floatValuesCount * sizeof(Float)));
		position = obj.position;
	}
}

void Object::Unload()
{
	SetState(false);
	SetType(0);
	Delete(name);
	Delete(int32Values);
	Delete(uInt32Values);
	Delete(floatValues);
	int32ValuesCount = 0;
	uInt32ValuesCount = 0;
	floatValuesCount = 0;
	SetPosition(Point3D(0, 0, 0));
}