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
#ifndef UTILITIES_ALGORITHMS
#define UTILITIES_ALGORITHMS

#include "Utilities/carray.h"
#include <string.h>

namespace Utilities
{
	inline void BubbleSort(void* data, 
		const size_t dataCount, const size_t dataBlockSize, bool (*sortFunction)(void* data0, void* data1))
	{
		for(size_t i = 0; i < dataCount-1; i++)
		{
			bool anySwapped = false;

			for(size_t j = 0; j < dataCount - i - 1; j++)
			{
				auto currentPtr = (char*)data + (j * dataBlockSize);
				auto nextPtr = (char*)data + ((j + 1) * dataBlockSize);

				if(sortFunction(currentPtr, nextPtr))
				{
					void* cachedCurrentData = new char[dataBlockSize];
					memcpy(cachedCurrentData, currentPtr, dataBlockSize);

					memcpy(currentPtr, nextPtr, dataBlockSize);
					memcpy(nextPtr, cachedCurrentData, dataBlockSize);

					delete[] cachedCurrentData;
					anySwapped = true;
				}
			}

			if(!anySwapped)
			{
				break;
			}
		}
	}

	inline void AlgorithmPrintData(const char* algorithmName, int* data, int dataSize)
	{
		printf(algorithmName);
		printf("data: ");
		for(int i = 0; i < dataSize; i++)
		{
			printf("%d ", data[i]);
		}
		printf("\n");
	}

	inline void AlgorithmTesting()
	{
		auto sortFunc = [](void* a, void* b)
		{
			const int value0 = *((int*)a);
			const int value1 = *((int*)b);
			return value0 > value1;
		};

		int sortData[] = { 4, 1, 0, 3, 2, 5 };

		Utilities::BubbleSort(&sortData, ARRAY_COUNT(sortData), sizeof(int), sortFunc);
		AlgorithmPrintData("bubble sort", sortData, ARRAY_COUNT(sortData));
	}
}

#endif