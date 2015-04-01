#ifndef CAMERA_CHARACTER_H
#define CAMERA_CHARACTER_H

#define HEIGHT_OFFSET 1.5

#include <btBulletCollisionCommon.h>

class CameraCharacter{
public:
	btRigidBody *rigidBody;
	btCollisionShape *groundShape;
	btDefaultMotionState *myMotionState;

	void draw();
	void init();
	void update();
};

#endif