#include "type.h"
#include "point.h"
#include "rect.h"
#include "script.h"
#include "scriptdef.h"
#include "animation.h"
#include "emitter.h"
#include "gameobject.h"

Point2D point2Dhelper;
Rect rectHelper;

Float MakePoint2D(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * xWord = script->GetNextWord();
		Word * yWord = script->GetNextWord();

		point2Dhelper.SetValues(xWord->value, yWord->value);
		return (uint32)&point2Dhelper;
	}
	return false;
}

Float MakeRect(Script * script)
{
	if (script->VerifyArguments(4) == true)
	{
		Word * leftWord = script->GetNextWord();
		Word * topWord = script->GetNextWord();
		Word * rightWord = script->GetNextWord();
		Word * bottomWord = script->GetNextWord();

		rectHelper.SetValues(leftWord->value, topWord->value, rightWord->value, bottomWord->value);
		return (uint32)&rectHelper;
	}
	return false;
}

Float MakeNewVariable(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * varWord = script->GetNextWord();

		if ((varWord != NULL) && (varWord->type == NUMERIC))
		{
			Word * variable = script->AddVariable(varWord->word);
			if (variable != NULL)
			{
				return RETURN_CONTINUE;
			}
		}
	}
	return false;
}

Float MakeNewAnimation(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * animWord = script->GetNextWord();
		Word * posWord = script->GetNextWord();

		Point2D * position = (Point2D *)(uint32)posWord->value;
		if ((position != NULL) && (IntervalCheck(animWord->value, 0, GetAnimationDataSize())))
		{
			Object * source = script->GetSource();
			Point2D targetPos(position->GetX(), position->GetY());

			if (source != NULL) 
			{
				const Animation * animation = GetAnimation(animWord->value);
				Animation * newAnim = animation->AddAnimation(source);
				if (newAnim != NULL)
				{
					newAnim->SetPosition(targetPos);
					return true;
				}
			}
		}
	}
	return false;
}

Float MakeNewGameObject(Script * script)
{
	if (script->VerifyArguments(3) == true)
	{
		Word * gameObjWord = script->GetNextWord();
		Word * posWord = script->GetNextWord();
		Word * sourceWord = script->GetNextWord();

		Point2D * position = (Point2D *)(uint32)posWord->value;

		if ((position != NULL) && (IntervalCheck(gameObjWord->value, 0, GetGameObjectDataSize()) == true))
		{
			Object * source = script->GetTarget(sourceWord);
			Point2D targetPos(position->GetX(), position->GetY());

			const GameObject * GameObjectData = GetGameObjectData();
			const GameObject * gameObject = &GameObjectData[(uint32)gameObjWord->value];

			if ((source != NULL) && (gameObject->AddGameObject(source, targetPos) != NULL))
			{
				return true;
			}
		}
	}
	return false;
}

Float MakeNewParticleEmitter(Script * script)
{
	if (script->VerifyArguments(3) == true)
	{
		Word * particleWord = script->GetNextWord();
		Word * posWord = script->GetNextWord();
		Word * sourceWord = script->GetNextWord();

		Object * source = script->GetTarget(sourceWord);
		Point2D * position = (Point2D *)(uint32)posWord->value;

		if ((position != NULL) && (IntervalCheck(particleWord->value, 0, GetEmitterDataSize()) == true))
		{
			Point2D targetPos(position->GetX(), position->GetY());

			const Emitter * EmitterData = GetEmitterData();
			const Emitter * emitter = &EmitterData[(uint32)particleWord->value];

			if ((source != NULL) && (emitter->AddEmitter(source, targetPos) != NULL))
			{
				return true;
			}
		}
	}
	return false;
}

