#pragma once
#include "StringFormat.h"
#include "WBenchmark.h"
#include "WDataGenerators.h"


void _declspec(noinline) InsertionSort(float* arr, std::size_t n)
{
	for (std::size_t i = 1; i < n; ++i)
	{
		float val = arr[i];
		std::size_t j = i;
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
		std::size_t j = i;
		while (val < arr[j - 1])
		{
			arr[j] = arr[j - 1];
			--j;
		}
		arr[j] = val;
	}
}



void inline BenchmarkSorts(const std::string& benchmarkLabel, float* arr, std::size_t n)
{

	float* arr0 = new float[n];
	std::memcpy(arr0, arr, n * sizeof(float));
	BENCHMARK(benchmarkLabel, InsertionSort, arr0, n);
	delete[] arr0;

	float* arr1 = new float[n];
	std::memcpy(arr1, arr, n * sizeof(float));
	BENCHMARK(benchmarkLabel, InsertionSortSentinel, arr1, n);
	delete[] arr1;
}

int main()
{
	std::size_t N;
	std::cout << "Testing arrays. Input the size: " << std::endl;
	std::cin >> N;

	float max = 1000000.0;
	float min = 0.0;
	float* uniformUnsorted = WDataGenerator::generateArray<std::uniform_real_distribution<float>>(N, 0.0, 100000.0);
	uniformUnsorted[0] = -INFINITY; // hack for sentinal sortings
	BenchmarkSorts(str_format("Uniform Distributed"), uniformUnsorted, N);

	delete[] uniformUnsorted;
	float* normalUnsorted = WDataGenerator::generateArray<std::normal_distribution<float>>(N, max/2.0, max/10.0);
	normalUnsorted[0] = -INFINITY; // hack for sentinal sortings
	BenchmarkSorts("Normal Distributed", normalUnsorted, N);
	delete[] normalUnsorted;
	
	float* chiSquaredUnsorted = WDataGenerator::generateArray<std::chi_squared_distribution<float>>(N, 2);
	chiSquaredUnsorted[0] = -INFINITY; // hack for sentinal sortings
	BenchmarkSorts("Chi Squared Distributed", chiSquaredUnsorted, N);
	delete[] chiSquaredUnsorted;

	float* uniformPartiallySorted0 = WDataGenerator::generateArrayPartiallySorted<std::uniform_real_distribution<float>>(N, 1, 0.0, 1000000.0);
	uniformPartiallySorted0[0] = -INFINITY; // hack for sentinal sortings
	BenchmarkSorts(str_format("Uniform Distributed | Partially Sorted - %d", 1), uniformPartiallySorted0, N);
	delete[] uniformPartiallySorted0;

	float* uniformPartiallySorted1 = WDataGenerator::generateArrayPartiallySorted<std::uniform_real_distribution<float>>(N, 5, 0.0, 1000000.0);
	uniformPartiallySorted1[0] = -INFINITY; // hack for sentinal sortings
	BenchmarkSorts(str_format("Uniform Distributed | Partially Sorted - %d", 7), uniformPartiallySorted1, N);
	delete[] uniformPartiallySorted1;
	

	return 0;

}