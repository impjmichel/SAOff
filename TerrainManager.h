#pragma once

class TerrainManager
{
public:
	virtual unsigned char * CreateRandomTerrain(unsigned int width, unsigned int height);
	virtual void createVegetation(int amount);
};
