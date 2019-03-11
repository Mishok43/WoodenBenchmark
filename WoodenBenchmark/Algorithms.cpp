#pragma once
#include "WBenchmark.h"
#include "WDataGenerators.h"
uint32_t* arrRandInt0;
uint32_t* arrRandInt1;
uint32_t* arrPartiallySortedInt0;

void _declspec(noinline) InsertionSort(std::size_t n)
{
	for (std::size_t i = 1; i < n; ++i)
	{
		uint32_t val = arrRandInt0[i];
		int j = i;
		while (j > 0 && val < arrRandInt0[j-1])
		{
			arrRandInt0[j] = arrRandInt0[j-1];
			--j;
		}
		arrRandInt0[j] = val;
	}
}

void _declspec(noinline) InsertionSortSentinel(std::size_t n)
{
	arrRandInt1[0] = 0;
	for (std::size_t i = 2; i < n + 1; ++i)
	{
		uint32_t val = arrRandInt1[i];
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
	uint32_t N;
	std::cin >> N;
	arrRandInt0 = WDataGenerator::generateArray<std::uniform_int_distribution<uint32_t>>(N, 0u, 100000u);

	BENCHMARK(InsertionSort, N);

	arrRandInt1 = new uint32_t[N + 1];
	std::memcpy(arrRandInt1 + 1, arrRandInt0, N * sizeof(uint32_t));
	BENCHMARK(InsertionSortSentinel, N);

	arrPartiallySortedInt0 = WDataGenerator::generatyArrayPartiallySorted<std::uniform_int_distribution<uint32_t>>(N, 0u, 20u, 2);

	return 0;

}