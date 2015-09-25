#include <math.h>
#include "math.h"

#ifndef USE_C_MATH

Float NaN()
{
	Float indeterminate = 0.0;
	return 0.0f/indeterminate;
}

Float Log2Rem(Float value, Float fractPart = 1.0, Float precision = 1.0E-10)
{
	if ((value > 1.0) && (value < 2.0))
	{
		if (fractPart < precision)
		{
			return 0;
		}

		while (value < 2.0)
		{
			fractPart /= 2.0;
			value *= value;
		}

		return fractPart + Log2Rem(value/2.0, fractPart);
	}
	return 0;
}

Float Log2(Float value)
{
	if (value <= 0)
	{
		return NaN();
	}
	else if (value == 2.0)
	{
		return 1.0;
	}

	int32 integerPart = 0;
	Float remainder = value;

	//Format into the range [1, 2)
	while (remainder < 1.0)
	{
		integerPart--;
		remainder *= 2.0;
	}
	while (remainder >= 2.0)
	{
		integerPart++;
		remainder /= 2.0;
	}

	return (float)integerPart + Log2Rem(remainder);
}

Float LogN(Float base, Float value)
{
	return Log2(value)/Log2(base);
}

Float Log10(Float value)
{
	return Log2(value)/Log2(10.0);
}

Float LogE(Float value)
{
	return Log2(value)/Log2(e);
}

Float Ln(Float value)
{
	return Log2(value)/Log2(e);
}

Float PowInt(Float base, uint32 exponent)
{
	if (exponent == 0)
	{
		return 1.0;
	}
	else if (exponent == -1)
	{
		return 1.0/base;
	}
	else if (exponent == 1)
	{
		return base;
	}

	Float result = 1.0;
	Float baseValue = base;
	
	while (exponent > 1)
	{
		if ((exponent % 2) == 1)
		{
			result *= baseValue;
		}
		exponent /= 2;
		baseValue *= baseValue;
	}

	result *= baseValue;

	return result;
}

Float ExpE(Float value)
{
	return PowInt((1 + value/1E9), (uint32)1E9);
}

Float Exp(Float value)
{
	return PowInt((1 + value/1E9), (uint32)1E9);
}

Float ExpN(Float base, Float exponent)
{
	return Exp(exponent * LogE(base));
}

Float Exp10(Float exponent)
{
	return Exp(exponent * LogE(10.0));
}

Float Exp2(Float exponent)
{
	return Exp(exponent * LogE(2.0));
}


Float Root2(Float value)
{
	return (value <= 0) ? 0 : Exp(Ln(value)/2.0);
}

Float Root3(Float value)
{
	return (value == 0) ? 0 : Exp(Ln(value)/3.0);
}

Float RootN(Float degree, Float value)
{
	return (value == 0) ? 0 : Exp(Ln(value)/degree);
}

Float SineTable[] = 
{
	0.0000000000, 0.0174524064, 0.0348994967, 0.0523359562, 0.0697564737, 0.0871557427,
	0.1045284633, 0.1218693434, 0.1391731010, 0.1564344650, 0.1736481777, 0.1908089954,
	0.2079116908, 0.2249510543, 0.2419218956, 0.2588190451, 0.2756373558, 0.2923717047,
	0.3090169944, 0.3255681545, 0.3420201433, 0.3583679495, 0.3746065934, 0.3907311285,
	0.4067366431, 0.4226182617, 0.4383711468, 0.4539904997, 0.4694715628, 0.4848096202,
	0.5000000000, 0.5150380749, 0.5299192642, 0.5446390350, 0.5591929035, 0.5735764364,
	0.5877852523, 0.6018150232, 0.6156614753, 0.6293203910, 0.6427876097, 0.6560590290,
	0.6691306064, 0.6819983601, 0.6946583705, 0.7071067812, 0.7193398003, 0.7313537016,
	0.7431448255, 0.7547095802, 0.7660444431, 0.7771459615, 0.7880107536, 0.7986355100,
	0.8090169944, 0.8191520443, 0.8290375726, 0.8386705679, 0.8480480962, 0.8571673007,
	0.8660254038, 0.8746197071, 0.8829475929, 0.8910065242, 0.8987940463, 0.9063077870,
	0.9135454576, 0.9205048535, 0.9271838546, 0.9335804265, 0.9396926208, 0.9455185756,
	0.9510565163, 0.9563047560, 0.9612616959, 0.9659258263, 0.9702957263, 0.9743700648,
	0.9781476007, 0.9816271834, 0.9848077530, 0.9876883406, 0.9902680687, 0.9925461516,
	0.9945218954, 0.9961946981, 0.9975640503, 0.9986295348, 0.9993908270, 0.9998476952,
	1.0000000000
};

Float SinD(Float value)
{
	Float angle = value;
	uint8 quadrant = 1;
	Float floor = Floor(value);

	if (floor >= 360)
	{
		Float overlap = (uint32)floor % 360;
		angle = value - floor + overlap;
	}
	if (floor < 0)
	{
		Float overlap = (uint32)AbsVal(floor) % 360;
		angle = 360 - (AbsVal(value) - AbsVal(floor) + overlap);
	}

	while (angle > 90.0)
	{
		angle -= 90.0;
		quadrant++;
	}

	if ((quadrant % 2) == 1)		//I and III
	{
		Float point1 = SineTable[(int32)angle];
		Float point2 = SineTable[(int32)angle+1];

		Float remainder = angle - (int32)angle;

		return point1 + (remainder * (point2 - point1));
	}
	else if ((quadrant % 2) == 0)
	{
		Float point1 = SineTable[90-(int32)angle];
		Float point2 = SineTable[89-(int32)angle];

		Float remainder = angle - (int32)angle;

		return point1 + (remainder * (point2 - point1));
	}

	return NaN();
}

Float SinR(Float value)
{
	return SinD(value/pi * 180);
}
Float CosD(Float value)
{
	return SinD(value - 90);
}
Float CosR(Float value)
{
	return CosD(value/pi * 180);
}
Float TanD(Float value)
{
	return SinD(value)/CosD(value);
}
Float TanR(Float value)
{
	return SinR(value)/CosR(value);
}
#endif

Float Smooth(Float value, Float precision)
{
	if (value == value)		//If not-NaN
	{
		value /= precision;
		int32 result = (int32)value;

		if ((value - result) > 0.5)
		{
			result++;
		}
	
		return (float)result * precision;
	}
	return value;
}

Float Ceiling(Float value)
{
	if (value == value)
	{
		int32 result = (int32)value;

		if (((Smooth(value - result)) != 0) && (result > 0))
		{
			return (float)result + 1.0;
		}
		else
		{
			return (float)result;
		}
	}
	return value;
}

Float Floor(Float value)
{
	if (value == value)
	{
		int32 result = (int32)value;

		if (((Smooth(value - result)) != 0) && (result < 0))
		{
			return (float)result - 1.0;
		}
		else
		{
			return (float)result;
		}
	}
	return value;
}
