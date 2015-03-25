#include "StaticEnvironment.h"
#include <GL/glew.h>
#include "Mob.h"
#include "ObjModel.h"
#include "Shader.h"
#include "GameManager.h"
#include "Level.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

void StaticEnvironment::init()
{
	// test:
	shaderID = initShader("simple");
	pObjModel = new ObjModel("c:\\VrCave\\Development\\SwordArtOffline\\Data\\wolf3\\Wolf.obj");
}

void StaticEnvironment::draw()
{
	//test:
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
