#include <GL/glew.h>
#include "Mob.h"
#include "ObjModel.h"
#include "Shader.h"

void Mob::init()
{
	shaderID = initShader("simple");
	//pObjModel = new ObjModel("c:\\VrCave\\Development\\SwordArtOffline\\Data\\wolf\\Wolf.obj");
	pObjModel = new ObjModel("c:\\VrCave\\Development\\SwordArtOffline\\Data\\wolf2\\Wolf.obj");
	//pObjModel = new ObjModel("c:\\VrCave\\Development\\SwordArtOffline\\Data\\boss\\Mon.obj");
}

void Mob::draw()
{
	glUseProgram(shaderID);
	glPushMatrix();
	glScalef(20, 20, 20);
	pObjModel->draw(shaderID);
	glPopMatrix();
	glUseProgram(0);
}