#pragma once


class TerrainManager
{
public:
	TerrainManager();
	~TerrainManager();
	unsigned char * CreateRandomTerrain(unsigned int width, unsigned int height);
};

