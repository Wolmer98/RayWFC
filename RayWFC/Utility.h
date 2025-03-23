#pragma once

#include <vector>
#include <stdarg.h>
#include "raylib.h"

class Utility
{
public:

	template<typename T>
	static bool Contains(std::vector<T>* collection, T element);
	
	template<typename T>
	static bool ContainsAnyFromB(std::vector<T>* collectionA, std::vector<T>* collectionB);

	static int WeightedRandom(std::vector<int> weights);
};

inline bool operator==(const Vector2& A, const Vector2& B)
{
	return A.x == B.x && A.y == B.y;
};

template<typename T>
inline bool Utility::Contains(std::vector<T>* collection, T element)
{
	if (!collection)
		return false;

	return std::find(collection->begin(), collection->end(), element) != collection->end();
}

template<typename T>
inline bool Utility::ContainsAnyFromB(std::vector<T>* collectionA, std::vector<T>* collectionB)
{
	if (!collectionA || !collectionB)
		return false;

	// TODO: Maybe optimize later.

	for (auto& ElementB : *collectionB)
		for (auto& ElementA : *collectionA)
			if (ElementB == ElementA)
				return true;

	return false;
}