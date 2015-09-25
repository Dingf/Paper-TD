#ifndef INC_MOUSE_H
#define INC_MOUSE_H

#include "type.h"
#include "point.h"
#include "mousedef.h"

#define DOUBLE_CLICK_INTERVAL 250
#define DOUBLE_CLICK_RADIUS 2

//enum MouseModes
//{
//	MOUSE_MODE_NORMAL,
//	//MOUSE_MODE_ATTACK,
//};

struct Mouse
{
	public:
		Mouse();
		~Mouse();

		bool GetButtonState(uint8 button) const;
		const Point2D& GetPosition() const { return position; }
		Float GetX() const { return position.GetX(); }
		Float GetY() const { return position.GetY(); }

		void SetButtonState(uint8 button, bool state);
		void SetPosition(int32 x, int32 y) { position.SetValues(x, y); }
		void SetPosition(const Point2D& pos) { position = pos; }
		void UpdateLastClickPos() { lastClickPos = position; }

		void ClearClickTime(uint8 button);
		void UpdateClickTime(uint8 button);
		bool CheckDoubleClick(uint8 button) const;

		void Unload();
	private:
		bool buttons[MAX_MOUSE_BUTTONS];
		uint32 lastClickTime[MAX_MOUSE_BUTTONS];
		Point2D position, lastClickPos;
};

extern Mouse WorldMouse;

void HandleMouseEvent(int32 button, int32 state, int32 x, int32 y);
void HandleMouseMotion(int32 x, int32 y);
void HandlePassiveMouseMotion(int32 x, int32 y);
//
//void SetPosition(int32 x, int32 y);
//void SetPosition(Point2D pos);
//Point2D GetPosition();
//Float GetMouseX();
//Float GetMouseY();
//
//void SetButtonState(uint8 button, bool state);
//bool GetButtonState(uint8 button);

#endif