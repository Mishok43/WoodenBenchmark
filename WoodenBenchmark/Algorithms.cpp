#pragma once
#include "WBenchmark.h"
#include "WDataGenerators.h"


void _declspec(noinline) InsertionSort(float* arr, std::size_t n)
{
	for (std::size_t i = 1; i < n; ++i)
	{
		float val = arr[i];
		int j = i;
		while (j > 0 && val < arr[j-1])
		{
			arr[j] = arr[j-1];
			--j;
		}
		arr[j] = val;
	}
}

void _declspec(noinline) InsertionSortSentinel(float* arr, std::size_t n)
{
	for (std::size_t i = 2; i < n; ++i)
	{
		float val = arr[i];
		int j = i;
		while (val < arr[j - 1])
		{
			arr[j] = arr[j - 1];
			--j;
		}
		arr[j] = val;
	}
}

void _declspec(noinline) ShellSort(int* arr, size_t size, int t, int f)
{
	int f = t;
	while (f < size / 2)
	{
		f *= t;
	}
	f /= t;

	for (std::size_t gap = f; gap > 0; gap /= t)
	{
		for (std::size_t i = gap; i < size; ++i)
		{
			int curValue = arr[i];
			int j;
			for (j = i; j >= gap && curValue < arr[j - gap]; j -= gap)
			{
				arr[j] = arr[j - gap];
			}

			arr[j] = curValue;
		}
	}
}



void BenchmarkSorts(const std::string& benchmarkLabel, float* arr, std::size_t n)
{

	float* arr1 = new float[n];
	std::memcpy(arr1, arr, n * sizeof(float));

	BENCHMARK(benchmarkLabel, InsertionSortSentinel, arr1, n);
	BENCHMARK(benchmarkLabel, InsertionSort, arr, n);

//	delete[] arr1;
}

int main()
{
	float N;
	std::cout << "Testing arrays. Input the size: " << std::endl;
	std::cin >> N;

	float max = 1000000.0;
	float min = 0.0;
	float* uniformUnsorted = WDataGenerator::generateArray<std::uniform_real_distribution<float>>(N, 0.0, 100000.0);
	uniformUnsorted[0] = 0; // hack for sentinal sortings
	BenchmarkSorts("Uniform Distributed", uniformUnsorted, N);

	float* normalUnsorted = WDataGenerator::generateArray<std::normal_distribution<float>>(N, max/2.0, max/6.0);
	normalUnsorted[0] = 0; // hack for sentinal sortings
	BenchmarkSorts("Normal Distributed", normalUnsorted, N);
	
	float* chiSquaredUnsorted = WDataGenerator::generateArray<std::chi_squared_distribution<float>>(N, 2);
	chiSquaredUnsorted[0] = 0; // hack for sentinal sortings
	BenchmarkSorts("Chi Squared Distributed", chiSquaredUnsorted, N);

	float* uniformPartiallySorted = WDataGenerator::generateArrayPartiallySorted<std::uniform_real_distribution<float>>(N, 2, 0.0, 1000000.0);
	uniformPartiallySorted[0] = 0; // hack for sentinal sortings
	BenchmarkSorts("Uniform Distributed| Partially Sorted", uniformPartiallySorted, N);

	/*
	delete[] uniformUnsorted;
	delete[] normalUnsorted;
	delete[] chiSquaredUnsorted;
	delete[] uniformPartiallySorted;
	*/

	return 0;

}