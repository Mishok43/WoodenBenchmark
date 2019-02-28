#pragma once
#include <random>
class WDataGenerator
{
public:
	static int* generateArrayIntUniform(std::size_t size, int minValue=0, int maxValue=10000000)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(minValue, maxValue);

		int* arr = new int[size];

		for (std::size_t i = 0; i < size; ++i)
		{
			arr[i] = dis(gen);
		}

		return arr;
	}
};

