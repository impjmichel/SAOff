#include <GL/glew.h>
#include "Mob.h"
#include "ObjModel.h"
#include "Shader.h"
#include "GameManager.h"
#include "Level.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <ctime>
#include <random>
#include "Camera.h"

void Mob::init()
{
	shaderID = initShader("simple");
	//pObjModel = new ObjModel("c:\\VrCave\\Development\\SwordArtOffline\\Data\\portalbutton\\portalbutton.obj");
	pObjModel = new ObjModel("c:\\VrCave\\Development\\SwordArtOffline\\Data\\wolf3\\Wolf.obj");
	//pObjModel = new ObjModel("c:\\VrCave\\Development\\SwordArtOffline\\Data\\wolf2\\Wolf.obj");
	//pObjModel = new ObjModel("c:\\VrCave\\Development\\SwordArtOffline\\Data\\boss\\Mon.obj");
}

void Mob::draw()
{
	glUseProgram(shaderID);
	glPushMatrix();
	glScalef(0.5, 0.5, 0.5);

	glScalef(1, 1, -1);
	btTransform trans;
	btScalar m[16];
	pObjModel->rigidBody->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(m);
	glMultMatrixf(m);
	pObjModel->draw(shaderID);

	glPopMatrix();
	glUseProgram(0);
}

void Mob::update()
{
	pObjModel->rigidBody->applyForce(btVector3(rand() & 3, 0, rand() & 3), btVector3(fpCameraXCoordinate, fpCameraYCoordinate, fpCameraZCoordinate));
}