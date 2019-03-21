#pragma once
#include <cassert>
#include <random>

class WDataGenerator
{
public:
	template<typename DistributionType, typename ValueType = DistributionType::result_type, typename... Args>
	static ValueType* generateArray(std::size_t size, Args... distributionArgs)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		DistributionType dis(distributionArgs...);
		
		ValueType* arr = new ValueType[size];

		for (std::size_t i = 0; i < size; ++i)
		{
			arr[i] = dis(gen);
		}

		return arr;
	}

	template<typename DistributionType, typename ValueType = DistributionType::result_type, typename... Args>
	static ValueType* generateArrayPartiallySorted(std::size_t size, uint8_t unsortedFactor, Args... distributionArgs)
	{
		constexpr uint8_t UNCORRECT_NUM_INDICES = uint8_t(0) - 1;
		constexpr std::size_t UNCORRECT_ID = std::size_t(0) - 1;

		ValueType* arr = generateArray<DistributionType>(size, distributionArgs...);
		std::sort(arr, arr + size);

		uint8_t *numPossibleIndices = new uint8_t[size];
		for (uint8_t i = 0; i < unsortedFactor; ++i)
		{
			uint8_t num = 1 + unsortedFactor + i;
			numPossibleIndices[i] = num;
			numPossibleIndices[size-1-i] = num;
		};

		std::memset(numPossibleIndices + unsortedFactor, 1 + 2 * unsortedFactor, (size - 2 * unsortedFactor)*sizeof(uint8_t));

		ValueType* res = new ValueType[size];

		std::uniform_real_distribution<> uniNormDist(0.0, 1.0);
		std::random_device rd;
		std::mt19937 e2(rd());
		for (std::size_t i = 0; i < size; ++i)
		{
			double overallPosabilities = 0.0;
			uint16_t iStart, iFinish;
			if (i < unsortedFactor)
			{
				iStart = 0u;
				iFinish = i + unsortedFactor;
			}
			else
			{
				if (i >= size-unsortedFactor)
				{
					iStart = i - unsortedFactor;
					iFinish = size - 1u;
				}
				else
				{
					iStart = i - unsortedFactor;
					iFinish = i + unsortedFactor;
				}
			}

			std::size_t iSwap = UNCORRECT_ID;

			for (std::size_t k = iStart; k <= iFinish; ++k)
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
				for (std::size_t k = iStart; k <= iFinish; ++k)
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

			for (std::size_t k = iStart; k <= iFinish; ++k)
			{
				if (numPossibleIndices[k] != UNCORRECT_NUM_INDICES)
				{
					assert(numPossibleIndices[k] >= 2);
					--numPossibleIndices[k];
				}
			}

			res[i] = arr[iSwap];
		}

		delete[] arr;

		return res;
	}
};