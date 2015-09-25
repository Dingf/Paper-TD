#include "opengl.h"
#include "type.h"
#include "utility.h"
#include "keyboard.h"
#include "mouse.h"
#include "mousehandler.h"
#include "game.h"

#include "emitter.h"
#include "animation.h"

Mouse WorldMouse;

Mouse::Mouse()
{
	position = NullPoint;
}

Mouse::~Mouse()
{
	Unload();
}

bool Mouse::GetButtonState(uint8 button) const
{
	if (button < MAX_MOUSE_BUTTONS)
	{
		return buttons[button];
	}
	return UP;
}


void Mouse::SetButtonState(uint8 button, bool state)
{
	if (button < MAX_MOUSE_BUTTONS)
	{
		buttons[button] = state;
	}
}

void Mouse::ClearClickTime(uint8 button)
{
	if (button < MAX_MOUSE_BUTTONS)
	{
		lastClickTime[button] = 0;
	}
}

void Mouse::UpdateClickTime(uint8 button)
{
	if (button < MAX_MOUSE_BUTTONS)
	{
		lastClickTime[button] = GetGameTime();
	}
}

bool Mouse::CheckDoubleClick(uint8 button) const
{
	return (((GetGameTime() - lastClickTime[button]) <= DOUBLE_CLICK_INTERVAL) && (CalculateDistance(position, lastClickPos) <= DOUBLE_CLICK_RADIUS));
}

void Mouse::Unload()
{
	position = NullPoint;
	for (uint8 i = 0; i < MAX_MOUSE_BUTTONS; i++)
	{
		SetButtonState(i, false);
	}
}

void HandleMouseEvent(int32 button, int32 state, int32 x, int32 y)
{
	/*if (state == DOWN)
	{
	const Emitter * EmitterData = GetEmitterData();
	EmitterData[27].AddEmitter(&WorldGame, Point2D(x, y));
	}*/

	/*const Animation * AnimationData = GetAnimationData();
	AnimationData[45].AddAnimation(&WorldGame);

	List<Animation *> * ActiveAnimations = GetActiveAnimations();
	Animation * lastAnim = ActiveAnimations->GetData(ActiveAnimations->GetSize() - 1);

	lastAnim->SetPosition(x, y);*/



	WorldMouse.SetButtonState(button, state);

	for (uint32 i = 0; i < MOUSE_HANDLER_AMOUNT; i++)
	{
		if ((MouseHandlerTable[i].hotSpot.ContainsPoint(Point2D(x, y)) == true) && (CheckFlag(MouseHandlerTable[i].button, (1 << button)) == true) && (state == MouseHandlerTable[i].state))
		{
			if (CheckModifierFlag(MouseHandlerTable[i].modFlags) == true)
			{
				if (MouseHandlerTable[i].handler(button, Point2D(x, y)) == true)
				{
					break;
				}
			}
		}
	}

	if (state == DOWN)
	{
		if (WorldMouse.CheckDoubleClick(button) == true)
		{
			WorldMouse.ClearClickTime(button);
		}
		else
		{
			WorldMouse.UpdateClickTime(button);
		}
	}

	WorldMouse.UpdateLastClickPos();
}

void HandleMouseMotion(int32 x, int32 y)
{
	WorldMouse.SetPosition(x, y);
	/*if (GetButtonState(RIGHT) == DOWN)
	{
		Vector3D * angle = &GlobalMouse.lastAngle;
		Point2D * position = &GlobalMouse.position;

		Float dx = angle->GetX() + ((x - position->GetX()) * 0.001);
		Float dy = angle->GetY() + ((y - position->GetY()) * 0.001);

		dy = IntervalFit(dy, -1.5707, 1.5707);	//(-pi/2, pi/2) interval. Faster to just type decimal value than divide...

		OrientCamera(CosR(dy) * SinR(dx), -SinR(dy), -CosR(dy) * CosR(dx));
		GlobalMouse.angle.SetValues(dx, dy, 0);
	}*/
}

void HandlePassiveMouseMotion(int32 x, int32 y)
{
	WorldMouse.SetPosition(x, y);
}
