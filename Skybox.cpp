#include "Skybox.h"

#include <GL\glew.h>
#include <CaveLib\CaveLib\texture.h>

#include "stb_image.h"
#include "shader.h"

#include <Windows.h>

void Skybox::initTexturing()
{
	if (!isInitialized)
	{
		isInitialized = true;
		initTexturePart("Front", 0);
		initTexturePart("Left", 1);
		initTexturePart("Back", 2);
		initTexturePart("Right", 3);
		initTexturePart("Up", 4);
		initTexturePart("Down", 5);
	}
}

void Skybox::initTexturePart(char *partName, int partNumber)
{
	glGenTextures(1, &textureIDs[partNumber]);
	glBindTexture(GL_TEXTURE_2D, textureIDs[partNumber]);

	int width = 1024, height = 1024, bpp = 32;
	char finalTexturePath[MAX_PATH];
	sprintf_s(finalTexturePath, "C:/VrCave/Development/SwordArtOffline/Data/Skybox2%s%s", partName, ".png");
	unsigned char* imgData = stbi_load(finalTexturePath, &width, &height, &bpp, 4);
	glTexImage2D(GL_TEXTURE_2D,
		0,					//level
		GL_RGBA,			//internal format
		width,				//width
		height,				//height
		0,					//border
		GL_RGBA,			//data format
		GL_UNSIGNED_BYTE,	//data type
		imgData);			//data

	stbi_image_free(imgData);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
	
void Skybox::draw()
{
	initTexturing();
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_CULL_FACE);

	// Just in case we set all vertices to white.
	glColor4f(1, 1, 1, 1);
	glPushMatrix();
	glScalef(SKYBOX_SCALE, SKYBOX_SCALE, SKYBOX_SCALE);
	glTranslatef(0, 0, 0);

	// Render the front quad
	glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(1, 1); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(1, 0); glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2f(0, 0); glVertex3f(0.5f, 0.5f, -0.5f);
	glEnd();

	// Render the left quad
	glBindTexture(GL_TEXTURE_2D, textureIDs[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(1, 1); glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(1, 0); glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2f(0, 0); glVertex3f(0.5f, 0.5f, 0.5f);
	glEnd();

	// Render the back quad
	glBindTexture(GL_TEXTURE_2D, textureIDs[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2f(1, 1); glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(1, 0); glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2f(0, 0); glVertex3f(-0.5f, 0.5f, 0.5f);

	glEnd();

	// Render the right quad
	glBindTexture(GL_TEXTURE_2D, textureIDs[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(1, 1); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2f(1, 0); glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2f(0, 0); glVertex3f(-0.5f, 0.5f, -0.5f);
	glEnd();

	// Render the top quad
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, textureIDs[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0); glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2f(0, 0); glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2f(0, 1); glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2f(1, 1); glVertex3f(0.5f, 0.5f, -0.5f);
	glEnd();
	glPopMatrix();

	// Render the bottom quad
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, textureIDs[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(0, 0); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2f(1, 0); glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(1, 1); glVertex3f(0.5f, -0.5f, -0.5f);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);

	// Restore enable bits and matrix
	glPopAttrib();
	glPopMatrix();
}
