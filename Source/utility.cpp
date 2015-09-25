#include <string>
#include <sstream>
#include "type.h"
#include "utility.h"

void Delete(void * data)
{
	if (data != NULL)
	{
		delete [] data;
		data = NULL;
	}
}

char ToLower(char character)
{
	return ((character >= 'A') && (character <= 'Z')) ? character + 32 : character;
}

void ToLower(char * string)
{
	for (uint32 i = 0; string[i] != NULL; i++)
	{
		string[i] = ToLower(string[i]);
	}
}


char ToUpper(char character)
{
	return ((character >= 'a') && (character <= 'z')) ? character - 32 : character;
}

void ToUpper(char * string)
{
	for (uint32 i = 0; string[i] != NULL; i++)
	{
		string[i] = ToUpper(string[i]);
	}
}

char * StringCopy(const char * string)
{
	if (string != NULL)
	{
		char * newString = new char[strlen(string)+1];
		for (uint32 i = 0; i < strlen(string); i++)
		{
			newString[i] = string[i];
		}
		newString[strlen(string)] = '\0';

		return newString;	
	}
	return NULL;
}

int32 StringChar(const char * string, char character)
{
	character -= ((character >= 'a') && (character <= 'z')) ? 32 : 0;
	for (uint32 i = 0; string[i] != NULL; i++)
	{
		char stringChar = string[i];
		stringChar -= ((stringChar >= 'a') && (stringChar <= 'z')) ? 32 : 0;
		if (stringChar == character)
		{
			return i;
		}
	}
	return -1;
}


uint32 Next2(uint32 value)
{
	for (uint8 i = 0; i < 32; i++)
	{
		if (((1 << i)) > value)
		{
			return (1 << i);
		}
	}
	return 0;
}

uint8 StringToHex8(const char * string, uint32 start)
{
	uint8 value = 0;
	for (uint32 i = start; i < start + 2; i++)
	{
		if ((string[i] >= '0') && (string[i] <= '9'))
		{
			value += (string[i] - 48) << (4 * (1 ^ (i - start)));
		}
		else if ((string[i] >= 'A') && (string[i] <= 'F'))
		{
			value += (string[i] - 55) << (4 * (1 ^ (i - start)));
		}
		else if ((string[i] >= 'a') && (string[i] <= 'f'))
		{
			value += (string[i] - 87) << (4 * (1 ^ (i - start)));
		}
		else
		{
			return 0;
		}
	}
	return value;
}

uint16 StringToHex16(const char * string, uint32 start)
{
	uint32 value = 0;
	for (uint32 i = start; i < start + 4; i += 2)
	{
		value += StringToHex8(string, i) << (4 * (2 - (i - start)));
	}
	return value;
}

uint32 StringToHex32(const char * string, uint32 start)
{
	uint32 value = 0;
	for (uint32 i = start; i < start + 8; i += 2)
	{
		value += StringToHex8(string, i) << (4 * (6 - (i - start)));
	}
	return value;
}

char * FloatToString(Float value)
{
	std::ostringstream oss;
	if ((oss << value) == false)
	{
		return "";
	}

	return StringCopy(oss.str().c_str());
}