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

void _declspec(noinline) InsertionSortSentinelUnrooled(float* arr, std::size_t n)
{

	for (std::size_t i = 2; i < n; ++i)
	{
		float val = arr[i];
		std::size_t j = i;
		while (true)
		{
			float v1 = arr[j - 1];
			float v2 = arr[j - 2];
			float v3 = arr[j - 3];
			if (val < v1)
				arr[j] = v1;
			else
				break;
			float v4 = arr[j - 4];
			if (val < v2)
				arr[j - 1] = v2;
			else
				break;
			float v5 = arr[j - 5];
			if (val < v3)
				arr[j - 2] = v3;
			else
				break;
			if (val < v4)
				arr[j - 3] = v4;
			else
				break;

			if (val < v5)
				arr[j - 4] = v5;
			else
				break;


			j -= 5;
		}
		arr[j] = val;
	}
}

void _declspec(noinline) ShellSort(float* arr, size_t size, int t, int f)
{
	for (std::size_t gap = f; gap > 0; gap /= t)
	{
		for (std::size_t i = gap; i < size; ++i)
		{
			float curValue = arr[i];
			std::size_t j;
			for (j = i; j >= gap && curValue < arr[j - gap]; j -= gap)
			{
				arr[j] = arr[j - gap];
			}

			arr[j] = curValue;
		}
	}
}

void _declspec(noinline) MergeSort(float* _arr, size_t size)
{
	MergeSortNamespace::arr = _arr;
}

namespace MergeSortNamespace
{
	float* arr;

	void Sorting(size_t lo, size_t hi)
	{
		if (lo <= hi)
		{
			return;
		}

		size_t mid = lo + (hi - lo) / 2;
		Sorting(lo, mid);
		Sorting(mid + 1, hi);
		Merge(lo, mid, hi);
	}

	void Merge(size_t lo, size_t mi, size_t hi)
	{

	}

}

void BenchmarkSorts(const std::string& benchmarkLabel, float* arr, std::size_t n)
{
	/*float* arr0 = new float[n];
	std::memcpy(arr0, arr, n * sizeof(float));
	BENCHMARK(benchmarkLabel, InsertionSort, arr0, n);
	delete[] arr0;

	float* arr1 = new float[n];
	std::memcpy(arr1, arr, n * sizeof(float));
	BENCHMARK(benchmarkLabel, InsertionSortSentinel, arr1, n);
	delete[] arr1;

	*/
	float* arr2 = new float[n];
	std::memcpy(arr2, arr, n * sizeof(float));
	BENCHMARK(benchmarkLabel, InsertionSortSentinelUnrooled, arr2, n);
	delete[] arr2;


	int t = 2;
	int f = t;
	while (f < n/ 2)
	{
		f *= t;
	}
	f /= t;

	float* arr4 = new float[n];
	std::memcpy(arr4, arr, n * sizeof(float));
	BENCHMARK(benchmarkLabel, ShellSort, arr4, n, t, f);

	float* arr3 = new float[n];
	std::memcpy(arr3, arr, n * sizeof(float));
	BENCHMARK(benchmarkLabel, std::qsort, arr3, n, sizeof(float), [](const void* a, const void* b)
	{
		float a_v = *static_cast<const float*>(a);
		float b_v = *static_cast<const float*>(b);
		if (a_v < b_v)
			return -1;
		if (a_v > b_v)
			return 1;
		return 0;
	});
	delete[] arr3;


	std::cout << std::endl;
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
	BenchmarkSorts(str_format("Uniform Distributed | Partially Sorted - unsorting factor = %d", 1), uniformPartiallySorted0, N);
	delete[] uniformPartiallySorted0;

	float* uniformPartiallySorted1 = WDataGenerator::generateArrayPartiallySorted<std::uniform_real_distribution<float>>(N, 5, 0.0, 1000000.0);
	uniformPartiallySorted1[0] = -INFINITY; // hack for sentinal sortings
	BenchmarkSorts(str_format("Uniform Distributed | Partially Sorted - unsorting factor = %d", 7), uniformPartiallySorted1, N);
	delete[] uniformPartiallySorted1;

	float* uniformReverseSorted = WDataGenerator::generateReverseArray<std::uniform_real_distribution<float>>(N, 0.0, 1000000.0);
	uniformReverseSorted[0] = -INFINITY; // hack for sentinal sortings
	BenchmarkSorts(str_format("Uniform Distributed | Reverse Sorted"), uniformReverseSorted, N);
	delete[] uniformReverseSorted;
	

	return 0;

}