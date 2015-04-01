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
#include <cmath>
#include <glm/gtc/type_ptr.hpp>

void Mob::init()
{
	shaderID = initShader("simple");
	//pObjModel = new ObjModel("c:\\VrCave\\Development\\SwordArtOffline\\Data\\portalbutton\\portalbutton.obj");
	pObjModel = new ObjModel("c:\\VrCave\\Development\\SwordArtOffline\\Data\\wolf3\\Wolf.obj");
	//pObjModel = new ObjModel("c:\\VrCave\\Development\\SwordArtOffline\\Data\\wolf2\\Wolf.obj");
	//pObjModel = new ObjModel("c:\\VrCave\\Development\\SwordArtOffline\\Data\\boss\\Mon.obj");

	pObjModel->rigidBody->setAngularFactor(btVector3(0.0, 0.0, 0.0));
}

void Mob::draw()
{
	glUseProgram(shaderID);
	GLint loc = glGetUniformLocation(shaderID, "glow");
	GLint loc2 = glGetUniformLocation(shaderID, "glowColor");
	btTransform trans;
	btScalar m[16];

	//Glow draw - Bigger
	glDepthMask(GL_FALSE);
	glUniform1i(loc, true);

	float healthPercentage = health / maxHealth;

	glUniform4f(loc2, 1.0 - healthPercentage, healthPercentage, 0.0, 0.1);

	glPushMatrix();
	//glScalef(1, 1, -1);
	pObjModel->rigidBody->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(m);
	glMultMatrixf(m);
	glScalef(1.01, 1.01, 1.01);
	glScalef(0.5, 0.5, 0.5);

	glTranslatef(0, -1, 0);
	pObjModel->draw(shaderID);

	glPopMatrix();

	//Normal draw
	glDepthMask(GL_TRUE);
	glUniform1i(loc, false);

	glPushMatrix();
	//glScalef(1, 1, -1);
	pObjModel->rigidBody->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(m);
	glMultMatrixf(m);
	glScalef(0.5, 0.5, 0.5);

	glTranslatef(0, -1, 0);
	pObjModel->draw(shaderID);

	glPopMatrix();

	glUseProgram(0);
}

void Mob::update()
{
	if (health != 0)
		health--;
	else
		health = maxHealth;

	//Activate
	pObjModel->rigidBody->setActivationState(1);

	//Get X and Z location
	btTransform trans;
	btScalar m[16];
	pObjModel->rigidBody->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(m);
	glm::mat4 mat = glm::make_mat4(m);
	glm::vec3 location = glm::vec3(mat[3]);
	float xPosition = location[0];
	float zPosition = location[2];

	//Direction to camera
	float xVel = (fpCameraXCoordinate) - xPosition;
	float zVel = (fpCameraZCoordinate) - zPosition;

	if (sqrt(pow(xVel, 2) + pow(zVel, 2)) > 20)
	{
		pObjModel->rigidBody->setLinearVelocity(btVector3(0, pObjModel->rigidBody->getLinearVelocity().y(), 0));
		return;
	}

	//Set X and Z velocity, keep Y the same.
	btVector3 vec = pObjModel->rigidBody->getLinearVelocity();
	float yValue = vec.y();
	vec.setX(((vec.x() * 2.) + (xVel * 1.)) / 3.);//TODO: Did vec.x() and average for fun, seem pretty good; check.
	vec.setY(yValue);
	vec.setZ(((vec.z() * 2.) + (zVel * 1.)) / 3.);//TODO: Did vec.z() and average for fun, seem pretty good; check.
	vec.normalize();
	vec *= speed;
	pObjModel->rigidBody->setLinearVelocity(vec);

	//Rotate front of wolf to face his velocity
	btQuaternion qNewOrientation;
	qNewOrientation.setEuler(PI * 0.5 - atan2(vec.z(), vec.x()), 0, 0);
	btTransform transBody(trans);
	transBody.setRotation(qNewOrientation);
	pObjModel->rigidBody->setCenterOfMassTransform(transBody);
}