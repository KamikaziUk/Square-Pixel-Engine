#pragma once
#ifndef AABB_BOX
#define AABB_BOX

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

#endif