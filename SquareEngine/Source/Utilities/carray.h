#pragma once
#ifndef C_ARRAY
#define C_ARRAY

namespace Utilities
{
	#define ARRAY_COUNT(arrayName) sizeof(arrayName) / sizeof(arrayName[0])
}

#endif