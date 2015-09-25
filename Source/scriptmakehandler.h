#ifndef INC_SCRIPT_MAKE_HANDLER_H
#define INC_SCRIPT_MAKE_HANDLER_H

#include "type.h"

enum MakeKeywords
{
	MAKE_KEYWORD_POINT2D,
	MAKE_KEYWORD_RECT,
	MAKE_KEYWORD_VARIABLE,
	MAKE_KEYWORD_ANIMATION,
	MAKE_KEYWORD_GAME_OBJECT,
	MAKE_KEYWORD_EMITTER,
};

Float MakePoint2D(Script * script);
Float MakeRect(Script * script);
Float MakeNewVariable(Script * script);
Float MakeNewAnimation(Script * script);
Float MakeNewGameObject(Script * script);
Float MakeNewParticleEmitter(Script * script);

#endif