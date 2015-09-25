//#include "type.h"
//#include "utility.h"
//#include "script.h"
//#include "scripthandler.h"
//#include "particle.h"
//#include "emitter.h"
//
//Float ActionAddParticleEmitter(Script * script, uint32 index)
//{
//	if (VerifyArguments(script, index, 3) == true)
//	{
//		Word * particleWord = script->GetWord(index + 1);
//		Word * posWord = script->GetWord(index + 2);
//		Word * sourceWord = script->GetWord(index + 3);
//
//		Object * source = script->GetTarget(sourceWord);
//		Point2D * position = (Point2D *)(uint32)posWord->value;
//
//		if ((position != NULL) && (IntervalCheck(particleWord->value, 0, GetEmitterDataSize()) == true))
//		{
//			Point2D targetPos(position->GetX(), position->GetY());
//
//			const Emitter * EmitterData = GetEmitterData();
//			const Emitter * emitter = &EmitterData[(uint32)particleWord->value];
//
//			if ((source != NULL) && (emitter->AddEmitter(source, targetPos) != NULL))
//			{
//				return true;
//			}
//		}
//	}
//	return false;
//}