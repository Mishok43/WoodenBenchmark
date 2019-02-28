#pragma once
#include "WBenchmark.h"
#include "WDataGenerators.h"

#define SIZE 100000

static int* arrRandInt0 = WDataGenerator::generateArrayIntUniform(SIZE, 0, 1000);
static int* arrRandInt1;

void _declspec(noinline) InsertionSort(int n)
{
	for (std::size_t i = 1; i < n; ++i)
	{
		int val = arrRandInt0[i];
		int j = i;
		while (j > 0 && val < arrRandInt0[j-1])
		{
			arrRandInt0[j] = arrRandInt0[j-1];
			--j;
		}
		arrRandInt0[j] = val;
	}
}

void _declspec(noinline) InsertionSortSentinel(int n)
{
	arrRandInt1[0] = 0;
	for (std::size_t i = 2; i < n + 1; ++i)
	{
		int val = arrRandInt1[i];
		int j = i;
		while (val < arrRandInt1[j - 1])
		{
			arrRandInt1[j] = arrRandInt1[j - 1];
			--j;
		}
		arrRandInt1[j] = val;
	}
}


int main()
{
	arrRandInt1 = new int[SIZE+1];
	std::memcpy(arrRandInt1 + 1, arrRandInt0, SIZE * sizeof(int));

	BENCHMARK(InsertionSortSentinel, SIZE);
	BENCHMARK(InsertionSort, SIZE);

	return 0;

}