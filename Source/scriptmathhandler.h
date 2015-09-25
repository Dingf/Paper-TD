#ifndef INC_SCRIPT_MATH_HANDLER_H
#define INC_SCRIPT_MATH_HANDLER_H

#include "type.h"

Float OperatorEquals(Script * script);
Float OperatorNotEquals(Script * script);
Float OperatorLessThan(Script * script);
Float OperatorLessOrEqual(Script * script);
Float OperatorGreaterThan(Script * script);
Float OperatorGreaterOrEqual(Script * script);
Float OperatorAdd(Script * script);
Float OperatorSubtract(Script * script);
Float OperatorMultiply(Script * script);
Float OperatorDivide(Script * script);
Float OperatorIntDivide(Script * script);
Float OperatorModulo(Script * script);
Float OperatorTruncate(Script * script);
Float OperatorRound(Script * script);
Float OperatorCeiling(Script * script);
Float OperatorFloor(Script * script);
Float OperatorAssign(Script * script);
Float OperatorLogicalAnd(Script * script);
Float OperatorLogicalOr(Script * script);
Float OperatorLogicalNot(Script * script);
Float OperatorLogicalXOr(Script * script);
Float OperatorSineR(Script * script);
Float OperatorSineD(Script * script);
Float OperatorCosineR(Script * script);
Float OperatorCosineD(Script * script);
Float OperatorTangentR(Script * script);
Float OperatorTangentD(Script * script);
Float OperatorArcSineR(Script * script);
Float OperatorArcSineD(Script * script);
Float OperatorArcCosineR(Script * script);
Float OperatorArcCosineD(Script * script);
Float OperatorArcTangentR(Script * script);
Float OperatorArcTangentD(Script * script);
Float OperatorLog2(Script * script);
Float OperatorLogN(Script * script);
Float OperatorLogE(Script * script);
Float OperatorLog10(Script * script);
Float OperatorExp2(Script * script);
Float OperatorExpN(Script * script);
Float OperatorExpE(Script * script);
Float OperatorExp10(Script * script);
Float OperatorRoot2(Script * script);
Float OperatorRoot3(Script * script);
Float OperatorMinFit(Script * script);
Float OperatorMaxFit(Script * script);
Float OperatorIntervalFit(Script * script);
Float OperatorAbsVal(Script * script);
Float OperatorGetZeroAngleD(Script * script);
Float OperatorGetZeroAngleR(Script * script);

#endif