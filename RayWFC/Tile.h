#pragma once

#include "raylib.h"
#include <vector>

enum ETileType
{
	Grass,
	Water,
	Forest,
	Shore,
	Mountain,
	MAX,
};

class Tile
{
public:

public:
	
	static bool IsTypeAvailable(ETileType tileType, std::vector<ETileType>* neighbours);
	static void RenderTile(ETileType tileType, Vector2 position, std::size_t size);
};