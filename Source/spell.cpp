#include <stdio.h>
#include "tinyxml.h"
#include "utility.h"
#include "type.h"
#include "object.h"
#include "script.h"
#include "script.h"
#include "spell.h"
#include "game.h"

Spell * SpellData = NULL;
uint32 spellDataSize = 0;

Spell::Spell()
{
	SetState(true);
	SetType(OBJ_TYPE_SPELL);
	text = NULL;
	source = NULL;
	target = NULL;
	AllocateInt32Values(SPELL_INT32_VARS);
	AllocateUInt32Values(SPELL_UINT32_VARS);
}

Spell::~Spell()
{
	Unload();
}

Spell * Spell::CreateInstance(Object * source, Object * target) const
{
	Spell * spellInst = new Spell;

	spellInst->Copy(*this);

	if (text != NULL)
	{
		spellInst->text = StringCopy(text);
			
		/*	new char[strlen(text)+1];

		for (uint32 j = 0; j < strlen(text); j++)
		{
			spellInst->text[j] = text[j];
		}
		spellInst->text[strlen(text)] = '\0';*/
	}

	const char * spellScript = script.GetScript();
	spellInst->script.LoadScript(spellScript, spellInst);
	spellInst->source = source;
	spellInst->target = target;

	return spellInst;
}

Float Spell::Activate()
{
	script.SetSource(this);
	return script.ActivateScript(true);
}

bool Spell::CheckCharges() const
{
	if ((GetInt32Value(SPELL_VAR_CHARGES) == 0) || ((GetInt32Value(SPELL_VAR_CHARGES) != 0) && (GetUInt32Value(SPELL_VAR_REMAINING_CHARGES) > 0)))
	{
		return true;
	}
	return false;
}

bool Spell::CheckCooldown() const
{
	if ((GetLastCastTime() == 0) || (GetGameTime() - GetLastCastTime() >= GetCooldown()))
	{
		return true;
	}
	return false;
}

void Spell::Unload()
{
	SetState(false);
	Delete(text);
	source = NULL;
	script.Unload();
}

const Spell * GetSpellData()
{
	return SpellData;
}

const Spell * GetSpell(uint32 ID)
{
	return (ID >= spellDataSize) ? NULL : &SpellData[ID];
}

uint32 GetSpellDataSize()
{
	return spellDataSize;
}

bool LoadSpellData()
{
	if (SpellData == NULL)
	{
		FILE * file = fopen("Assets\\XML\\spell.xml", "r");

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

		TiXmlDocument SpellXML;
		SpellXML.Parse(fileData);

		TiXmlNode * node = SpellXML.FirstChild("SpellData");
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
					spellDataSize = i;
					SpellData = new Spell[spellDataSize];
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

		for (uint32 i = 0; i < spellDataSize; i++)
		{
			int32 ID = 0;
			element->Attribute("ID", &ID);

			SpellData[i].SetUInt32Value(SPELL_VAR_ID, ID);
			SpellData[i].SetName(element->Attribute("Name"));

			for (uint8 j = SPELL_VAR_VISIBILITY; j <= SPELL_VAR_CATEGORY; j++)
			{
				if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
				{
					const char * text = secondaryElement->GetText();
					Float word = LookupWord(text, LOOKUP_VALUE);
					if (word == NOT_FOUND)
					{
						word = atof(text);
					}

					SpellData[i].SetUInt32Value(j, word);
				}
				secondaryElement = secondaryElement->NextSiblingElement();
			}

			for (uint8 j = SPELL_VAR_COST; j <= SPELL_VAR_HOTKEY; j++)
			{
				if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
				{
					const char * text = secondaryElement->GetText();
					Float word = LookupWord(text, LOOKUP_VALUE);
					if (word == NOT_FOUND)
					{
						word = (j == SPELL_VAR_HOTKEY) ? ToUpper(text[0]) : atof(text);
					}

					SpellData[i].SetInt32Value(j, word);
					if (j == SPELL_VAR_CHARGES)
					{
						SpellData[i].SetUInt32Value(SPELL_VAR_REMAINING_CHARGES, ZeroFit(word));
					}
					else if ((j == SPELL_VAR_STACKS) && (word == 0))
					{
						SpellData[i].SetInt32Value(SPELL_VAR_STACKS, 1);
					}
				}
				secondaryElement = secondaryElement->NextSiblingElement();
			}

			if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
			{
				const char * text = secondaryElement->GetText();
				SpellData[i].text = StringCopy(text);
					
				/*	new char[strlen(text)+1];
				for (uint32 j = 0; j < strlen(text); j++)
				{
					SpellData[i].text[j] = text[j];
				}
				SpellData[i].text[strlen(text)] = '\0';*/
			}
			secondaryElement = secondaryElement->NextSiblingElement();

			if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
			{
				const char * text = secondaryElement->GetText();

				SpellData[i].script.LoadScript(text, &SpellData[i]);
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

		SpellXML.Clear();
		Delete(fileData);

		return true;
	}
	return false;
}
		/*FILE * file = fopen("Assets\\XML\\spell.xml", "r");

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

		TiXmlDocument SpellXML;
		SpellXML.Parse(fileData);

		TiXmlNode * node = SpellXML.FirstChild("SpellData");
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
					spellDataSize = i;
					SpellData = new Spell[spellDataSize];
				}
				break;
			}
			lastElement = lastElement->NextSiblingElement();
		}

		node = element->FirstChild();
		TiXmlElement * secondaryElement = node->ToElement();

		for (uint32 i = 0; i < spellDataSize; i++)
		{
			int32 ID = 0;
			element->Attribute("ID", &ID);

			SpellData[i].SetUInt32Value(SPELL_VAR_ID, ID);
			SpellData[i].SetName(element->Attribute("Name"));

			for (uint8 j = SPELL_VAR_VISIBILITY; j <= 14; j++)
			{
				if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
				{
					const char * text = secondaryElement->GetText();
					Float word = LookupWord(text, LOOKUP_INDEX);

					if (word == NOT_FOUND)
					{
						if (j == SPELL_VAR_HOTKEY)
						{
							word = ((text[0] >= 'a') && (text[0] <= 'z')) ? text[0] - 32 : text[0];
						}
						else
						{
							word = atof(text);
						}
					}
					else
					{
						word = WordList[word].value;
					}

					SpellData[i].SetUInt32Value(j, word);	
				}
				secondaryElement = secondaryElement->NextSiblingElement();
			}

			if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
			{
				const char * text = secondaryElement->GetText();
				SpellData[i].text = new char[strlen(text)+1];
				for (uint32 j = 0; j < strlen(text); j++)
				{
					SpellData[i].text[j] = text[j];
				}
				SpellData[i].text[strlen(text)] = '\0';
			}
			secondaryElement = secondaryElement->NextSiblingElement();

			if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
			{
				const char * text = secondaryElement->GetText();

				Script onInit;
				SpellData[i].script.LoadScript(text, &SpellData[i]);
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

		SpellXML.Clear();
		Delete(fileData);

		return true;
	}
	return false;
}*/

void UnloadSpellData()
{
	delete [] SpellData;
	spellDataSize = 0;
}