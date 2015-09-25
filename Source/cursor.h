#ifndef INC_CURSOR_H
#define INC_CURSOR_H

#include "type.h"
#include "point.h"
#include "mouse.h"
#include "sprite.h"

struct Cursor
{
	public:
		Cursor(Mouse * mouse);
		~Cursor();

		Mouse * GetSource() const { return source; }

		void LoadSprite(const char * filename, const Point2D& hotSpot, Float offset, const char * properties = NULL);
		void SetShadow(bool state, const Point2D& lightSource);
		void DrawCursor() const;

		void Unload();
	private:
		bool shadowFlag;
		Float offset;
		Point2D lightSource;
		Point2D hotSpot;
		Sprite sprite;
		Mouse * source;
};

#endif