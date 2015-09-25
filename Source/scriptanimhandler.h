#ifndef INC_SCRIPT_ANIMATION_HANDLER_H
#define INC_SCRIPT_ANIMATION_HANDLER_H

#include "animation.h"

enum AnimationKeywords
{
	ANIM_KEYWORD_BOUNDS,
	ANIM_KEYWORD_VAR_BOUNDS,
	ANIM_KEYWORD_SCALE,
	ANIM_KEYWORD_OPACITY,
	ANIM_KEYWORD_ROTATE,
	ANIM_KEYWORD_CENTER_S,
	ANIM_KEYWORD_CENTER_R,
	ANIM_KEYWORD_OFFSET,
	ANIM_KEYWORD_DRAW_ANIM,
};

Float SetBounds(Script * script);
Float SetVariableBounds(Script * script);
Float SetAnimationScale(Script * script);
Float SetAnimationOpacity(Script * script);
Float SetAnimationRotation(Script * script);
Float SetScalingCenter(Script * script);
Float SetRotationCenter(Script * script);
Float SetAnimationOffset(Script * script);
Float DrawAnimation(Script * script);

#endif