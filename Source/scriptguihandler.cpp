//#include <string>
//#include "type.h"
//#include "utility.h"
//#include "script.h"
//#include "scripthandler.h"
//#include "font.h"
//
//Float ActionPrintText(Script * script, uint32 index)
//{
//	if (VerifyArguments(script, index, 5) == true)
//	{
//		Point2D position, corner1, corner2;
//
//		Word * sizeWord = script->GetWord(index + 1);
//		Word * formatWord = script->GetWord(index + 2);
//		Word * posWord = script->GetWord(index + 3);
//		Point2D * pos = (Point2D *)(uint32)posWord->value;
//		if (pos != NULL)
//		{
//			position.SetValues(pos->GetX(), pos->GetY());
//		}
//		else
//		{
//			return false;
//		}
//
//		Word * cornerWord1 = script->GetWord(index + 4);
//		Point2D * c1 = (Point2D *)(uint32)cornerWord1->value;
//		if (c1 != NULL)
//		{
//			corner1.SetValues(c1->GetX(), c1->GetY());
//		}
//		else
//		{
//			return false;
//		}
//
//		Word * cornerWord2 = script->GetWord(index + 5);
//		Point2D * c2 = (Point2D *)(uint32)cornerWord2->value;
//		if (c2 != NULL)
//		{
//			corner2.SetValues(c2->GetX(), c2->GetY());
//		}
//		else
//		{
//			return false;
//		}
//
//		std::string text;
//
//		GetScriptText(script, index + 6, text);
//		/*for (uint32 i = 6;; i++)
//		{
//			Word * word = script->GetWord(index + i);
//			if ((word == NULL) || (word->value == RETURN_CONTINUE))
//			{
//				break;
//			}
//			else
//			{
//				if (word->word == NULL)
//				{
//					char * floatStr = FloatToString(word->value);
//					text.append(floatStr);
//					delete [] floatStr;
//				}
//				else if (strcmp(word->word, "_") == 0)
//				{
//					text.append(" ");
//				}
//				else
//				{
//					text.append(word->word);
//				}
//			}
//		}*/
//
//		uint8 size = IntervalFit(sizeWord->value, 8, 47);
//		FeltPen[size-8]->PrintText(text.c_str(), position, Rect(corner1, corner2), formatWord->value);
//		return true;
//	}
//	return false;
//}