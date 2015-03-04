#include <GL\glew.h>
//#include <glm\vec3.hpp>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "Cube.h"

Cube::Cube(float xPosition, float yPosition, float zPosition, float xVelocity, float yVelocity, float zVelocity, float xScale, float yScale, float zScale, bool isMoving, bool isAlive) :
xPosition(xPosition), yPosition(yPosition), zPosition(zPosition),
xVelocity(xVelocity), yVelocity(yVelocity), zVelocity(zVelocity),
xScale(xScale), yScale(yScale), zScale(zScale),
isMoving(isMoving), isAlive(isAlive) {}

void Cube::draw()
{
	if (!isAlive)
		return;

	glPushMatrix();
	//glTranslatef(-xPosition, -yPosition, -zPosition);
	glScalef(1, 1, -1);
	btTransform trans;
	btScalar m[16];

	body->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(m);
	glMultMatrixf(m);

	glScalef(xScale, yScale, zScale);
	glBegin(GL_QUADS);
	// front
	//glNormal3f(0, 0, -1);
	glColor3f(1, 0, 0);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	// back
	//glNormal3f(0, 0, 1);
	glColor3f(0, 1, 0);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	// right
	//glNormal3f(1, 0, 0);
	glColor3f(0, 0, 1);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	// left
	//glNormal3f(-1, 0, 0);
	glColor3f(1, 1, 0);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	// top
	glColor3f(1, 0, 1);
	//glNormal3f(0, 1, 0);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	// bottom
	//glNormal3f(0, -1, 0);
	glColor3f(0, 1, 1);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glEnd();
	glPopMatrix();
}

void Cube::setBody(btRigidBody* body)
{
	this->body = body;
}

void Cube::update()
{
	btVector3 velocity = body->getLinearVelocity();
	//xVelocity = velocity.x();
	yVelocity = velocity.y();
	//zVelocity = velocity.z();

	//body->setLinearVelocity(btVector3(xVelocity, velocity.getY(), zVelocity));

	btQuaternion location = body->getOrientation();
	xPosition = location.x();
	yPosition = location.y();
	zPosition = location.z();
}

bool Cube::isOutOfBounds(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax)
{
	return (xPosition - xScale <= xMin || xPosition + xScale >= xMax ||
		yPosition - yScale <= yMin || yPosition + yScale >= yMax ||
		zPosition - zScale <= zMin || zPosition + zScale >= zMax);
}