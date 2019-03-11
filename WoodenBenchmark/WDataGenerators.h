#pragma once
#include <cassert>
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

	template<typename DistributionType, typename ValueType = DistributionType::result_type>
	static ValueType* generateArrayPartiallySorted(std::size_t size, ValueType minValue, ValueType maxValue, uint8_t unsortedFactor)
	{
		constexpr uint8_t UNCORRECT_NUM_INDICES = uint8_t(0) - 1;
		constexpr std::size_t UNCORRECT_ID = std::size_t(0) - 1;

		ValueType* arr = generateArray<DistributionType>(size, minValue, maxValue);
		std::sort(arr, arr + size);
		
		std::size_t* swapIndices = new std::size_t[size];
		for (std::size_t i = 0; i < size; ++i)
		{
			swapIndices[i] = i;
		}

		uint8_t *numPossibleIndices = new uint8_t[size];
		for (uint8_t i = 0; i < unsortedFactor; ++i)
		{
			uint8_t num = 1 + unsortedFactor + i;
			numPossibleIndices[i] = num;
			numPossibleIndices[size-1-i] = num;
		};

		std::memset(numPossibleIndices + unsortedFactor, 1 + 2 * unsortedFactor, (size - 2 * unsortedFactor)*sizeof(uint8_t));

		std::random_shuffle(swapIndices, swapIndices + size);

		ValueType* res = new ValueType[size];

		std::uniform_real_distribution<> uniNormDist(0.0, 1.0);
		std::random_device rd;
		std::mt19937 e2(rd());
		for (std::size_t i = 0; i < size; ++i)
		{
			std::size_t j = swapIndices[i];

			double overallPosabilities = 0.0;
			uint16_t iStart, iFinish;
			if (j < unsortedFactor)
			{
				iStart = 0u;
				iFinish = j + unsortedFactor;
			}
			else
			{
				if (j >= size-unsortedFactor)
				{
					iStart = j - unsortedFactor;
					iFinish = size - 1u;
				}
				else
				{
					iStart = j - unsortedFactor;
					iFinish = j + unsortedFactor;
				}
			}

			std::size_t iSwap = UNCORRECT_ID;

			for (uint16_t k = iStart; k <= iFinish; ++k)
			{
				if (numPossibleIndices[k] != UNCORRECT_NUM_INDICES)
				{
					if (numPossibleIndices[k] != 1)
					{
						overallPosabilities += 1.0/numPossibleIndices[k];
					}
					else
					{
						iSwap = k;
						break;
					}
				}
			}
			bool useDistribution = false;

			// if don't find an index for swapping, then randomly pick it using theory of universality of probability
			if (iSwap == UNCORRECT_ID)
			{
				double u = uniNormDist(e2); // get uniform distributed random variable

				double curCDF = 0.0; // current value of CDF
				for (uint16_t k = iStart; k <= iFinish; ++k)
				{
					if (numPossibleIndices[k] != UNCORRECT_NUM_INDICES)
					{
						curCDF += 1.0 / numPossibleIndices[k] / overallPosabilities; // renormalize it by dividing by overallPossabilities

						if (u <= curCDF)
						{
							iSwap = k;
							break;
						}
					}
				}
				useDistribution = true;
			}

			numPossibleIndices[iSwap] = UNCORRECT_NUM_INDICES; // set UNCORRECT_NUM_INDICES as flag for not considering it in future

			for (uint16_t k = iStart; k <= iFinish; ++k)
			{
				if (numPossibleIndices[k] != UNCORRECT_NUM_INDICES)
				{
					assert(numPossibleIndices[k] >= 2);
					--numPossibleIndices[k];
				}
			}

			res[j] = iSwap;
		}

		delete[] arr;

		return res;
	}
};