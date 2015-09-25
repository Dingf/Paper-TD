#include <string>
#include "type.h"
#include "utility.h"
#include "object.h"
#include "list.h"
#include "script.h"
#include "scriptdef.h"
#include "animation.h"
#include "gameobject.h"
#include "creep.h"
#include "projectile.h"
#include "tower.h"
#include "game.h"

Script::Script()
{
	sourceFlag = NULL;
	script = NULL;
	source = NULL;
	selection = NULL;
	superscript = NULL;
}

Script::~Script()
{
	Unload();
}

Object * Script::GetSource()
{ 
	if ((source != NULL) && (source->CheckType(OBJ_TYPE_SPELL) == true) && (sourceFlag == false))
	{
		Spell * spell = (Spell *)source;
		return spell->GetTarget();
	}
	else
	{
		return source;
	}
}

const Object * Script::GetSource() const
{
	if ((source != NULL) && (source->CheckType(OBJ_TYPE_SPELL) == true) && (sourceFlag == false))
	{
		Spell * spell = (Spell *)source;
		return spell->GetTarget();
	}
	else
	{
		return source;
	}
}

void Script::SetSourceFlag(bool state)
{
	sourceFlag = state;

	subscripts.ToStart();
	for (uint32 i = 0; i < subscripts.GetSize(); i++)
	{
		Script * subscript = subscripts.GetCurrent();
		subscripts.ToNext();

		subscript->SetSourceFlag(state);
	}
}

void Script::SetSource(Object * source)
{
	this->source = source;

	subscripts.ToStart();
	for (uint32 i = 0; i < subscripts.GetSize(); i++)
	{
		Script * subscript = subscripts.GetCurrent();
		subscripts.ToNext();

		subscript->SetSource(source);
	}
}

void Script::SetSelection(Object * selection)
{
	if (this->selection != selection)
	{
		this->selection = selection;

		if (superscript != NULL)
		{
			superscript->SetSelection(selection);
		}

		subscripts.ToStart();
		for (uint32 i = 0; i < subscripts.GetSize(); i++)
		{
			Script * subscript = subscripts.GetCurrent();
			subscripts.ToNext();

			subscript->SetSelection(selection);
		}
	}
}

void Script::SetVariable(const char * name, Float value)
{
	variables.ToStart();
	for (uint32 i = 0; i < variables.GetSize(); i++)
	{
		Word * variable = variables.GetCurrent();
		variables.ToNext();

		if (strcmp(variable->word, name) == 0)
		{
			variable->value = value;

			subscripts.ToStart();
			for (uint32 j = 0; j < subscripts.GetSize(); j++)
			{
				Script * subscript = subscripts.GetCurrent();
				subscripts.ToNext();

				subscript->SetVariable(name, value);
			}
			break;
		}
	}
}

bool Script::LoadScript(const char * text, Object * source)
{
	Unload();

	this->source = source;
	if (text != NULL)
	{
		script = StringCopy(text);
	}

	return ParseScript();
}

bool Script::ParseScript()
{
	if (script != NULL)
	{
		words.Unload();

		uint16 wordCount = 0;
		for (uint32 i = 0; i < strlen(script); i++)
		{
			char character = script[i];
			if ((character == ' ') || (character == '\n'))
			{
				wordCount++;
			}
		}

		bool literalFlag = false;
		uint32 scriptLength = strlen(script);
		uint32 wordPos = 0;
		while (true)
		{
			char word[256] = "";
			for (uint32 i = 0; i < 256; i++)
			{
				char character = script[wordPos+i];
				if ((wordPos + i) >= scriptLength)
				{
					AddWord(word);
					return true;
				}
				else if (character == '"')
				{
					literalFlag ^= 1;
					AddWord(word);
					wordPos += i;
					break;
				}
				else if ((character == '(') && (literalFlag == false))
				{
					Float index = subscripts.GetSize();
					char indexWord[32] = "";

					itoa(index, indexWord, 10);

					Word * word = AddWord(indexWord);

					if (word != NULL)
					{
						Delete(word->word);
						word->word = "SUBSCRIPT";
						word->type = SUBSCRIPT;

						uint32 result = ParseSubscript(wordPos+i+1);
						if (result == false)
						{
							return false;
						}
						wordPos += result;
					}
				}
				else if (((character == ' ') ||  (character == '\n') || (character == ';')) && (literalFlag == false))
				{
					if (strlen(word) > 0)
					{
						AddWord(word);
					}
					if (character == ';')
					{
						AddWord(";");
					}
					wordPos += i;
					break;
				}
				else
				{
					word[i] = character;
				}
			}
			
			wordPos++;
		}
	}
	return false;
}

uint32 Script::ParseSubscript(uint32 position)
{
	uint8 depth = 0;
	for (uint32 i = position; script[i] != '\0'; i++)
	{
		if (script[i] == '(')
		{
			depth++;
		}
		else if ((script[i] == ')') && (depth > 0))
		{
			depth--;
		}
		else if ((script[i] == ')') && (depth == 0))
		{
			subscripts.Append(new Script);
			Script * subscript = subscripts.GetLast();

			subscript->script = new char[i-position+1];
			subscript->script[i-position] = '\0';

			for (uint32 j = 0; j < i - position; j++)
			{
				subscript->script[j] = script[position+j];
			}

			subscript->SetSource(GetOriginalSource());
			subscript->superscript = this;
			
			return (subscript->ParseScript() == false) ? 0 : strlen(subscript->script);
		}
		else if (i >= (strlen(script) - 1))
		{
			return false;
		}
	}
	return false;
}

Word * Script::AddVariable(Word * variable)
{
	if ((variable != NULL) && (variable->type == VARIABLE))
	{
		variables.Append(variable);

		subscripts.ToStart();
		for (uint32 i = 0; i < subscripts.GetSize(); i++)
		{
			Script * subscript = subscripts.GetCurrent();
			subscripts.ToNext();

			subscript->AddVariable(variable);
		}

		return variable;
	}
	return NULL;
}

Word * Script::AddVariable(const char * name, Float value)
{
	if (strlen(name) > 0)
	{
		if (GetVariable(name) != NULL)
		{
			return false;
		}

		variables.Append(new Word);
		Word * variable = variables.GetLast();

		variable->handler = &TakeNoAction;
		variable->type = VARIABLE;
		variable->value = value;
		variable->word = StringCopy(name);
			
		/*	new char[strlen(name)+1];
		for (uint32 i = 0; i < strlen(name); i++)
		{
			variable->word[i] = name[i];
		}
		variable->word[strlen(name)] = '\0';*/

		subscripts.ToStart();
		for (uint32 i = 0; i < subscripts.GetSize(); i++)
		{
			Script * subscript = subscripts.GetCurrent();
			subscripts.ToNext();

			subscript->AddVariable(variable);
		}

		return variable;
	}
	return NULL;
}

Word * Script::GetVariable(const char * name)
{
	variables.ToStart();
	for (uint32 i = 0; i < variables.GetSize(); i++)
	{
		Word * variable = variables.GetCurrent();
		variables.ToNext();

		if (strcmp(name, variable->word) == 0)
		{
			return variable;
		}
	}
	return NULL;
}

Word * Script::AddWord(const char * wordText)
{
	if (strlen(wordText) > 0)
	{
		Word * lastWord = (words.GetSize() == 0) ? NULL : words.GetLast();
		uint32 word = LookupWord(wordText, LOOKUP_WORD, lastWord);
		if (word == NOT_FOUND)
		{
			variables.ToStart();
			for (uint32 i = 0; i < variables.GetSize(); i++)
			{
				Word * variable = variables.GetCurrent();
				variables.ToNext();

				if (strcmp(wordText, variable->word) == 0)
				{
					words.Append(variable);
					return words.GetLast();
				}
			}

			words.Append(new Word);
			Word * newWord = words.GetLast();

			newWord->value = atof(wordText);
			newWord->word = StringCopy(wordText);
			newWord->subcommands = NULL;
				
			/*	new char[strlen(wordText)+1];
			for (uint32 i = 0; i < strlen(wordText); i++)
			{
				newWord->word[i] = wordText[i];
			}
			newWord->word[strlen(wordText)] = '\0';*/
			newWord->type = NUMERIC;
			newWord->handler = &TakeNoAction;

			return newWord;
		}
		else
		{
			words.Append((Word *)word);
			return words.GetLast();
		}
	}
	return NULL;
}

Word * Script::PeekWord(int32 offset)
{
	Word * word = NULL;
	if (offset > 0)
	{
		for (uint32 i = 0; i < offset; i++)
		{
			if (words.ToNext() == false)
			{
				return NULL;
			}
		}
		word = words.GetCurrent();
		for (uint32 i = 0; i < offset; i++)
		{
			words.ToPrevious();
		}
	}
	else
	{
		offset = AbsVal(offset);
		for (uint32 i = 0; i < offset; i++)
		{
			if (words.ToPrevious() == false)
			{
				return NULL;
			}
		}
		word = words.GetCurrent();
		for (uint32 i = 0; i < offset; i++)
		{
			words.ToNext();
		}
	}

	if (word->type == SUBSCRIPT)
	{
		uint32 subscriptID = word->value;
		uint32 iterator = 0;

		subResults.Append(new Word);
		Word * subResult = subResults.GetLast();

		subResult->handler = &TakeNoAction;
		subResult->type = NUMERIC;
		subResult->word = NULL;
		subResult->subcommands = NULL;

		subscripts[subscriptID]->words.ToStart();
		subResult->value = subscripts[subscriptID]->ExecuteNextCommand();

		return subResult;
	}
	else if (word->type == NUMERIC)
	{
		Word * variable = GetVariable(word->word);
		if (variable != NULL)
		{
			return variable;
		}
	}
	return word;
}

Word * Script::PeekOriginalWord(int32 offset)
{
	if (offset > 0)
	{
		for (uint32 i = 0; i < offset; i++)
		{
			if (words.ToNext() == false)
			{
				return NULL;
			}
		}
		Word * word = words.GetCurrent();
		for (uint32 i = 0; i < offset; i++)
		{
			words.ToPrevious();
		}
		return word;
	}
	else
	{
		offset = AbsVal(offset);
		for (uint32 i = 0; i < offset; i++)
		{
			if (words.ToPrevious() == false)
			{
				return NULL;
			}
		}
		Word * word = words.GetCurrent();
		for (uint32 i = 0; i < offset; i++)
		{
			words.ToNext();
		}
		return word;
	}
}

Word * Script::GetNextWord()
{
	if (words.ToNext() == true)
	{
		Word * word = words.GetCurrent();

		if (word->type == SUBSCRIPT)
		{
			uint32 subscriptID = word->value;
			uint32 iterator = 0;

			subResults.Append(new Word);
			Word * subResult = subResults.GetLast();

			subResult->handler = &TakeNoAction;
			subResult->type = NUMERIC;
			subResult->word = NULL;
			subResult->subcommands = NULL;

			subscripts[subscriptID]->words.ToStart();
			subResult->value = subscripts[subscriptID]->ExecuteNextCommand();

			return subResult;
		}
		else if (word->type == NUMERIC)
		{
			Word * variable = GetVariable(word->word);
			if (variable != NULL)
			{
				return variable;
			}
		}
		return word;
	}
	return NULL;
}

Word * Script::GetNextOriginalWord()
{
	if (words.ToNext() == true)
	{
		return words.GetCurrent();
	}
	return NULL;
}

Word * Script::GetLastSubResult()
{
	return subResults.GetLast();
}

Script * Script::GetSubscript(uint32 index)
{
	if (index < subscripts.GetSize())
	{
		return subscripts[index];
	}
	return NULL;
}

Object * Script::GetTarget(Word * word)
{
	if (word->type == NUMERIC)
	{
		return (Object *)(uint32)word->value;
	}
	else if (word->type == POINTER)
	{
		Object * source = GetSource();
		switch ((uint32)word->value)
		{
			case KEYWORD_TARGET_SELF:
			{
				return source;
				break;
			}
			case KEYWORD_TARGET_SELECTION:
			{
				return selection;
				break;
			}
			case KEYWORD_TARGET_TARGET:
			{
				if (source != NULL)
				{
					if (source->CheckType(OBJ_TYPE_TOWER) == true)
					{
						return ((Tower *)source)->GetTarget();
					}
					else if (source->CheckType(OBJ_TYPE_PROJECTILE) == true)
					{
						return ((Projectile *)source)->GetTarget();
					}
					else if (source->CheckType(OBJ_TYPE_SPELL) == true)
					{
						return ((Spell *)source)->GetTarget();
					}
				}
				break;
			}
			case KEYWORD_TARGET_PROJECTILE:
			{
				if ((source != NULL) && (source->CheckType(OBJ_TYPE_TOWER) == true))
				{
					return ((Tower *)source)->GetProjectile();
				}
				break;
			}
			case KEYWORD_TARGET_SOURCE:
			{
				if (source != NULL)
				{
					if (source->CheckType(OBJ_TYPE_PROJECTILE) == true)
					{
						return ((Projectile *)source)->GetSource();
					}
					if (source->CheckType(OBJ_TYPE_ANIMATION) == true)
					{
						return ((Animation *)source)->GetSource();
					}
					if (source->CheckType(OBJ_TYPE_GAME_OBJECT) == true)
					{
						return ((GameObject *)source)->GetSource();
					}
					if (source->CheckType(OBJ_TYPE_SPELL) == true)
					{
						return ((Spell *)source)->GetSource();
					}
				}
				break;
			}
			case KEYWORD_TARGET_THIS:
			{
				return GetOriginalSource();
				break;
			}
			case KEYWORD_TARGET_WORLDGAME:
			{
				return &WorldGame;
				break;
			}
		}
	}
	return NULL;
}

Float Script::ActivateScript(bool clearFlag)
{
	if (clearFlag == true)
	{
		ClearSubscriptResults();
		ClearVariables();
		SetSourceFlag(false);
	}
	if (words.GetState() == true)
	{
		words.ToStart();
		do
		{
			Float result = ExecuteNextCommand();
			if ((result == false) || (result == RETURN_STOP))
			{
				return result;
			}
		}
		while (words.ToNext() == true);

		return true;
	}
	return false;
}

Float Script::ExecuteNextCommand()
{
	while (true)
	{
		Word * word = words.GetCurrent();
		Word * nextWord = GetNextOriginalWord();

		words.ToPrevious();

		Float result = RETURN_CONTINUE;
		Word * subCommands = word->subcommands;

		for (uint32 i = 0;; i++)
		{
			if ((subCommands == NULL) || (subCommands[i].handler == NULL))
			{
				result = word->handler(this);
				break;
			}
			else if (nextWord == &subCommands[i])
			{
				break;
			}
		}
	
		if ((result != RETURN_CONTINUE) || (nextWord == NULL))
		{
			if (nextWord == NULL)
			{
				GetNextOriginalWord();
			}
			return result;
		}
		else
		{
			words.ToNext();
		}
	}

	return false;
}

bool Script::VerifyArguments(uint32 amount)
{
	bool result = true;
	for (uint32 i = 1; i <= amount; i++)
	{
		Word * word = GetNextOriginalWord();
		if ((word == NULL) || ((word->value == KEYWORD_END) && (word->type == ACTION)))
		{
			result = false;
		}
	}
	for (uint32 i = 1; i <= amount; i++)
	{
		words.ToPrevious();
	}
	return true;
}

void Script::ClearSubscriptResults()
{
	subResults.ToStart();
	for (uint32 i = 0; i < subResults.GetSize(); i++)
	{
		Word * subResult = subResults.GetCurrent();
		subResults.ToNext();

		if (subResult->word != NULL)
		{
			delete subResult->word;
		}
		delete subResult;
	}
	subResults.Unload();

	subscripts.ToStart();
	for (uint32 i = 0; i < subscripts.GetSize(); i++)
	{
		Script * subscript = subscripts.GetCurrent();
		subscripts.ToNext();

		subscript->ClearSubscriptResults();
	}
}

void Script::ClearSubscriptVariables()
{
	while (variables.GetSize() > 0)
	{
		variables.Remove(0);
	}

	subscripts.ToStart();
	for (uint32 i = 0; i < subscripts.GetSize(); i++)
	{
		Script * subscript = subscripts.GetCurrent();
		subscripts.ToNext();

		subscript->ClearSubscriptVariables();
	}
}

void Script::ClearVariables()
{
	while (variables.GetSize() > 0)
	{
		Word * variable = variables.GetFirst();

		delete variable->word;
		delete variable;
		variables.Remove(0);
	}

	subscripts.ToStart();
	for (uint32 i = 0; i < subscripts.GetSize(); i++)
	{
		Script * subscript = subscripts.GetCurrent();
		subscripts.ToNext();
		 
		subscript->ClearSubscriptVariables();
	}
}

void Script::CopyVariables(const Script& script)
{
	for (uint32 i = 0; i < script.variables.GetSize(); i++)
	{
		AddVariable(script.variables[i]->word);
		Word * newVar = variables.GetLast();

		newVar->value = script.variables[i]->value;
	}
}

void Script::Copy(const Script& script)
{
	/*this->script = StringCopy(script.script);

	for (uint32 i = 0; i < script.words.GetSize(); i++)
	{
		Word * word = script.words[i];
		if (word->type == NUMERIC)
		{
			words.Append(new Word(*word));
			Word * newWord = words.GetLast();
			newWord->word = StringCopy(word->word);
		}
		else if (word->type == SUBSCRIPT)
		{
			words.Append(new Word(*word));
		}
		else
		{
			words.Append(word);
		}
	}

	CopyVariables(script);
	source = script.source;
	selection = script.selection;

	for (uint32 i = 0; i < script.subscripts.GetSize(); i++)
	{
		subscripts.Append(new Script);
		Script * subscript = subscripts.GetLast();
		subscript->Copy(*script.subscripts[i]);
	}*/

	LoadScript(script.script, script.source);
	CopyVariables(script);
	selection = script.selection;
}

void Script::Unload()
{
	Delete(script);
	script = NULL;

	ClearSubscriptResults();
	ClearVariables();

	while (words.GetSize() > 0)
	{
		if (words[0]->type == NUMERIC)
		{
			delete words[0]->word;
			delete words[0];
		}
		else if (words[0]->type == SUBSCRIPT)
		{
			delete words[0];
		}
		words.Remove(0);
	}
	while (subscripts.GetSize() > 0)
	{
		delete subscripts[0];
		subscripts.Remove(0);
	}

	source = NULL;
}