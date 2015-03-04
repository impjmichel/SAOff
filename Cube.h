#ifndef CUBE_H
#define CUBE_H

#define CUBE_MASS 1.0

class Cube
{
public:
	float xPosition, yPosition, zPosition;
	float xVelocity, yVelocity, zVelocity;
	float xScale, yScale, zScale;
	bool isMoving, isAlive;
	btRigidBody* body;

	Cube(float xPosition, float yPosition, float zPosition, float xVelocity, float yVelocity, float zVelocity, float xScale, float yScale, float zScale, bool isMoving, bool isAlive);
	void draw();
	void update();
	bool Cube::isOutOfBounds(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);
	void setBody(btRigidBody* body);
};

#endif