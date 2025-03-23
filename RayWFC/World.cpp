#include "World.h"
#include "raylib.h"
#include "Utility.h"

#include <algorithm>
#include <experimental/generator>
#include <numeric>
#include <iostream>
#include <format>

std::experimental::generator<Vector2> World::GridCellIterator()
{
	for (std::size_t y = 0; y < GRIDHEIGHT; ++y)
		for (std::size_t x = 0; x < GRIDWIDTH; ++x)
			co_yield Vector2(x, y);
}

std::vector<ETileType>* World::GetPossibilities(Vector2 coordinate)
{
	if (coordinate.x < 0 || coordinate.x >= GRIDWIDTH)
		return nullptr;
	if (coordinate.y < 0 || coordinate.y >= GRIDHEIGHT)
		return nullptr;

	return &m_possibilitySpace[coordinate.y][coordinate.x];
}

std::optional<Vector2> World::GetLowestEntropyTile()
{
	std::size_t lowestEntropy = std::numeric_limits<std::size_t>::max();
	Vector2 lowestEntropyCoordinate = Vector2(-1, -1);

	for (auto& [x, y] : GridCellIterator())
	{
		bool bHasCellCollapsed = m_grid[y][x] != MAX;

		auto entropy = GetPossibilities(Vector2(x, y))->size();
		if (entropy < lowestEntropy && !bHasCellCollapsed)
		{
			lowestEntropy = entropy;
			lowestEntropyCoordinate = Vector2(x, y);
		}
	}
	
	if (lowestEntropyCoordinate.x < 0)
		return std::nullopt;

	return lowestEntropyCoordinate;
}

void World::PropagateEntropy(std::vector<Vector2>& openTiles)
{
	auto coordinate = openTiles.back();
	openTiles.pop_back();

	auto* possibilities = GetPossibilities(coordinate);
	if (!possibilities)
		return;

	std::vector<ETileType> newPossibilities;

	for (int i = 0; i < possibilities->size(); ++i)
	{
		bool bIsTypeAvailable = true;
		for (int y = -1; y <= 1; ++y)
		{
			for (int x = -1; x <= 1; ++x)
			{
				if (x == 0 && y == 0)
					continue;

				auto neighbours = GetPossibilities(Vector2(coordinate.x + x, coordinate.y + y));
				if (!neighbours)
					continue;

				if (!Tile::IsTypeAvailable(possibilities->at(i), neighbours))
				{
					bIsTypeAvailable = false;
					break;
				}
			}

			if (!bIsTypeAvailable)
				break;
		}
	
		if (bIsTypeAvailable)
			newPossibilities.push_back(possibilities->at(i));
	}

	if (newPossibilities.size() == 0)
		return;

	if (newPossibilities.size() != possibilities->size())
		InsertNeighbours(coordinate, openTiles);

	*possibilities = newPossibilities;
}

void World::CollapseTile(Vector2 coordinate, std::vector<Vector2>& openTiles)
{
	auto& possibilities = m_possibilitySpace[coordinate.y][coordinate.x];
	if (possibilities.size() <= 0)
	{
		std::cout << std::format("Error trying to collapse empty possibility space ({},{})", coordinate.x, coordinate.y) << std::endl;
		return;
	}

	std::vector<int> weights;
	weights.resize(ETileType::MAX);

	// calculate weights based on neighbouring collapsed tiles.
	for (auto possibility : possibilities)
	{
		weights[possibility] += 1; // Offset weight
		for (int y = -1; y <= 1; ++y)
		{
			for (int x = -1; x <= 1; ++x)
			{
				if (x == 0 && y == 0)
					continue;

				if (coordinate.x + x <= 0 || coordinate.x + x >= GRIDWIDTH || coordinate.y + y <= 0 || coordinate.y + y >= GRIDHEIGHT)
					continue;

				auto neighbourTile = m_grid[coordinate.y + y][coordinate.x + x];
				if (neighbourTile != possibility)
					continue;

				weights[neighbourTile]++;
			}
		}
	}
	
	m_grid[coordinate.y][coordinate.x] = static_cast<ETileType>(Utility::WeightedRandom(weights));
	possibilities = std::vector<ETileType>{ m_grid[coordinate.y][coordinate.x] };
	InsertNeighbours(coordinate, openTiles);
}

void World::InsertNeighbours(Vector2 coordinate, std::vector<Vector2>& tiles)
{
	for (int y = -1; y <= 1; ++y)
		for (int x = -1; x <= 1; ++x)
			tiles.push_back(Vector2(coordinate.x + x, coordinate.y + y));
}

void World::GenerateWorld()
{
	for (auto& [x, y] : GridCellIterator())
	{
		m_possibilitySpace[y][x].clear();
		for (std::size_t i = 0; i != ETileType::MAX; ++i)
			m_possibilitySpace[y][x].push_back((ETileType)i);

		m_grid[y][x] = ETileType::MAX;
	}

	std::vector<Vector2> openTiles;
	openTiles.reserve(GRIDHEIGHT * GRIDWIDTH);

	std::optional<Vector2> currentCoordinate = Vector2(20, 15); // Start coordinate
	CollapseTile(currentCoordinate.value(), openTiles);

	do
	{
		while (openTiles.size() > 0)
			PropagateEntropy(openTiles);

		currentCoordinate = GetLowestEntropyTile();
		if (!currentCoordinate.has_value())
			break;

		CollapseTile(currentCoordinate.value(), openTiles);
	} while (true);
}

void World::RenderWorld()
{
	for (auto& [x, y] : GridCellIterator())
	{
		Tile::RenderTile(m_grid[y][x], Vector2(x * GRIDCELLSIZE, y * GRIDCELLSIZE), GRIDCELLSIZE - 1);
	}
}
