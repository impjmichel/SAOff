#ifndef CAMERA_CHARACTER_H
#define CAMERA_CHARACTER_H

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