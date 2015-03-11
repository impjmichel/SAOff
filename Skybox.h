#ifndef SKYBOX_H
#define sKYBOX_H

#define SKYBOX_SCALE 750

class Skybox
{
public:
	void draw();

private:
	void initTexturePart(char *partName, int partNumber);
	void initTexturing();
	unsigned int textureIDs[6];
	bool isInitialized = false;
};

#endif