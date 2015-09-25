#ifndef INC_COLOR_H
#define INC_COLOR_H

#include "type.h"

struct ColorARGB
{
	public:
		ColorARGB();
		ColorARGB(uint8 a, uint8 r, uint8 g, uint8 b);
		ColorARGB(uint32 values);

		bool operator == (const ColorARGB& c) const;
		bool operator != (const ColorARGB& c) const;

		ColorARGB operator ! () const;

		ColorARGB operator + (const ColorARGB& c) const;
		ColorARGB operator - (const ColorARGB& c) const;

		ColorARGB& operator = (const ColorARGB& c);
		ColorARGB& operator += (const ColorARGB& c);
		ColorARGB& operator -= (const ColorARGB& c);

		void SetAlpha(uint8 alpha) { this->alpha = alpha; }
		void SetValues(uint8 a, uint8 r, uint8 g, uint8 b);
		void SetValues(uint32 values);
		void GetValues(uint8 &a, uint8 &r, uint8& g, uint8& b) const;

		const uint8 * GetValues() const { return (uint8 *)&alpha; }
		      uint8 * GetValues()       { return (uint8 *)&alpha; }

		uint8 GetAlpha() const { return alpha; }
		uint8 GetA() const { return alpha; }
		uint8 GetRed() const { return red; }
		uint8 GetR() const { return red; }
		uint8 GetGreen() const { return green; }
		uint8 GetG() const { return green; }
		uint8 GetBlue() const { return blue; }
		uint8 GetB() const { return blue; }
	private:
		uint8 alpha;
		uint8 red;
		uint8 green;
		uint8 blue;
};

ColorARGB ColorRGB(uint8 r, uint8 g, uint8 b);
ColorARGB ColorGrey(uint8 lightness);

extern ColorARGB Black;
extern ColorARGB White;
extern ColorARGB Grey50;
extern ColorARGB Grey25;
extern ColorARGB Grey75;

extern ColorARGB Red;
extern ColorARGB Green;
extern ColorARGB Blue;
extern ColorARGB Yellow;
extern ColorARGB Violet;
extern ColorARGB Teal;

#endif