#include "type.h"
#include "math.h"
#include "utility.h"
#include "script.h"
#include "scriptdef.h"

bool VerifyWord(Word * word)
{
	return ((word != NULL) && ((word->value != KEYWORD_END) || (word->type != ACTION)));
}

Float OperatorEquals(Script * script)
{
	Word * first = script->PeekWord(-1);
	Word * second = script->PeekWord(1);

	if ((VerifyWord(first) == true) && (VerifyWord(second) == true))
	{
		uint32 firstValue = (first->type == POINTER) ? (uint32)script->GetTarget(first) : first->value;
		uint32 secondValue = (second->type == POINTER) ? (uint32)script->GetTarget(second) : second->value;

		script->GetNextOriginalWord();
		return (firstValue == secondValue);
	}
	return false;
}

Float OperatorNotEquals(Script * script)
{
	Word * first = script->PeekWord(-1);
	Word * second = script->PeekWord(1);

	if ((VerifyWord(first) == true) && (VerifyWord(second) == true))
	{
		uint32 firstValue = (first->type == POINTER) ? (uint32)script->GetTarget(first) : first->value;
		uint32 secondValue = (second->type == POINTER) ? (uint32)script->GetTarget(second) : second->value;
		
		script->GetNextOriginalWord();
		return (firstValue != secondValue);
	}
	return false;
}

Float OperatorLessThan(Script * script)
{
	Word * first = script->PeekWord(-1);
	Word * second = script->PeekWord(1);

	if ((VerifyWord(first) == true) && (VerifyWord(second) == true))
	{
		script->GetNextOriginalWord();
		return first->value < second->value;
	}
	return false;
}

Float OperatorLessOrEqual(Script * script)
{
	Word * first = script->PeekWord(-1);
	Word * second = script->PeekWord(1);

	if ((VerifyWord(first) == true) && (VerifyWord(second) == true))
	{
		script->GetNextOriginalWord();
		return (first->value <= second->value);
	}
	return false;
}

Float OperatorGreaterThan(Script * script)
{
	Word * first = script->PeekWord(-1);
	Word * second = script->PeekWord(1);

	if ((VerifyWord(first) == true) && (VerifyWord(second) == true))
	{
		script->GetNextOriginalWord();
		return (first->value > second->value);
	}
	return false;
}

Float OperatorGreaterOrEqual(Script * script)
{
	Word * first = script->PeekWord(-1);
	Word * second = script->PeekWord(1);

	if ((VerifyWord(first) == true) && (VerifyWord(second) == true))
	{
		script->GetNextOriginalWord();
		return (first->value >= second->value);
	}
	return false;
}

Float OperatorAdd(Script * script)
{
	Word * first = script->PeekWord(-1);
	Word * second = script->PeekWord(1);

	if ((VerifyWord(first) == true) && (VerifyWord(second) == true))
	{
		script->GetNextOriginalWord();
		return first->value + second->value + FLOAT_EPSILON;
	}
	return false;
}

Float OperatorSubtract(Script * script)
{
	Word * first = script->PeekWord(-1);
	Word * second = script->PeekWord(1);

	if ((VerifyWord(first) == true) && (VerifyWord(second) == true))
	{
		script->GetNextOriginalWord();
		return first->value - second->value + FLOAT_EPSILON;
	}
	return false;
}

Float OperatorMultiply(Script * script)
{
	Word * first = script->PeekWord(-1);
	Word * second = script->PeekWord(1);

	if ((VerifyWord(first) == true) && (VerifyWord(second) == true))
	{
		script->GetNextOriginalWord();
		return (first->value * second->value) + FLOAT_EPSILON;
	}
	return false;
}

Float OperatorDivide(Script * script)
{
	Word * first = script->PeekWord(-1);
	Word * second = script->PeekWord(1);

	if ((VerifyWord(first) == true) && (VerifyWord(second) == true))
	{
		if (second->value == 0)
		{
			return false;
		}
		script->GetNextOriginalWord();
		return (first->value/second->value) + FLOAT_EPSILON;
	}
	return false;
}

Float OperatorIntDivide(Script * script)
{
	Word * first = script->PeekWord(-1);
	Word * second = script->PeekWord(1);

	if ((VerifyWord(first) == true) && (VerifyWord(second) == true))
	{
		if (((int32)second->value) == 0)
		{
			return false;
		}
		script->GetNextOriginalWord();
		return (int32)((int32)first->value/(int32)second->value) + FLOAT_EPSILON;
	}
	return false;
}

Float OperatorModulo(Script * script)
{
	Word * first = script->PeekWord(-1);
	Word * second = script->PeekWord(1);

	if ((VerifyWord(first) == true) && (VerifyWord(second) == true))
	{
		if (second->value == 0)
		{
			return false;
		}
		script->GetNextOriginalWord();
		return fmod(first->value, second->value) + FLOAT_EPSILON;
	}
	return false;
}

Float OperatorTruncate(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * valWord = script->GetNextWord();
		return (int32)valWord->value + FLOAT_EPSILON;
	}
	return false;
}

Float OperatorRound(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * valWord = script->GetNextWord();
		Word * precision = script->GetNextWord();

		return Smooth(valWord->value, ExpN(10, -precision->value)) + FLOAT_EPSILON;
	}
	return false;
}

Float OperatorCeiling(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * valWord = script->GetNextWord();
		return Ceiling(valWord->value) + FLOAT_EPSILON;
	}
	return false;
}

Float OperatorFloor(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * valWord = script->GetNextWord();
		return Floor(valWord->value) + FLOAT_EPSILON;
	}
	return false;
}

Float OperatorAssign(Script * script)
{
	Word * first = script->PeekWord(-1);
	Word * second = script->PeekWord(1);

	if ((VerifyWord(first) == true) && (VerifyWord(second) == true) && (first->type == VARIABLE) && ((second->type == NUMERIC) || (second->type == VARIABLE)))
	{
		script->GetNextOriginalWord();
		script->SetVariable(first->word, second->value);
		return true;
	}
	return false;
}

Float OperatorLogicalAnd(Script * script)
{
	Word * first = script->PeekWord(-1);
	Word * second = script->PeekWord(1);

	if ((VerifyWord(first) == true) && (VerifyWord(second) == true))
	{
		script->GetNextOriginalWord();
		return ((bool)first->value && (bool)second->value);
	}
	return false;
}

Float OperatorLogicalOr(Script * script)
{
	Word * first = script->PeekWord(-1);
	Word * second = script->PeekWord(1);

	if ((VerifyWord(first) == true) && (VerifyWord(second) == true))
	{
		script->GetNextOriginalWord();
		return ((bool)first->value || (bool)second->value);
	}
	return false;
}

Float OperatorLogicalNot(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * first = script->GetNextWord();
		return !((bool)first->value);
	}
	return false;
}

Float OperatorLogicalXOr(Script * script)
{
	Word * first = script->PeekWord(-1);
	Word * second = script->PeekWord(1);

	if ((VerifyWord(first) == true) && (VerifyWord(second) == true))
	{
		script->GetNextOriginalWord();
		return (((bool)first->value || (bool)second->value) && !((bool)first->value && (bool)second->value));
	}
	return false;
}

Float OperatorSineR(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * value = script->GetNextWord();
		return SinR(value->value);
	}
	return false;
}

Float OperatorSineD(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * value = script->GetNextWord();
		return SinD(value->value);
	}
	return false;
}

Float OperatorCosineR(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * value = script->GetNextWord();
		return CosR(value->value);
	}
	return false;
}

Float OperatorCosineD(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * value = script->GetNextWord();
		return CosD(value->value);
	}
	return false;
}

Float OperatorTangentR(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * value = script->GetNextWord();
		return TanR(value->value);
	}
	return false;
}

Float OperatorTangentD(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * value = script->GetNextWord();
		return TanD(value->value);
	}
	return false;
}

Float OperatorArcSineR(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * value = script->GetNextWord();
		return ArcSinR(value->value);
	}
	return false;
}

Float OperatorArcSineD(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * value = script->GetNextWord();
		return ArcSinD(value->value);
	}
	return false;
}

Float OperatorArcCosineR(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * value = script->GetNextWord();
		return ArcCosR(value->value);
	}
	return false;
}

Float OperatorArcCosineD(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * value = script->GetNextWord();
		return ArcCosD(value->value);
	}
	return false;
}

Float OperatorArcTangentR(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * value = script->GetNextWord();
		return ArcTanR(value->value);
	}
	return false;
}

Float OperatorArcTangentD(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * value = script->GetNextWord();
		return ArcTanD(value->value);
	}
	return false;
}

Float OperatorLog2(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * value = script->GetNextWord();

		if (value->value < 0)
		{
			return false;
		}
		return Log2(value->value);
	}
	return false;
}

Float OperatorLogN(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * base = script->GetNextWord();
		Word * value = script->GetNextWord();

		if (value->value < 0)
		{
			return false;
		}
		return LogN(base->value, value->value);
	}
	return false;
}

Float OperatorLogE(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * value = script->GetNextWord();

		if (value->value < 0)
		{
			return false;
		}
		return Ln(value->value);
	}
	return false;
}

Float OperatorLog10(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * value = script->GetNextWord();

		if (value->value < 0)
		{
			return false;
		}
		return Log10(value->value);
	}
	return false;
}

Float OperatorExp2(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * value = script->GetNextWord();
		return Exp2(value->value);
	}
	return false;
}

Float OperatorExpN(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * base = script->GetNextWord();
		Word * value = script->GetNextWord();
		return ExpN(base->value, value->value);
	}
	return false;
}

Float OperatorExpE(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * value = script->GetNextWord();
		return ExpE(value->value);
	}
	return false;
}

Float OperatorExp10(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * value = script->GetNextWord();
		return Exp10(value->value);
	}
	return false;
}

Float OperatorRoot2(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * value = script->GetNextWord();
		return Root2(value->value);
	}
	return false;
}

Float OperatorRoot3(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * value = script->GetNextWord();
		return Root3(value->value);
	}
	return false;
}

Float OperatorMinFit(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * value = script->GetNextWord();
		Word * min = script->GetNextWord();

		return MinFit(value->value, min->value);
	}
	return false;
}

Float OperatorMaxFit(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Word * value = script->GetNextWord();
		Word * max = script->GetNextWord();

		return MaxFit(value->value, max->value);
	}
	return false;
}

Float OperatorIntervalFit(Script * script)
{
	if (script->VerifyArguments(3) == true)
	{
		Word * value = script->GetNextWord();
		Word * min = script->GetNextWord();
		Word * max = script->GetNextWord();

		return IntervalFit(value->value, min->value, max->value);
	}
	return false;
}

Float OperatorAbsVal(Script * script)
{
	if (script->VerifyArguments(1) == true)
	{
		Word * value = script->GetNextWord();
		return AbsVal(value->value);
	}
	return false;
}

Float OperatorGetZeroAngleD(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Point2D position1, position2;

		Word * posWord1 = script->GetNextWord();
		Point2D * pos1 = (Point2D *)(uint32)posWord1->value;
		if (pos1 != NULL)
		{
			position1.SetValues(pos1->GetX(), pos1->GetY());
		}
		else
		{
			return false;
		}

		Word * posWord2 = script->GetNextWord();
		Point2D * pos2 = (Point2D *)(uint32)posWord2->value;
		if (pos2 != NULL)
		{
			position2.SetValues(pos2->GetX(), pos2->GetY());
		}
		else
		{
			return false;
		}

		Vector3D vector(Point2D(position1.GetX(), position1.GetY()),
						Point2D(position2.GetX(), position2.GetY()));

		return vector.GetZeroAngleD() + FLOAT_EPSILON;
	}
	return false;
}

Float OperatorGetZeroAngleR(Script * script)
{
	if (script->VerifyArguments(2) == true)
	{
		Point2D position1, position2;

		Word * posWord1 = script->GetNextWord();
		Point2D * pos1 = (Point2D *)(uint32)posWord1->value;
		if (pos1 != NULL)
		{
			position1.SetValues(pos1->GetX(), pos1->GetY());
		}
		else
		{
			return false;
		}

		Word * posWord2 = script->GetNextWord();
		Point2D * pos2 = (Point2D *)(uint32)posWord2->value;
		if (pos2 != NULL)
		{
			position2.SetValues(pos2->GetX(), pos2->GetY());
		}
		else
		{
			return false;
		}

		Vector3D vector(Point2D(position1.GetX(), position1.GetY()),
						Point2D(position2.GetX(), position2.GetY()));

		return vector.GetZeroAngleR() + FLOAT_EPSILON;
	}
	return false;
}

