#pragma once
#ifndef UTILITIES_MATH
#define UTILITIES_MATH

#include <cmath>

namespace Utilities
{
	inline int RandomMinMax(int min, int max)
	{
		if(min == 0 && max == 0)
		{
			return 0;
		}

		int random = (rand() % (max - min + 1)) + min;
		return random;
	}

	inline float Clamp(float min, float max, float value)
	{
		if(value < min)
		{
			return min;
		}
		else if(value > max)
		{
			return max;
		}

		return value;
	}

	inline int Clamp(int min, int max, int value)
	{
		if(value < min)
		{
			return min;
		}
		else if(value > max)
		{
			return max;
		}

		return value;
	}
}

#endif