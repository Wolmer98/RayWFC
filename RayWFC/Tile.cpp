#include "Tile.h"
#include "raylib.h"
#include "Utility.h"

#include <unordered_map>

std::unordered_map<ETileType, std::vector<ETileType>> adjacencies =
{
	{Grass, {Grass, Forest, Shore}},
	{Water, {Water, Shore}},
	{Forest, {Forest, Grass, Mountain}},
	{Shore, {Shore, Water, Grass}},
	{Mountain, {Mountain, Forest}},
};

bool Tile::IsTypeAvailable(ETileType tileType, std::vector<ETileType>* neighbours)
{
	return Utility::ContainsAnyFromB(&adjacencies[tileType], neighbours);
}

void Tile::RenderTile(ETileType tileType, Vector2 position, std::size_t size)
{
	Color color = WHITE;
	switch (tileType)
	{
	case Grass: color = GREEN; break;
	case Forest: color = DARKGREEN; break;
	case Water: color = SKYBLUE; break;
	case Shore: color = BEIGE; break;
	case Mountain: color = GRAY; break;
	}

	DrawRectangle(position.x, position.y, size, size, color);
}

