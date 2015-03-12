#include <noise\noise.h>
#include "noiseutils\noiseutils.h"
#include "TerrainManager.h"

using namespace noise;

TerrainManager::TerrainManager()
{
}


TerrainManager::~TerrainManager()
{
}

bool TerrainManager::CreateRandomTerrain()
{
	bool success = false;
	// preparing
	module::Perlin myModule;
	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(myModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	// setting the sizes and bounds:
	heightMapBuilder.SetDestSize(256, 256);
	heightMapBuilder.SetBounds(1.0, 5.0, 1.0, 5.0);
	// building
	heightMapBuilder.Build();

	// creating the image
	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	renderer.Render();

	// writing the image to file
	utils::WriterBMP writer;
	writer.SetSourceImage(image);
	writer.SetDestFilename("terrain.bmp");
	writer.WriteDestFile();

	return success;
}
