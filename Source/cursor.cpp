#include "type.h"
#include "math.h"
#include "point.h"
#include "mouse.h"
#include "sprite.h"
#include "cursor.h"
//#include "game.h"


Cursor::Cursor(Mouse * mouse)
{
	source = mouse;
}

Cursor::~Cursor()
{
	Unload();
}

void Cursor::LoadSprite(const char * filename, const Point2D& hotSpot, Float offset, const char * properties)
{
	Image * image = LoadImageFromFile(filename, properties);
	sprite.CreateSprite(image->GetTexture());
	delete image;

	this->hotSpot = hotSpot;
	this->offset = offset;
}

void Cursor::SetShadow(bool state, const Point2D& lightSource)
{
	shadowFlag = state;
	this->lightSource = lightSource;
}

void Cursor::DrawCursor() const
{
	Point2D mousePos = source->GetPosition();

	Rect frame(0, 0, sprite.GetWidth(), sprite.GetHeight());

	Point2D cursorPos(mousePos.GetX() - hotSpot.GetX(), mousePos.GetY() - hotSpot.GetY());
	Point2D pivot(mousePos.GetX() - hotSpot.GetX() + ((sprite.GetWidth()/2) - hotSpot.GetX()), mousePos.GetY() - hotSpot.GetY() + ((sprite.GetHeight()/2) - hotSpot.GetY()));

	if (shadowFlag == true)
	{
		Point2D shadowPivot(mousePos.GetX() - (sprite.GetWidth()/2), mousePos.GetY() - (sprite.GetHeight()/2));
		Point2D shadowPos(mousePos.GetX() - hotSpot.GetX(), mousePos.GetY() - hotSpot.GetY());
		
		Float angle = ArcTanD(CalculateSlope(mousePos, lightSource)) + offset;
		if (mousePos.GetX() == lightSource.GetX())
		{
			angle = (mousePos.GetY() > lightSource.GetY()) ? 90.0 : -90.0;
		}
		else if (mousePos.GetX() < lightSource.GetX())
		{
			angle = 180.0 + angle;
		}

		sprite.DrawSprite(frame, shadowPos, NullPoint, shadowPivot, 1.0, angle, ColorARGB(128, 0, 0, 0));
	}
	sprite.DrawSprite(frame, cursorPos, NullPoint, pivot, 1.0, 0.0, White);
}

void Cursor::Unload()
{
	source = NULL;
	sprite.Unload();
	hotSpot = NullPoint;
}