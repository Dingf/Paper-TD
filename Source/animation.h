#ifndef INC_ANIMATION_H
#define INC_ANIMATION_H

#include "type.h"
#include "point.h"
#include "object.h"
#include "rect.h"
#include "list.h"
#include "script.h"
#include "color.h"
#include "sprite.h"

enum AnimUInt32Variables
{
	ANIM_VAR_DURATION,
	ANIM_VAR_START_TIME,
	ANIM_VAR_PERIODIC_TIME,
	ANIM_UINT32_VARS
};

enum AnimFloatVariables		
{
	ANIM_VAR_BASE_SCALE,
	ANIM_VAR_BASE_OPACITY,
	ANIM_VAR_BASE_ROTATION,
	ANIM_VAR_SCALE,
	ANIM_VAR_OPACITY,
	ANIM_VAR_ROTATION,
	ANIM_FLOAT_VARS
};

struct Animation : public Object
{
	public:
		Animation();
		~Animation();

		Float GetScale() const { return GetFloatValue(ANIM_VAR_BASE_SCALE) + GetFloatValue(ANIM_VAR_SCALE); }
		Float GetOpacity() const { return color.GetA() + GetFloatValue(ANIM_VAR_BASE_OPACITY) + GetFloatValue(ANIM_VAR_OPACITY); }
		Float GetRotation() const { return GetFloatValue(ANIM_VAR_BASE_ROTATION) + GetFloatValue(ANIM_VAR_ROTATION); }

		      Rect& GetBounds()       { return bounds; }
		const Rect& GetBounds() const { return bounds; }

		      Point2D& GetScalingCenter()       { return scalingCenter; }
		const Point2D& GetScalingCenter() const { return scalingCenter; }
		      Point2D& GetRotationCenter()       { return rotationCenter; }
		const Point2D& GetRotationCenter() const { return rotationCenter; }

		      Sprite * GetSprite()       { return sprite; }
		const Sprite * GetSprite() const { return sprite; }

			  Object * GetSource()		 { return source; }
		const Object * GetSource() const { return source; }

		void SetSource(Object * source) { this->source = source; }

		void SetColor(const ColorARGB& color) { this->color = color; }

		void SetBounds(const Rect& bounds) { this->bounds = bounds; }
		void SetScalingCenter(const Point2D& center) { scalingCenter = center; }
		void SetRotationCenter(const Point2D& center) { rotationCenter = center; }
		void SetOffset(const Point2D& offset) { this->offset = offset; }

		Animation * AddAnimation(Object * source) const;

		bool ActivateScript() { script.SetSource(this); return script.ActivateScript(true); }
		void DrawAnimation(const Point2D& position);

		void Unload();

		friend bool LoadAnimationData();
	private:
		Rect bounds;
		Point2D scalingCenter, rotationCenter, offset;
		Script script, onAdd;
		ColorARGB color;
		Sprite * sprite;
		Object * source;
};

bool LoadAnimationData();
void UnloadAnimationData();

const Animation * GetAnimationData();
const Animation * GetAnimation(uint32 ID);
uint32 GetAnimationDataSize();

List<Animation *> * GetActiveAnimations();
void AnimationMaintenance();
void ClearActiveAnimations();

extern Animation * AnimationData;

#endif