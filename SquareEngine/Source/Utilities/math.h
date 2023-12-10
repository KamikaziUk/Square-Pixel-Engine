// Copyright (c) Marty Green 2023
// https://github.com/KamikaziUk

// MIT License
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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
		else if(value >= max)
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
		else if(value >= max)
		{
			return max;
		}

		return value;
	}
}

#endif