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
#ifndef AABB_BOX
#define AABB_BOX

namespace Utilities
{
	struct AABB
	{
		float x;
		float y;
		float w;
		float h;

		AABB()
		{
			x = 0;
			y = 0;
			w = 0;
			h = 0;
		}

		AABB(float inX, float inY, float inW, float inH)
		{
			x = inX;
			y = inY;
			w = inW;
			h = inH;
		}
	};

	bool operator<(const AABB& lhs, const AABB& rhs)
	{
		return
			(lhs.x < rhs.x + rhs.w &&
				lhs.x + lhs.w > rhs.x &&
				lhs.y < rhs.y + rhs.h &&
				lhs.h + lhs.y > rhs.y);
	}
}

#endif