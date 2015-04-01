#include "CameraCharacter.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

void CameraCharacter::draw()
{
	//glPushMatrix();

	//btTransform trans;
	//btScalar m[16];
	//rigidBody->getMotionState()->getWorldTransform(trans);
	//trans.getOpenGLMatrix(m);
	//glMultMatrixf(m);

	//glColor3f(1, 0, 0);
	//glScalef(100, 100, 100);
	//glBegin(GL_TRIANGLES);
	//glVertex3f(0, 0, 0);
	//glVertex3f(0, 1, 1);
	//glVertex3f(0, 1, 0);
	//glEnd();
	//glPopMatrix();
}

void CameraCharacter::init()
{
	groundShape = new btBoxShape(btVector3(1, 1, 1));

	btScalar mass = 0.0; //rigidbody is static if mass is zero, otherwise dynamic
	btVector3 localInertia(0, 0, 0);

	groundShape->calculateLocalInertia(mass, localInertia);

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, 2, 0));
	//btQuaternion quat(34.5, 0.0, 0.0);
	//btQuaternion quat(0, 0, 0);
	//quat.setRotation(btVector3(0.0, 1.0, 0.0), TO_RADIANS(180));
	//const btQuaternion &quaternion = quat;
	//groundTransform.setRotation(quaternion);

	myMotionState = new btDefaultMotionState(groundTransform); //motionstate provides interpolation capabilities, and only synchronizes 'active' objects
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
	rigidBody = new btRigidBody(rbInfo);
	rigidBody->setUserPointer(rigidBody);

	rigidBody->setRestitution(0);

	rigidBody->setAngularFactor(btVector3(0.0, 1.0, 0.0));
}

void CameraCharacter::update()
{
	//btTransform trans;
	//btScalar m[16];
	//rigidBody->getMotionState()->getWorldTransform(trans);
	//trans.getOpenGLMatrix(m);
	//glm::mat4 mat = glm::make_mat4(m);
	//glm::vec3 location = glm::vec3(mat[3]);
	//float xPosition = location[0];
	//float yPosition = location[1];
	//float zPosition = location[2];

	btTransform tr;
	myMotionState->getWorldTransform(tr);
	tr.setOrigin(btVector3(fpCameraXCoordinate, fpCameraYCoordinate, fpCameraZCoordinate));
	rigidBody->getMotionState()->setWorldTransform(tr);
}