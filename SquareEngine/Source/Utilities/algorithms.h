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
	#define SortFuncDef bool (*sortFunction)(const void* data0, const void* data1)
	#define FindFuncDef bool (*findGreaterFunction)(const void* data0, const void* data1)
	#define LONG long int

	// Swaps two elements
	inline void Swap(void* currentPtr, void* nextPtr, LONG dataBlockSize)
	{
		void* cachedCurrentData = new char[dataBlockSize];
		memcpy(cachedCurrentData, currentPtr, dataBlockSize);

		memcpy(currentPtr, nextPtr, dataBlockSize);
		memcpy(nextPtr, cachedCurrentData, dataBlockSize);

		delete[] cachedCurrentData;
	}

	// Swaps the elements in this partition
	inline LONG QuickSortPartition(void* data,
		const LONG startIdx, const LONG endIdx, const LONG dataBlockSize, SortFuncDef)
	{
		char* pivotPtr = (char*)data + (endIdx * dataBlockSize);
		LONG i = (startIdx - 1);

		for(LONG j = startIdx; j <= endIdx - 1; j++)
		{
			char* currentJPtr = (char*)data + (j * dataBlockSize);

			if(sortFunction(currentJPtr, pivotPtr))
			{
				i++;
				char* currentIPtr = (char*)data + (i * dataBlockSize);
				Swap(currentIPtr, currentJPtr, dataBlockSize);
			}
		}

		char* nextPtr = (char*)data + ((i + 1) * dataBlockSize);
		char* endPtr = (char*)data + (endIdx * dataBlockSize);
		Swap(nextPtr, endPtr, dataBlockSize);
		return (i + 1);
	}

	// Starts with a pivot then puts all elements into left and right sides
	inline void QuickSort(void* data,
		const LONG startIdx, const LONG endIdx, const LONG dataBlockSize, SortFuncDef)
	{
		if(startIdx < endIdx)
		{
			LONG pivotIdx = QuickSortPartition(data, startIdx, endIdx, dataBlockSize, sortFunction);
			QuickSort(data, startIdx, pivotIdx - 1, dataBlockSize, sortFunction); // Left
			QuickSort(data, pivotIdx + 1, endIdx, dataBlockSize, sortFunction); // Right
		}
	}

	// Swaps elements while walking through it linearly
	inline void BubbleSort(void* data, 
		const LONG dataCount, const LONG dataBlockSize, SortFuncDef)
	{
		const LONG lastIdx = dataCount - 1;

		for(LONG i = 0; i < lastIdx; i++)
		{
			bool anySwapped = false;

			for(LONG j = 0; j < (dataCount - 1) - i; j++)
			{
				char* currentPtr = (char*)data + (j * dataBlockSize);
				char* nextPtr = (char*)data + ((j + 1) * dataBlockSize);

				if(!sortFunction(currentPtr, nextPtr))
				{
					Swap(currentPtr, nextPtr, dataBlockSize);
					anySwapped = true;
				}
			}

			if(!anySwapped)
			{
				break;
			}
		}
	}

	// Checks middle element then searchs right or left side (keeps doing this until found)
	inline int BinarySearch(void* data, const LONG dataCount,
		const void* dataToFind, const LONG dataBlockSize, FindFuncDef)
	{
		int startIdx = 0;
		int endIdx = dataCount - 1;
		while(startIdx <= endIdx)
		{
			const int middleIdx = (startIdx + endIdx) / 2;
			const char* middlePtr = (char*)data + (middleIdx * dataBlockSize);

			if(memcmp(middlePtr, dataToFind, dataBlockSize) == 0)
			{
				return middleIdx;
			} 
			else if(findGreaterFunction(dataToFind, middlePtr))
			{
				startIdx = middleIdx + 1;
			}
			else
			{
				endIdx = middleIdx - 1;
			}
		}

		return -1;
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
		auto sortFunc = [](const void* a, const void* b)
		{
			const int intA = *((int*)a);
			const int intB = *((int*)b);
			return intA < intB;
		};

		int sortBubbleData[] = { 4, 1, 0, 3, 2, 5 };
		Utilities::BubbleSort(&sortBubbleData, ARRAY_COUNT(sortBubbleData), sizeof(int), sortFunc);
		AlgorithmPrintData("bubble sort ", sortBubbleData, ARRAY_COUNT(sortBubbleData));

		int sortQuickData[] = { 4, 1, 0, 3, 2, 5 };
		Utilities::QuickSort(&sortQuickData, 0, ARRAY_COUNT(sortQuickData) - 1, sizeof(int), sortFunc);
		AlgorithmPrintData("quick sort ", sortQuickData, ARRAY_COUNT(sortQuickData));

		auto findGreaterFunction = [](const void* a, const void* b)
		{
			const int intA = *((int*)a);
			const int intB = *((int*)b);
			return intA > intB;
		};

		int findValue = 2;
		int foundIdx = BinarySearch(&sortQuickData, ARRAY_COUNT(sortQuickData), &findValue, sizeof(int), findGreaterFunction);
		printf("Binary search found at idx:%d!\n", foundIdx);
	}
}

#endif