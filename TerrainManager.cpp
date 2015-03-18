#include <noise\noise.h>
#include "noiseutils\noiseutils.h"
#include "TerrainManager.h"
#include <iostream>

using namespace noise;
using namespace std;

TerrainManager::TerrainManager()
{
}


TerrainManager::~TerrainManager()
{
}

unsigned char * TerrainManager::CreateRandomTerrain( unsigned int width, unsigned int height)
{
	bool success = false;
	// preparing
	module::Perlin myModule;
	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(myModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	// setting the sizes and bounds:
	heightMapBuilder.SetDestSize(width, height);
	heightMapBuilder.SetBounds(1.0, 5.0, 1.0, 5.0);
	// building
	heightMapBuilder.Build();

	// creating the image
	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	renderer.Render();

	// rewrite the image to byte array thingy that can be used by the Terrain
	const int size = image.GetWidth() * image.GetHeight();
	unsigned char * map = new unsigned char[size];

	for (int y = 0; y < image.GetHeight(); ++y)
	{
		for (int x = 0; x < image.GetWidth(); ++x)
		{
			map[y * image.GetWidth() + x] = image.GetValue(x, y).red;
		}
	}
	return map;
}
