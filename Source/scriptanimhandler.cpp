#include "type.h"
#include "utility.h"
#include "point.h"
#include "rect.h"
#include "script.h"
#include "scriptdef.h"
#include "animation.h"
#include "game.h"

//Float ActionAnimate(Script * script, uint32 index)
//{
//	if (script->VerifyArguments(script, index, 2) == true)
//	{
//		Word * animWord = script->GetWord(index + 1);
//		Word * posWord = script->GetWord(index + 2);
//
//		Point2D * position = (Point2D *)(uint32)posWord->value;
//		if ((position != NULL) && (IntervalCheck(animWord->value, 0, GetAnimationDataSize())))
//		{
//			Object * source = script->GetSource();
//			Point2D targetPos(position->GetX(), position->GetY());
//
//			const Animation * AnimationData = GetAnimationData();
//			const Animation * animation = &AnimationData[(uint32)animWord->value];
//
//			if (source != NULL) 
//			{
//				Animation * newAnim = animation->AddAnimation(source);
//				if (newAnim != NULL)
//				{
//					newAnim->SetPosition(targetPos);
//					return true;
//				}
//			}
//		}
//	}
//	return false;
//}

/*Float ActionAnimateTime(Script * script, uint32 index)
{
	if (VerifyArguments(script, index, 4) == true)
	{
		Word * animWord = script->GetWord(index + 1);
		Word * posWord = script->GetWord(index + 2);
		Word * intervalWord = script->GetWord(index + 3);
		Word * durationWord = script->GetWord(index + 4);
		Word * offsetWord = script->GetWord(index + 5);

		Animation * source = (Animation *)script->GetSource();
		if ((source != NULL) && (source->CheckType(OBJ_TYPE_ANIMATION) == true))
		{
			int32 lastAnimTime = source->GetUInt32Value(ANIM_VAR_LAST_ANIM_TIME);
			int32 animStartTime = source->GetUInt32Value(ANIM_VAR_START_TIME);
			if (((GetGameTime() - lastAnimTime) >= intervalWord->value) && ((GetGameTime() - animStartTime) > offsetWord->value) && ((GetGameTime() - animStartTime) < (durationWord->value + offsetWord->value)))
			{
				Point2D * position = (Point2D *)(uint32)posWord->value;
				if ((position != NULL) && (IntervalCheck(animWord->value, 0, GetAnimationDataSize())))
				{
					Point2D targetPos(position->GetX(), position->GetY());

					const Animation * AnimationData = GetAnimationData();
					const Animation * animation = &AnimationData[(uint32)animWord->value];

					Animation * newAnim = animation->AddAnimation(source);
					if (newAnim != NULL)
					{
						newAnim->SetPosition(targetPos);
						source->SetUInt32Value(ANIM_VAR_LAST_ANIM_TIME, GetGameTime());
						return true;
					}
				}
			}
			return RETURN_CONTINUE;
		}
	}
	return false;
}*/

Float SetBounds(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * targetWord = script->GetNextWord();
		Word * rectWord = script->GetNextWord();

		Rect bounds(0, 0, 0, 0);
		Rect * rect = (Rect *)(uint32)rectWord->value;
		if (rect != NULL)
		{
			bounds.SetValues(rect->GetLeft(), rect->GetTop(), rect->GetRight(), rect->GetBottom());
		}
		else
		{
			return false;
		}

		Animation * target = (Animation *)script->GetTarget(targetWord);
		if ((target != NULL) && (target->CheckType(OBJ_TYPE_ANIMATION) == true))
		{
			target->SetBounds(bounds);
			return true;
		}
	}
	return false;
}

Float SetVariableBounds(Script * script)
{
	if (script->VerifyArguments(4) == true)
	{
		Word * targetWord = script->GetNextWord();
		Word * value = script->GetNextWord();
		Word * amount = script->GetNextWord();
		Word * size = script->GetNextWord();

		Animation * target = (Animation *)script->GetTarget(targetWord);

		if ((target != NULL) && (target->CheckType(OBJ_TYPE_ANIMATION) == true))
		{
			Float x = (int32)(fmod(value->value, amount->value)) * size->value;
			Float y = (int32)(value->value / amount->value) * size->value;

			target->SetBounds(Rect(x, y, x + size->value, y + size->value));
			return true;
		}
	}
	return false;
}

Float SetAnimationScale(Script * script)
{
	if (script->VerifyArguments(4) == true)
	{
		Word * targetWord = script->GetNextWord();
		Word * amount = script->GetNextWord();
		Word * duration = script->GetNextWord();
		Word * timeOffset = script->GetNextWord();

		Animation * target = (Animation *)script->GetTarget(targetWord);

		if ((target != NULL) && (target->CheckType(OBJ_TYPE_ANIMATION) == true))
		{
			uint32 startTime = target->GetUInt32Value(ANIM_VAR_START_TIME);
			uint32 curTime = GetGameTime();

			if ((((curTime - startTime) < (duration->value + timeOffset->value)) && ((curTime - startTime) > timeOffset->value)) || (timeOffset->value == -1))
			{
				uint32 effectTime = ZeroFit(curTime - startTime - timeOffset->value);
				Float scale = IntervalFit((effectTime % (uint32)duration->value)/duration->value, 0, 1);

				target->ModFloatValue(ANIM_VAR_SCALE, amount->value/100.0 * scale);
			}
			else if ((curTime - startTime) >= (duration->value + timeOffset->value))
			{
				target->ModFloatValue(ANIM_VAR_SCALE, amount->value/100.0);
			}
			return true;
		}
	}
	return false;
}

Float SetAnimationOpacity(Script * script)
{
	if (script->VerifyArguments(4) == true)
	{
		Word * targetWord = script->GetNextWord();
		Word * amount = script->GetNextWord();
		Word * duration = script->GetNextWord();
		Word * timeOffset = script->GetNextWord();

		Animation * target = (Animation *)script->GetTarget(targetWord);

		if ((target != NULL) && (target->CheckType(OBJ_TYPE_ANIMATION) == true))
		{
			uint32 startTime = target->GetUInt32Value(ANIM_VAR_START_TIME);
			uint32 curTime = GetGameTime();

			if ((((curTime - startTime) < (duration->value + timeOffset->value)) && ((curTime - startTime) > timeOffset->value)) || (timeOffset->value == -1))
			{
				uint32 effectTime = ZeroFit(curTime - startTime - timeOffset->value);
				Float scale = IntervalFit((effectTime % (uint32)duration->value)/duration->value, 0, 1);

				target->ModFloatValue(ANIM_VAR_OPACITY, amount->value * scale);
			}
			else if ((curTime - startTime) >= (duration->value + timeOffset->value))
			{
				target->ModFloatValue(ANIM_VAR_OPACITY, amount->value);
			}
			return true;
		}
	}
	return false;
}

Float SetAnimationRotation(Script * script)
{
	if (script->VerifyArguments(4) == true)
	{
		Word * targetWord = script->GetNextWord();
		Word * amount = script->GetNextWord();
		Word * duration = script->GetNextWord();
		Word * timeOffset = script->GetNextWord();

		Animation * target = (Animation *)script->GetTarget(targetWord);

		if ((target != NULL) && (target->CheckType(OBJ_TYPE_ANIMATION) == true))
		{
			uint32 startTime = target->GetUInt32Value(ANIM_VAR_START_TIME);
			uint32 curTime = GetGameTime();

			if ((((curTime - startTime) < (duration->value + timeOffset->value)) && ((curTime - startTime) > timeOffset->value)) || (timeOffset->value == -1))
			{
				uint32 effectTime = ZeroFit(curTime - startTime - timeOffset->value);
				Float scale = IntervalFit((effectTime % (uint32)duration->value)/duration->value, 0, 1);

				target->ModFloatValue(ANIM_VAR_ROTATION, amount->value * scale);
			}
			else if ((curTime - startTime) >= (duration->value + timeOffset->value))
			{
				target->ModFloatValue(ANIM_VAR_ROTATION, amount->value);
			}
			return true;
		}
	}
	return false;
}

Float SetScalingCenter(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * targetWord = script->GetNextWord();
		Word * posWord = script->GetNextWord();

		Point2D position(0, 0);
		Point2D * pos = (Point2D *)(uint32)posWord->value;
		if (pos != NULL)
		{
			position.SetValues(pos->GetX(), pos->GetY());
		}
		else
		{
			return false;
		}

		Animation * target = (Animation *)script->GetTarget(targetWord);
		if ((target != NULL) && (target->CheckType(OBJ_TYPE_ANIMATION) == true))
		{
			target->SetScalingCenter(position);
			return true;
		}
	}
	return false;
}

Float SetRotationCenter(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * targetWord = script->GetNextWord();
		Word * posWord = script->GetNextWord();

		Point2D position(0, 0);
		Point2D * pos = (Point2D *)(uint32)posWord->value;
		if (pos != NULL)
		{
			position.SetValues(pos->GetX(), pos->GetY());
		}
		else
		{
			return false;
		}

		Animation * target = (Animation *)script->GetTarget(targetWord);
		if ((target != NULL) && (target->CheckType(OBJ_TYPE_ANIMATION) == true))
		{
			target->SetRotationCenter(position);
			return true;
		}
	}
	return false;
}

Float SetAnimationOffset(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * targetWord = script->GetNextWord();
		Word * posWord = script->GetNextWord();

		Point2D position(0, 0);
		Point2D * pos = (Point2D *)(uint32)posWord->value;
		if (pos != NULL)
		{
			position.SetValues(pos->GetX(), pos->GetY());
		}
		else
		{
			return false;
		}

		Animation * target = (Animation *)script->GetSource();
		if ((target != NULL) && (target->CheckType(OBJ_TYPE_ANIMATION) == true))
		{
			target->SetOffset(position);
			return true;
		}
	}
	return false;
}

Float DrawAnimation(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * targetWord = script->GetNextWord();
		Word * posWord = script->GetNextWord();

		Point2D position(0, 0);
		Point2D * pos = (Point2D *)(uint32)posWord->value;
		if (pos != NULL)
		{
			position.SetValues(pos->GetX(), pos->GetY());
		}
		else
		{
			return false;
		}

		Animation * target = (Animation *)script->GetTarget(targetWord);
		if ((target != NULL) && (target->CheckType(OBJ_TYPE_ANIMATION) == true))
		{
			target->DrawAnimation(position);
			return true;
		}
	}
	return false;
}
