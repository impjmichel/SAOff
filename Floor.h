#ifndef FLOOR_H
#define FLOOR_H

#include <vector>
#define FLOOR_WIDTH 240
#define FLOOR_HEIGHT 240

struct TilePoint
{
	float xCoordinate, yCoordinate, zCoordinate;
};

struct Tile
{
	TilePoint tilePoints[4];
};

class Floor
{
public:
	Floor() { generate(); };
	void draw();

private:
	unsigned int textureID = 0;
	void generate();
	void initTexture();
};

#endif