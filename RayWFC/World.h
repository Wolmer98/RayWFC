#pragma once

#include "Tile.h"
#include <array>
#include <vector>
#include <experimental/generator>
#include <optional>

class World
{
public:

	static const std::size_t GRIDWIDTH = 48;//48;
	static const std::size_t GRIDHEIGHT = 32;//32;
	static const std::size_t GRIDCELLSIZE = 25;
	std::array<std::array<ETileType, GRIDWIDTH>, GRIDHEIGHT> m_grid;
	std::array<std::array<std::vector<ETileType>, GRIDWIDTH>, GRIDHEIGHT> m_possibilitySpace;

public:

	[[nodiscard]] std::experimental::generator<Vector2> GridCellIterator();

	[[nodiscard]] std::vector<ETileType>* GetPossibilities(Vector2 coordinate);
	[[nodiscard]] std::optional<Vector2> GetLowestEntropyTile();
	void PropagateEntropy(std::vector<Vector2>& openTiles);
	void CollapseTile(Vector2 coordinate, std::vector<Vector2>& openTiles);
	void InsertNeighbours(Vector2 coordinate, std::vector<Vector2>& tiles);
	void GenerateWorld();
	void RenderWorld();
};

