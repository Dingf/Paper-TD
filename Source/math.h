#ifndef INC_MATH_H
#define INC_MATH_H

#include <math.h>
#include "type.h"

#define USE_C_MATH

#ifndef e
#define e 2.718281828459045
#endif//e
#ifndef pi
#define pi 3.141592653589793
#endif//pi
#ifndef sqrt2
#define sqrt2 1.414213562373095
#endif//sqrt2
#ifndef sqrt3
#define sqrt3 1.732050807568877
#endif//sqrt3

#define AbsVal(x) ((x<0)?(-1*x):(x))

#define Pow2(x) ((x)*(x))
#define Pow3(x) ((x)*(x)*(x))

#define DegToRad(x) ((x)*(0.017453292))
#define RadToDeg(x) ((x)*(57.29577951))

Float Smooth(Float value, Float precision = 1E-4);
Float Ceiling(Float value);
Float Floor(Float value);

#ifdef USE_C_MATH
#define SinD(x) sin(x*0.017453292)
#define SinR(x) sin(x)
#define CosD(x) cos(x*0.017453292)
#define CosR(x) cos(x)
#define TanD(x) tan(x*0.017453292)
#define TanR(x) tan(x)
#define ArcSinD(x) (asin(x)*57.29577951)
#define ArcSinR(x) asin(x)
#define ArcCosD(x) (acos(x)*57.29577951)
#define ArcCosR(x) acos(x)
#define ArcTanD(x) (atan(x)*57.29577951)
#define ArcTanR(x) atan(x)

#define Log2(x) (log(x)/log(2.0))
#define LogN(b,x) (log(x)/log(b))
#define Log10(x) log10(x)
#define LogE(x) log(x)
#define Ln(x) log(x)

#define ExpE(x) exp(x)
#define Exp(x) exp(x)
#define ExpN(b,x) pow(b,x)
#define Exp10(x) exp(x*log(10.0))
#define Exp2(x) exp(x*log(2.0))

#define Root2(x) sqrt(x)
#define Root3(x) exp(log(x)/3.0)
#define RootN(b,x) exp(log(x)/b)

#else
Float SinD(Float value);
Float SinR(Float value);
Float CosD(Float value);
Float CosR(Float value);
Float TanD(Float value);
Float TanR(Float value);

Float Log2(Float value);
Float LogN(Float base, Float value);
Float Log10(Float value);
Float LogE(Float value);
Float Ln(Float value);

Float ExpE(Float value);
Float Exp(Float value);
Float ExpN(Float base, Float exponent);
Float Exp10(Float exponent);
Float Exp2(Float exponent);

Float Root2(Float value);
Float Root3(Float value);
Float RootN(Float degree, Float value);
#endif

/*
Note: Write the inverse trig functions that are not part of C Standard Lib.
*/

#endif


