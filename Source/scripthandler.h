#ifndef INC_SCRIPT_HANDLER_H
#define INC_SCRIPT_HANDLER_H
/*
#include <string>
#include "type.h"
#include "script.h"
#include "scripthandlerdef.h"

bool VerifyArguments(Script * script, uint32 index, uint32 amount);
void GetScriptText(Script * script, uint32 index, std::string& string);

Float TakeNoAction(Script * script, uint32 index);
Float InvalidHandler(Script * script, uint32 index);
Float EndCommand(Script * script, uint32 index);
Float EvaluateIf(Script * script, uint32 index);
Float ForLoop(Script * script, uint32 index);
Float StopScript(Script * script, uint32 index);

Float ActionMakePoint2D(Script * script, uint32 index);
Float ActionGenerateRand(Script * script, uint32 index);
Float ActionSet(Script * script, uint32 index);
Float ActionMod(Script * script, uint32 index);
Float ActionFlag(Script * script, uint32 index);
Float ActionNewVariable(Script * script, uint32 index);
Float ActionGet(Script * script, uint32 index);
Float ActionSelect(Script * script, uint32 index);
Float ActionDelete(Script * script, uint32 index);
Float ActionRefresh(Script * script, uint32 index);
Float ActionForAllLoop(Script * script, uint32 index);
Float ActionPeriodicScript(Script * script, uint32 index);
Float ActionTrigger(Script * script, uint32 index);
Float ActionCustomScript(Script * script, uint32 index);

Float OperatorEquals(Script * script, uint32 index);
Float OperatorNotEquals(Script * script, uint32 index);
Float OperatorLessThan(Script * script, uint32 index);
Float OperatorLessOrEqual(Script * script, uint32 index);
Float OperatorGreaterThan(Script * script, uint32 index);
Float OperatorGreaterOrEqual(Script * script, uint32 index);
Float OperatorAdd(Script * script, uint32 index);
Float OperatorSubtract(Script * script, uint32 index);
Float OperatorMultiply(Script * script, uint32 index);
Float OperatorDivide(Script * script, uint32 index);
Float OperatorIntDivide(Script * script, uint32 index);
Float OperatorModulo(Script * script, uint32 index);
Float OperatorTruncate(Script * script, uint32 index);
Float OperatorRound(Script * script, uint32 index);
Float OperatorCeiling(Script * script, uint32 index);
Float OperatorFloor(Script * script, uint32 index);
Float OperatorAssign(Script * script, uint32 index);
Float OperatorLogicalAnd(Script * script, uint32 index);
Float OperatorLogicalOr(Script * script, uint32 index);
Float OperatorLogicalNot(Script * script, uint32 index);
Float OperatorLogicalXOr(Script * script, uint32 index);

Float ActionAnimate(Script * script, uint32 index);
Float ActionSetBounds(Script * script, uint32 index);
Float ActionSetVarBounds(Script * script, uint32 index);
Float ActionSetScale(Script * script, uint32 index);
Float ActionSetOpacity(Script * script, uint32 index);
Float ActionSetRotation(Script * script, uint32 index);
Float ActionSetScalingCenter(Script * script, uint32 index);
Float ActionSetRotationCenter(Script * script, uint32 index);
Float ActionSetOffset(Script * script, uint32 index);
Float ActionDrawAnimation(Script * script, uint32 index);
Float ActionPrintText(Script * script, uint32 index);

Float ActionAddParticleEmitter(Script * script, uint32 index);

Float ActionTriggerAmount(Script * script, uint32 index);

Float ActionLearnSpell(Script * script, uint32 index);
Float ActionForgetSpell(Script * script, uint32 index);
Float ActionApplySpell(Script * script, uint32 index);
Float ActionRemoveSpell(Script * script, uint32 index);
Float ActionActiveSpell(Script * script, uint32 index);
Float ActionKnowsSpell(Script * script, uint32 index);
Float ActionShowSpell(Script * script, uint32 index);
Float ActionCastSpell(Script * script, uint32 index);
Float ActionCheckCast(Script * script, uint32 index);
Float ActionCheckSpellCooldown(Script * script, uint32 index);
Float ActionCheckSpellCharges(Script * script, uint32 index);
Float ActionApplyAura(Script * script, uint32 index);

Float ActionTowerFire(Script * script, uint32 index);
Float ActionBuildTower(Script * script, uint32 index);
Float ActionSetProjectile(Script * script, uint32 index);
Float ActionResetAttack(Script * script, uint32 index);
Float ActionSetTowerWait(Script * script, uint32 index);
Float ActionAddTowerKill(Script * script, uint32 index);

Float ActionAddGameObject(Script * script, uint32 index);
Float ActionRemoveGameObject(Script * script, uint32 index);
Float ActionRotateGameObject(Script * script, uint32 index);
Float ActionSetGameObjectValues(Script * script, uint32 index);
Float ActionDamageArea(Script * script, uint32 index);
Float ActionSpellArea(Script * script, uint32 index);
//Float ActionConeDirection(Script * script, uint32 index);

Float ActionDamageCreep(Script * script, uint32 index);
Float ActionGetCreepDirectionD(Script * script, uint32 index);
Float ActionGetCreepDirectionR(Script * script, uint32 index);

Float TriggerOnHit(Script * script, uint32 index);
Float TriggerOnExplode(Script * script, uint32 index);
Float TriggerOnFire(Script * script, uint32 index);
Float TriggerOnUpgrade(Script * script, uint32 index);
Float TriggerOnSell(Script * script, uint32 index);
Float TriggerOnKill(Script * script, uint32 index);
Float TriggerOnDeath(Script * script, uint32 index);
Float TriggerOnClick(Script * script, uint32 index);

Float OperatorSineR(Script * script, uint32 index);
Float OperatorSineD(Script * script, uint32 index);
Float OperatorCosineR(Script * script, uint32 index);
Float OperatorCosineD(Script * script, uint32 index);
Float OperatorTangentR(Script * script, uint32 index);
Float OperatorTangentD(Script * script, uint32 index);
Float OperatorArcSineR(Script * script, uint32 index);
Float OperatorArcSineD(Script * script, uint32 index);
Float OperatorArcCosineR(Script * script, uint32 index);
Float OperatorArcCosineD(Script * script, uint32 index);
Float OperatorArcTangentR(Script * script, uint32 index);
Float OperatorArcTangentD(Script * script, uint32 index);
Float OperatorLog2(Script * script, uint32 index);
Float OperatorLogN(Script * script, uint32 index);
Float OperatorLogE(Script * script, uint32 index);
Float OperatorLog10(Script * script, uint32 index);
Float OperatorExp2(Script * script, uint32 index);
Float OperatorExpN(Script * script, uint32 index);
Float OperatorExpE(Script * script, uint32 index);
Float OperatorExp10(Script * script, uint32 index);
Float OperatorRoot2(Script * script, uint32 index);
Float OperatorRoot3(Script * script, uint32 index);
Float OperatorMinFit(Script * script, uint32 index);
Float OperatorMaxFit(Script * script, uint32 index);
Float OperatorIntervalFit(Script * script, uint32 index);
Float OperatorAbsVal(Script * script, uint32 index);
Float ActionGetZeroAngleD(Script * script, uint32 index);
Float ActionGetZeroAngleR(Script * script, uint32 index);

void DeleteCastHelper();*/

#endif