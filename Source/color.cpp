#include "type.h"
#include "utility.h"
#include "color.h"

ColorARGB Black(255, 0, 0, 0);
ColorARGB White(255, 255, 255, 255);
ColorARGB Grey50(255, 128, 128, 128);
ColorARGB Grey25(255, 64, 64, 64);
ColorARGB Grey75(255, 192, 192, 192);

ColorARGB Red(255, 255, 0, 0);
ColorARGB Green(255, 0, 255, 0);
ColorARGB Blue(255, 0, 0, 255);
ColorARGB Yellow(255, 255, 255, 0);
ColorARGB Violet(255, 255, 0, 255);
ColorARGB Teal(255, 0, 255, 255);

ColorARGB::ColorARGB()
{
	SetValues(0, 0, 0, 0);
}

ColorARGB::ColorARGB(uint8 a, uint8 r, uint8 g, uint8 b)
{
	SetValues(a, r, g, b);
}

ColorARGB::ColorARGB(uint32 values)
{
	SetValues(values);
}


bool ColorARGB::operator == (const ColorARGB& c) const
{
	return ((alpha == c.alpha) && (red == c.red) && (green == c.green) && (blue == c.blue));
}

bool ColorARGB::operator != (const ColorARGB& c) const
{
	return !(*this == c);
}

ColorARGB ColorARGB::operator ! () const
{
	return ColorARGB(255 - alpha, 255 - red, 255 - green, 255 - blue);
}

ColorARGB ColorARGB::operator + (const ColorARGB& c) const
{
	return ColorARGB(alpha + c.alpha, red + c.red, green + c.green, blue + c.blue);
}

ColorARGB ColorARGB::operator - (const ColorARGB& c) const
{
	return ColorARGB(ZeroFit(alpha - c.alpha), ZeroFit(red - c.red), ZeroFit(green - c.green), ZeroFit(blue - c.blue));
}

ColorARGB& ColorARGB::operator = (const ColorARGB& c)
{
	SetValues(c.alpha, c.red, c.green, c.blue);
	return *this;
}

ColorARGB& ColorARGB::operator += (const ColorARGB& c)
{
	SetValues(alpha + c.alpha, red + c.red, green + c.green, blue + c.blue);
	return *this;
}

ColorARGB& ColorARGB::operator -= (const ColorARGB& c)
{
	SetValues(ZeroFit(alpha - c.alpha), ZeroFit(red - c.red), ZeroFit(green - c.green), ZeroFit(blue - c.blue));
	return *this;
}

void ColorARGB::SetValues(uint8 a, uint8 r, uint8 g, uint8 b)
{
	alpha = a;
	red = r;
	green = g;
	blue = b;
} 

void ColorARGB::SetValues(uint32 values)
{
	alpha = (values & 0xFF000000) >> 24;
	red = (values & 0x00FF0000) >> 16;
	green = (values & 0x0000FF00) >> 8;
	blue = (values & 0x000000FF);
}

void ColorARGB::GetValues(uint8 &a, uint8 &r, uint8& g, uint8& b) const
{
	a = alpha;
	r = red;
	g = green;
	b = blue;
}

ColorARGB ColorRGB(uint8 r, uint8 g, uint8 b)
{
	return ColorARGB(255, r, g, b);
}

ColorARGB ColorGrey(uint8 lightness)
{
	return ColorARGB(255, lightness, lightness, lightness);
}