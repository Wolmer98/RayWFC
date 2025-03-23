#include "Utility.h"

int Utility::WeightedRandom(std::vector<int> weights)
{
	std::size_t count = weights.size();
	int weightSum = 0;
	for (std::size_t i = 0; i < count; i++) 
		weightSum += weights[i];
	
	int rnd = GetRandomValue(0, weightSum - 1);
	for (std::size_t i = 0; i < count; i++) 
	{
		if (rnd < weights[i])
			return i;
		rnd -= weights[i];
	}

	return -1;
}
