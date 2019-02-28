#pragma once
#include <random>

class WDataGenerator
{
public:
	template<typename DistributionType, typename ValueType = DistributionType::result_type>
	static ValueType* generateArray(std::size_t size, ValueType minValue, ValueType maxValue)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		DistributionType dis(minValue, maxValue);
		
		ValueType* arr = new ValueType[size];

		for (std::size_t i = 0; i < size; ++i)
		{
			arr[i] = dis(gen);
		}

		return arr;
	}
};

int f(){
	int* t = WDataGenerator::generateArray<std::uniform_int_distribution<int> >((size_t)0, 1, 2);
}
