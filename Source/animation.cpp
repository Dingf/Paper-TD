#include "tinyxml.h"
#include "type.h"
#include "utility.h"
#include "point.h"
#include "object.h"
#include "rect.h"
#include "list.h"
#include "script.h"
#include "sprite.h"
#include "animation.h"
#include "game.h"

#include "mouse.h"

Animation * AnimationData = NULL;
uint32 animDataSize = 0;

List<Animation *> ActiveAnimations;
List<Animation *> RemovedAnimations;

Animation::Animation()
{
	SetState(true);
	SetType(OBJ_TYPE_ANIMATION);
	SetPosition(Point2D(512, 512));
	sprite = NULL;
	source = NULL;

	color = White;

	AllocateUInt32Values(ANIM_UINT32_VARS);
	AllocateFloatValues(ANIM_FLOAT_VARS);

	SetFloatValue(ANIM_VAR_BASE_SCALE, 1.0);
}

Animation::~Animation()
{
	SetState(false);
	source = NULL;
	script.Unload();
	onAdd.Unload();
}

Animation * Animation::AddAnimation(Object * source) const
{
	if (GetState() == true)
	{
		ActiveAnimations.Append(new Animation);

		Animation * newAnim = ActiveAnimations.GetLast();
		newAnim->Copy(*this);
		newAnim->source = source;

		const char * animScript = script.GetScript();
		newAnim->script.LoadScript(animScript, newAnim);

		const char * onAddScript = onAdd.GetScript();
		newAnim->onAdd.LoadScript(onAddScript, newAnim);

		newAnim->sprite = sprite;

		newAnim->bounds = bounds;
		newAnim->scalingCenter = scalingCenter;
		newAnim->rotationCenter = rotationCenter;
		newAnim->offset = offset;

		newAnim->color = color;

		newAnim->onAdd.ActivateScript(true);
		newAnim->SetUInt32Value(ANIM_VAR_START_TIME, GetGameTime());

		return newAnim;
	}

	return NULL;
}

void Animation::DrawAnimation(const Point2D& position)
{
	Point2D sCenter = position + scalingCenter - offset;
	Point2D rCenter = position + rotationCenter - offset;

	sprite->DrawSprite(bounds, position - offset, sCenter, rCenter, GetScale(), GetRotation(), ColorARGB(GetOpacity(), color.GetR(), color.GetG(), color.GetB()));

	SetFloatValue(ANIM_VAR_SCALE, 0);
	SetFloatValue(ANIM_VAR_OPACITY, 0);
	SetFloatValue(ANIM_VAR_ROTATION, 0);
}

void Animation::Unload()
{
	SetState(false);
	/*for (uint32 i = 0; i < ActiveAnimations.GetSize(); i++)
	{
		Animation * animation = ActiveAnimations[i];
		if ((animation == this) && (animation != NULL))
		{
			delete animation;
			ActiveAnimations.Remove(i);
			return true;
		}
	}
	return false;*/
}

bool LoadAnimationData()
{
	if (AnimationData == NULL)
	{
		FILE * file = fopen("Assets\\XML\\animation.xml", "r");

		if (file == NULL)
		{
			return false;
		}

		fseek(file, 0, SEEK_END);
		uint32 fileSize = ftell(file);
		fseek(file, 0, SEEK_SET);

		char * fileData = new char[fileSize];

		fread(fileData, fileSize, 1, file);

		fclose(file);

		TiXmlDocument AnimXML;
		AnimXML.Parse(fileData);

		TiXmlNode * node = AnimXML.FirstChild("AnimationData");
		TiXmlElement * element = node->ToElement();

		node = element->FirstChild();
		element = node->ToElement();

		TiXmlElement * lastElement = element;
		for (uint32 i = 0;; i++)
		{
			if (lastElement == NULL)
			{
				if (i > 0)
				{
					animDataSize = i;
					AnimationData = new Animation[animDataSize];
					break;
				}
				else
				{
					return false;
				}
			}
			lastElement = lastElement->NextSiblingElement();
		}

		node = element->FirstChild();
		TiXmlElement * secondaryElement = node->ToElement();

		for (uint32 i = 0; i < animDataSize; i++)
		{
			AnimationData[i].SetName(element->Attribute("Name"));

			if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
			{
				const char * text = secondaryElement->GetText();
				AnimationData[i].sprite = new Sprite;
				AnimationData[i].sprite->LoadSprite(text);
			}
			secondaryElement = secondaryElement->NextSiblingElement();

			if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
			{
				const char * text = secondaryElement->GetText();
				Float word = LookupWord(text, LOOKUP_VALUE);
				if (word == NOT_FOUND)
				{
					word = atof(text);
				}

				AnimationData[i].SetUInt32Value(ANIM_VAR_DURATION, word);
			}
			secondaryElement = secondaryElement->NextSiblingElement();

			if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
			{
				const char * text = secondaryElement->GetText();

				Script onInit;
				onInit.LoadScript(text, &AnimationData[i]);
				onInit.ActivateScript(true);
			}
			secondaryElement = secondaryElement->NextSiblingElement();

			if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
			{
				const char * text = secondaryElement->GetText();
				AnimationData[i].onAdd.LoadScript(text, &AnimationData[i]);
			}
			secondaryElement = secondaryElement->NextSiblingElement();

			if ((secondaryElement != NULL) && (secondaryElement->GetText() != NULL))
			{
				const char * text = secondaryElement->GetText();
				AnimationData[i].script.LoadScript(text, &AnimationData[i]);
			}
			secondaryElement = secondaryElement->NextSiblingElement();

			element = element->NextSiblingElement();
			if (element != NULL)
			{
				node = element->FirstChild();
				secondaryElement = node->ToElement();
			}
			else
			{
				break;
			}
		}

		AnimXML.Clear();
		Delete(fileData);

		return true;
	}
	return false;
}

const Animation * GetAnimationData()
{
	return AnimationData;
}

const Animation * GetAnimation(uint32 ID)
{
	return (ID >= animDataSize) ? NULL : &AnimationData[ID];
}

uint32 GetAnimationDataSize()
{
	return animDataSize;
}

List<Animation *> * GetActiveAnimations()
{
	return &ActiveAnimations;
}

void AnimationMaintenance()
{
	while (RemovedAnimations.GetSize() > 0)
	{
		if (RemovedAnimations[0] != NULL)
		{
			delete RemovedAnimations[0];
		}
		RemovedAnimations.Remove(0);
	}

	ActiveAnimations.ToStart();
	for (int32 i = 0; i < ActiveAnimations.GetSize(); i++)
	{
		Animation * animation = ActiveAnimations.GetCurrent();
		Object * source = animation->GetSource();
		ActiveAnimations.ToNext();

		if ((source != NULL) && (source->GetState() == false))
		{
			animation->SetSource(NULL);
		}

		if (animation != NULL)
		{
			if (animation->GetState() == false)
			{
				RemovedAnimations.Append(animation);
				ActiveAnimations.Remove(i);
				i--;
			}
			else
			{
				Object * source = animation->GetSource();

				uint32 duration = animation->GetUInt32Value(ANIM_VAR_DURATION);
				uint32 startTime = animation->GetUInt32Value(ANIM_VAR_START_TIME);

				if (((duration == 0) && (source != NULL) && (source->GetState() == true)) || ((GetGameTime() - startTime) < duration))
				{
					animation->ActivateScript();
				}
				else
				{
					animation->Unload();
				}
			}
		}
		else
		{
			ActiveAnimations.Remove(i);
			i--;
		}
	}
		
}

void ClearActiveAnimations()
{
	while (ActiveAnimations.GetSize() > 0)
	{
		if (ActiveAnimations[0] != NULL)
		{
			delete ActiveAnimations[0];
		}
		ActiveAnimations.Remove(0);
	}
	while (RemovedAnimations.GetSize() > 0)
	{
		if (RemovedAnimations[0] != NULL)
		{
			delete RemovedAnimations[0];
		}
		RemovedAnimations.Remove(0);
	}
}

void UnloadAnimationData()
{
	for (uint32 i = 0; i < animDataSize; i++)
	{
		Sprite * sprite = AnimationData[i].GetSprite();
		if (sprite != NULL)
		{
			sprite->Unload();
			delete sprite;
		}
	}
	delete [] AnimationData;
	animDataSize = 0;
}