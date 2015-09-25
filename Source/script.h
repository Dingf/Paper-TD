#ifndef INC_SCRIPT_H
#define INC_SCRIPT_H

#include "type.h"
#include "object.h"
#include "list.h"

#define LOOKUP_VALUE 0
#define LOOKUP_WORD 1

enum ScriptReturnCodes
{
	RETURN_TYPE_NORMAL = 0,
	RETURN_TYPE_SYSTEM = 1,
	RETURN_STOP = 2,
	RETURN_SUCCESS = 1,
	RETURN_FAILURE = 0,
	RETURN_CONTINUE = -1,
};

struct Script;
struct Word
{
	char * word;
	uint8 type;
	Float value;
	Float (*handler)(Script * script);
	Word * subcommands;
};

struct Script
{
	public:
		Script();
		~Script();

		const char * GetScript() const { return script; }

		      Object * GetSource();
		const Object * GetSource() const;

		      Object * GetOriginalSource()       { return source; }
		const Object * GetOriginalSource() const { return source; }

		      Object * GetSelection()       { return selection; }
		const Object * GetSelection() const { return selection; }

		void SetSourceFlag(bool state);

		void SetSource(Object * source);
		void SetSelection(Object * selection);
		void SetVariable(const char * name, Float value);

		bool LoadScript(const char * text, Object * source);
		
		Word * AddVariable(Word * variable);
		Word * AddVariable(const char * name, Float value = 0);

		Word * GetVariable(const char * name);

		Word * AddWord(const char * wordText);
		Word * PeekWord(int32 offset);
		Word * PeekOriginalWord(int32 offset);
		Word * GetNextWord();
		Word * GetNextOriginalWord();
		Word * GetLastSubResult();

		Script * GetSubscript(uint32 index);
		Object * GetTarget(Word * word);

		Float ActivateScript(bool clearFlag);
		Float ExecuteNextCommand();

		bool VerifyArguments(uint32 amount);

		void ClearSubscriptResults();
		void ClearVariables();

		void CopyVariables(const Script& script);
		void Copy(const Script& script);
		void Unload();

	private:
		bool ParseScript();
		uint32 ParseSubscript(uint32 position);

		void ClearSubscriptVariables();

		bool sourceFlag;
		char * script;
		Object * source;
		Object * selection;
		Script * superscript;

		List<Word *> words;
		List<Word *> subResults;
		List<Word *> variables;	
		List<Script *> subscripts;
};

int32 LookupWord(const char * word, bool type, Word * lastWord = NULL);

#endif