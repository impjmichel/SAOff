#ifndef CUBE_H
#define CUBE_H

#define CUBE_MASS 1.0

#include <btBulletDynamicsCommon.h>

class Cube
{
private:
	bool isMoving;

public:
	float xPosition, yPosition, zPosition;
	float xVelocity, yVelocity, zVelocity;
	float xScale, yScale, zScale;
	bool isAlive;
	btRigidBody* body = 0;

	Cube(float xPosition, float yPosition, float zPosition, float xVelocity, float yVelocity, float zVelocity, float xScale, float yScale, float zScale, bool isMoving, bool isAlive);
	~Cube();
	void draw();
	void update();
	bool Cube::isOutOfBounds(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);
	void setBody(btRigidBody* body);
};

#endif